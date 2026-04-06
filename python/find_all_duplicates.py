#!/usr/bin/env python3
"""Find (and optionally remove) duplicate files in a directory tree.

Usage examples:
    python find_all_duplicates.py /path/to/directory
    python find_all_duplicates.py /path/to/directory --remove
    python find_all_duplicates.py /path/to/directory --remove --confirm
"""

import argparse
import os
import sys
import hashlib
import stat
from datetime import datetime
from collections import defaultdict
from typing import Dict, List, Optional, Tuple


# ── Helpers ────────────────────────────────────────────────────────────────────

def validate_directory(path: str) -> Tuple[bool, str]:
    """Return (ok, reason) for the given directory path."""
    if not os.path.exists(path):
        return False, f"Path does not exist: {path}"
    if not os.path.isdir(path):
        return False, f"Path is not a directory: {path}"
    if not os.access(path, os.R_OK):
        return False, f"Directory is not readable: {path}"
    return True, ""


def format_size(size_bytes: int) -> str:
    """Return a human-readable file size string."""
    for unit in ("B", "KB", "MB", "GB", "TB"):
        if abs(size_bytes) < 1024:
            return f"{size_bytes:.1f} {unit}" if unit != "B" else f"{size_bytes} {unit}"
        size_bytes /= 1024
    return f"{size_bytes:.1f} PB"


def format_mtime(mtime: float) -> str:
    """Return a human-readable modification timestamp."""
    return datetime.fromtimestamp(mtime).strftime("%Y-%m-%d %H:%M:%S")


def get_file_hash(file_path: str, block_size: int = 65536) -> Optional[str]:
    """Calculate the SHA-256 hash of a file's contents.

    Returns None (and prints a warning) if the file cannot be read.
    """
    sha256 = hashlib.sha256()
    try:
        with open(file_path, "rb") as f:
            while True:
                chunk = f.read(block_size)
                if not chunk:
                    break
                sha256.update(chunk)
        return sha256.hexdigest()
    except PermissionError:
        print(f"  [WARNING] Permission denied: {file_path}")
    except OSError as e:
        print(f"  [WARNING] OS error reading {file_path}: {e}")
    return None


# ── Core logic ─────────────────────────────────────────────────────────────────

def collect_files(directory: str) -> Dict[int, List[str]]:
    """Walk *directory* and group regular files by size.

    Skips symlinks, special files, and unreadable entries.
    Returns {size: [path, …]} only for sizes with 2+ files.
    """
    size_map: Dict[int, List[str]] = defaultdict(list)
    skipped = 0

    for root, dirs, files in os.walk(directory, onerror=lambda e: print(f"  [WARNING] {e}")):
        for name in files:
            file_path = os.path.join(root, name)

            # Skip symlinks — they can cause loops and misleading duplicates.
            if os.path.islink(file_path):
                continue

            try:
                st = os.stat(file_path)
            except PermissionError:
                print(f"  [WARNING] Permission denied: {file_path}")
                skipped += 1
                continue
            except OSError as e:
                print(f"  [WARNING] Cannot stat {file_path}: {e}")
                skipped += 1
                continue

            # Only consider regular files.
            if not stat.S_ISREG(st.st_mode):
                continue

            size_map[st.st_size].append(file_path)

    if skipped:
        print(f"  [INFO] Skipped {skipped} inaccessible file(s) during scan.\n")

    # Keep only sizes that have potential duplicates.
    return {sz: paths for sz, paths in size_map.items() if len(paths) > 1}


def find_duplicates(directory: str) -> List[Tuple[str, List[str], int]]:
    """Return a list of duplicate groups.

    Each group is (kept_path, [duplicate_paths], file_size).
    The first file encountered is treated as the "original" to keep.
    """
    print("Phase 1: Scanning files and grouping by size …")
    size_groups = collect_files(directory)
    candidate_count = sum(len(p) for p in size_groups.values())
    print(f"  Found {candidate_count} files in {len(size_groups)} size group(s) to hash.\n")

    if not size_groups:
        return []

    print("Phase 2: Hashing candidates to identify duplicates …")
    hash_map: Dict[str, List[str]] = defaultdict(list)

    for size, paths in size_groups.items():
        for path in paths:
            file_hash = get_file_hash(path)
            if file_hash is not None:
                hash_map[file_hash].append(path)

    # Build result: only groups with actual duplicates.
    results: List[Tuple[str, List[str], int]] = []
    for file_hash, paths in hash_map.items():
        if len(paths) > 1:
            # Sort so the "original" is deterministic (lexicographic first path).
            paths.sort()
            kept = paths[0]
            duplicates = paths[1:]
            try:
                size = os.path.getsize(kept)
            except OSError:
                size = 0
            results.append((kept, duplicates, size))

    return results


