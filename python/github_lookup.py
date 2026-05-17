#!/usr/bin/env python3
"""Look up real names of GitHub users by username."""

import argparse
import sys
import urllib.request
import urllib.error
import json


def lookup_user(username: str, token: str = None) -> dict:
    url = f"https://api.github.com/users/{username}"
    req = urllib.request.Request(url, headers={"Accept": "application/vnd.github+json"})
    if token:
        req.add_header("Authorization", f"Bearer {token}")

    try:
        with urllib.request.urlopen(req) as response:
            return json.loads(response.read())
    except urllib.error.HTTPError as e:
        if e.code == 404:
            print(f"User '{username}' not found.", file=sys.stderr)
        elif e.code == 403:
            print("Rate limit exceeded. Use --token to authenticate.", file=sys.stderr)
        else:
            print(f"HTTP error {e.code}: {e.reason}", file=sys.stderr)
        sys.exit(1)
    except urllib.error.URLError as e:
        print(f"Network error: {e.reason}", file=sys.stderr)
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(description="Look up real names of GitHub users.")
    parser.add_argument("usernames", nargs="+", metavar="USERNAME", help="GitHub username(s) to look up")
    parser.add_argument("--token", "-t", metavar="TOKEN", help="GitHub personal access token (increases rate limit)")
    args = parser.parse_args()

    for username in args.usernames:
        data = lookup_user(username, args.token)
        name = data.get("name") or "(no name set)"
        print(f"{username}: {name}")


if __name__ == "__main__":
    main()
