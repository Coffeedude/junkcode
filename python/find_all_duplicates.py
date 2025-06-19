import argparse
import os
import hashlib
from pathlib import Path
from collections import defaultdict

def validate_directory(path):
    """Validate that the provided path is a valid directory."""
    if not os.path.exists(path):
        return False
    if not os.path.isdir(path):
        return False
    return True

def get_file_hash(file_path):
    """Calculate the SHA256 hash of a file's contents."""
    hash_sha256 = hashlib.sha256()
    try:
        with open(file_path, "rb") as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_sha256.update(chunk)
        return hash_sha256.hexdigest()
    except Exception as e:
        print(f"Error reading file {file_path}: {str(e)}")
        return None

def find_duplicates(directory):
    """Find duplicate files in the specified directory."""
    # Dictionary to store file hashes and their paths
    file_hashes = defaultdict(list)
    
    try:
        # Walk through all files in the directory
        for root, _, files in os.walk(directory):
            for file in files:
                file_path = os.path.join(root, file)
                try:
                    # Get file size
                    file_size = os.path.getsize(file_path)
                    
                    # Calculate hash for files of the same size
                    file_hash = get_file_hash(file_path)
                    if file_hash:
                        file_hashes[file_hash].append(file_path)
                except Exception as e:
                    print(f"Error processing file {file_path}: {str(e)}")
    except Exception as e:
        print(f"Error processing files: {str(e)}")

    # Print duplicates
    try:
        for file_hash, paths in file_hashes.items():
            if len(paths) > 1:  # Only show if there are duplicates
                print(f"\nOriginal file: {paths[0]}")
                print("Duplicate files:")
                for duplicate in paths[1:]:
                    print(f"  - {duplicate}")
    except Exception as e:
        print(f"Error processing duplicates: {str(e)}")

def main():
    parser = argparse.ArgumentParser(
        description='Find duplicate files in a directory')
    parser.add_argument('directory', type=str,
                       help='Path to the directory to search for duplicates')
    
    args = parser.parse_args()
    
    if not validate_directory(args.directory):
        print("Error: Please provide a valid directory path.")
        print("Usage: python find_all_duplicates.py /path/to/directory")
        return
    
    print(f"Searching for duplicates in {args.directory}...")
    find_duplicates(args.directory)

if __name__ == "__main__":
    main()