# ── Display & removal ──────────────────────────────────────────────────────────

def print_report(groups: List[Tuple[str, List[str], int]]) -> None:
    """Print a human-readable report of duplicate groups."""
    if not groups:
        print("\nNo duplicate files found.")
        return

    total_dupes = sum(len(dupes) for _, dupes, _ in groups)
    wasted = sum(size * len(dupes) for _, dupes, size in groups)

    print(f"\n{'=' * 70}")
    print(f" DUPLICATE FILE REPORT")
    print(f"{'=' * 70}")
    print(f" Groups found  : {len(groups)}")
    print(f" Duplicate files: {total_dupes}")
    print(f" Wasted space   : {format_size(wasted)}")
    print(f"{'=' * 70}\n")

    for idx, (kept, dupes, size) in enumerate(groups, 1):
        print(f"── Group {idx} ({format_size(size)} each, {len(dupes)} duplicate(s)) "
              f"{'─' * 30}")
        try:
            mtime = format_mtime(os.path.getmtime(kept))
        except OSError:
            mtime = "unknown"
        print(f"  [KEEP]  {kept}")
        print(f"          Modified: {mtime}")

        for dup in dupes:
            try:
                mtime = format_mtime(os.path.getmtime(dup))
            except OSError:
                mtime = "unknown"
            print(f"  [DUP]   {dup}")
            print(f"          Modified: {mtime}")
        print()


def remove_duplicates(groups: List[Tuple[str, List[str], int]],
                      confirm: bool = False) -> Tuple[int, int]:
    """Delete duplicate files. Returns (deleted_count, error_count)."""
    deleted = 0
    errors = 0

    for kept, dupes, size in groups:
        for dup in dupes:
            if confirm:
                try:
                    answer = input(f"  Delete {dup} ? [y/N] ").strip().lower()
                except (EOFError, KeyboardInterrupt):
                    print("\n  Removal cancelled by user.")
                    return deleted, errors
                if answer != "y":
                    print(f"  Skipped: {dup}")
                    continue

            try:
                os.remove(dup)
                print(f"  Deleted: {dup}")
                deleted += 1
            except PermissionError:
                print(f"  [ERROR] Permission denied — cannot delete: {dup}")
                errors += 1
            except OSError as e:
                print(f"  [ERROR] Failed to delete {dup}: {e}")
                errors += 1

    return deleted, errors


# ── Entry point ────────────────────────────────────────────────────────────────

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Find (and optionally remove) duplicate files in a directory tree.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="Examples:\n"
               "  %(prog)s /photos\n"
               "  %(prog)s /photos --remove\n"
               "  %(prog)s /photos --remove --confirm\n",
    )
    parser.add_argument("directory", type=str,
                        help="Root directory to search for duplicates")
    parser.add_argument("--remove", action="store_true",
                        help="Delete duplicate files (keeps the lexicographic first path)")
    parser.add_argument("--confirm", action="store_true",
                        help="Prompt for confirmation before each deletion (implies --remove)")

    args = parser.parse_args()

    # --confirm implies --remove
    if args.confirm:
        args.remove = True

    ok, reason = validate_directory(args.directory)
    if not ok:
        print(f"Error: {reason}")
        return 1

    abs_dir = os.path.abspath(args.directory)
    print(f"Searching for duplicates in: {abs_dir}\n")

    groups = find_duplicates(abs_dir)
    print_report(groups)

    if not groups:
        return 0

    if args.remove:
        print(f"{'=' * 70}")
        print(" REMOVING DUPLICATES" + (" (with confirmation)" if args.confirm else ""))
        print(f"{'=' * 70}\n")
        deleted, errors = remove_duplicates(groups, confirm=args.confirm)
        print(f"\nRemoval complete — {deleted} file(s) deleted, {errors} error(s).")
        return 1 if errors else 0

    return 0


if __name__ == "__main__":
    sys.exit(main())
    