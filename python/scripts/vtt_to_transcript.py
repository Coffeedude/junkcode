#!/usr/bin/env python3
import sys
import webvtt


def main():
    if len(sys.argv) != 2:
        print("Usage: vtt_to_transcript.py /path/to/transcript.vtt", file=sys.stderr)
        sys.exit(2)

    current_speaker = None
    buffer = []
    first_paragraph = True

    def flush():
        nonlocal first_paragraph
        if current_speaker is not None and buffer:
            text = " ".join(buffer).strip()
            if text:
                if not first_paragraph:
                    print()
                print(f"{current_speaker}: {text}")
                first_paragraph = False

    for caption in webvtt.read(sys.argv[1]):
        speaker = caption.voice or "Unknown"
        text = caption.text.strip()
        if not text:
            continue

        if speaker != current_speaker:
            flush()
            current_speaker = speaker
            buffer = [text]
        else:
            buffer.append(text)

    flush()


if __name__ == "__main__":
    main()
