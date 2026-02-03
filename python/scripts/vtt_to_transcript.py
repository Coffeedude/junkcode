#!/usr/bin/env python3
import re
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
                print(f"[{current_speaker}]: {text}")
                first_paragraph = False

    def normalize_speaker(name):
        name = name.strip()
        if name.startswith("[") and name.endswith("]") and len(name) > 1:
            name = name[1:-1].strip()
        return name or "Unknown"

    def push_segment(speaker, text):
        nonlocal current_speaker, buffer
        if not text:
            return
        if speaker != current_speaker:
            flush()
            current_speaker = speaker
            buffer = [text]
        else:
            buffer.append(text)

    speaker_regex = re.compile(r"^\s*([^:\n]{1,80}):\s+", re.MULTILINE)

    for caption in webvtt.read(sys.argv[1]):
        speaker = normalize_speaker(caption.voice or "Unknown")
        text = caption.text.strip()
        if not text:
            continue

        if speaker == "Unknown":
            matches = list(speaker_regex.finditer(text))
            if matches:
                for idx, match in enumerate(matches):
                    name = normalize_speaker(match.group(1))
                    start = match.end()
                    end = matches[idx + 1].start() if idx + 1 < len(matches) else len(text)
                    segment = text[start:end].strip()
                    push_segment(name, segment)
                continue
            if current_speaker is None:
                continue

        push_segment(speaker, text)

    flush()


if __name__ == "__main__":
    main()
