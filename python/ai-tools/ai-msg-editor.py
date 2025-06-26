#!/usr/bin/python3 

import sys
import argparse
import os
from openai import OpenAI

def build_prompt(style: str, input_text: str) -> str:
    style_prompts = {
        "proofread": "Please proofread and correct the following text for "
            "grammar, spelling, and clarity. Do not change the original tone "
            "of the author. Output the corrected text:",
        "professional": "Rewrite the following text in a clear, "
            "professional, and polished manner suitable for business "
            "communication:",
        "concise": "Rewrite the following text to be brief, clear, "
            "and to the point, without losing essential meaning:",
        "personal": "Rewrite the following text in a warm, friendly, "
            "and personal tone, as if writing to a close colleague or "
            "friend. Do not be overly energetic or enthusiatistic. Do not "
            "use exclamation points or other exagerations. Do not use "
            "exaggerated language or hyperbole:",
        "social": "Rewrite the following text in an informal, casual "
            "style suitable for social media posts. The post shoudl be "
            "written in first person and should be concise and engaging: "
    }
    prompt_intro = style_prompts.get(style.lower())
    if not prompt_intro:
        raise ValueError(f"Unknown style: {style}")
    return f"{prompt_intro}\n\n\"\"\"\n{input_text}\n\"\"\""

def main():
    parser = argparse.ArgumentParser(description="Rewrite text in different writing styles using OpenAI GPT.")
    parser.add_argument('--style', type=str, required=True,
                        choices=["proofread", "professional", "concise", "personal", "social"],
                        help="The writing style to use for rewriting the input text.")
    args = parser.parse_args()

    input_text = sys.stdin.read().strip()
    if not input_text:
        print("No input text provided.", file=sys.stderr)
        sys.exit(1)

    client = OpenAI(api_key=os.environ['OPENAI_API_KEY'])
    if not client.api_key:
        print("OPENAI_API_KEY environment variable is not set.", file=sys.stderr)
        sys.exit(1)

    prompt = build_prompt(args.style, input_text)

    try:
        response = client.chat.completions.create(
            model="gpt-4.1-mini",  
            messages=[
                {"role": "system", "content": "You are a helpful assistant specializing in rewriting text."},
                {"role": "user", "content": prompt},
            ],
            max_tokens=500,
            temperature=0.7,
        )
        
        rewritten_text = response.choices[0].message.content.strip()
        print(rewritten_text)
    
    except OpenAI.RateLimitError as e:
        print(f"Rate limit exceeded. Please wait a few minutes and try again.", file=sys.stderr)
        print(f"Error details: {str(e)}", file=sys.stderr)
        sys.exit(1)
    
    except OpenAI.AuthenticationError:
        print("Invalid API key. Please check your OPENAI_API_KEY environment variable.", file=sys.stderr)
        sys.exit(1)
    
    except OpenAI.APIError as e:
        print(f"API error occurred: {str(e)}", file=sys.stderr)
        sys.exit(1)
    
    except Exception as e:
        print(f"An unexpected error occurred: {str(e)}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
