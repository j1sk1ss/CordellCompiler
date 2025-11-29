"""
This is a sort of "Wiki" file containing the basic concepts for the current repository. It covers the main topics on how to create commits and how to name them.

There are two main commit types:
    - Documentation
    - Code

Each type has some subtypes:
    - Documentation
        - Misc
        - Module Documentation
    - Code
        - Tokenizer
        - Markupper
        - AST
        - HIR
        - LIR
        - CodeGen

Note: Commits should be atomic (one module per commit).
The commit naming convention is simple:

```
    [MODULE] <message>
```
"""

import os
import textwrap
import argparse

from git import Repo
from collections import defaultdict

parser: argparse.ArgumentParser = argparse.ArgumentParser(description="Interactive atomic Git commit script")
parser.add_argument("--folder", type=str, default=".", help="Folder to commit (default: entire project)")
parser.add_argument("--major", action="store_true", help="Make ONE commit for all changes")

args = parser.parse_args()
repo: Repo = Repo(".")

changed_files: list = [ item.a_path for item in repo.index.diff(None) ]
changed_files += repo.untracked_files

if args.folder != ".":
    changed_files: list = [ f for f in changed_files if f.startswith(args.folder) ]

if not changed_files:
    print("No changes detected.")
    exit(0)

folder_to_commit_type = {
    "docs": "Documentation",
    "src/prep": "Code - Tkn/Mrkp",
    "src/ast": "Code - AST",
    "src/hir": "Code - HIR",
    "src/lir": "Code - LIR",
    "src/asm": "Code - CodeGen",
    "src/sem": "Code - Semantic",
    "src/symtab": "Code - Symtable",
    "tests": "Tests",
    "std": "Std",
}

def _get_module_root(f) -> str:
    parts = f.split(os.sep)
    if parts[0] == "include" and len(parts) >= 2:
        module = parts[1]
        return os.path.join("src", module)

    if parts[0] == "src" and len(parts) >= 2:
        module = parts[1]
        return os.path.join("src", module)

    if parts[0] in ["docs", "std", "tests"]:
        return parts[0]

    return parts[0]

if args.major:
    print("\n--major flag detected: all changes will be committed together. Changed files:")
    for f in changed_files:
        print(f"  {f}")

    guessed_type = None
    for folder, ctype in folder_to_commit_type.items():
        if all(f.startswith(folder) for f in changed_files):
            guessed_type = ctype
            break

    if guessed_type is None:
        guessed_type = "Major"

    commit_tag = f"[{guessed_type}]"

    message_input = input("\nEnter commit message for ALL changes: ").strip()
    if not message_input:
        print("Empty message. Aborting.")
        exit(1)

    wrapped_lines = textwrap.wrap(message_input, width=50)
    final_message = commit_tag + " " + ("\n".join(wrapped_lines))

    print("\nFinal commit message:")
    print(final_message)

    choice = input("\nCommit ALL changes with this message? [y/n] ")
    if choice.lower() != "y":
        print("Commit cancelled.")
        exit(0)

    existing_files = [f for f in changed_files if os.path.exists(f)]
    deleted_files = [f for f in changed_files if not os.path.exists(f)]

    if existing_files:
        repo.index.add(existing_files)
    if deleted_files:
        repo.index.remove(deleted_files, working_tree=False)

    repo.index.commit(final_message)
    print("Major commit successful!")
    exit(0)

module_files: dict[list] = defaultdict(list)
for f in changed_files:
    module_root = _get_module_root(f)
    module_files[module_root].append(f)

for module_root, files in module_files.items():
    commit_type = "Code - Misc"
    for folder, ctype in folder_to_commit_type.items():
        if module_root.startswith(folder):
            commit_type = ctype
            break

    commit_tag = f"[{commit_type}]"

    print(f"\nFiles for commit {commit_tag} (module: {module_root}):")
    for f in files:
        print(f"  {f}")

    message_input = input("\nEnter commit message for this group: ").strip()
    if not message_input:
        print("Empty message. Skipping this module.")
        continue

    wrapped_lines = textwrap.wrap(message_input, width=50)
    final_message = commit_tag + " " + ("\n".join(wrapped_lines))

    print("\nFormatted commit message:")
    print(final_message)

    choice = input("\nCommit these files with this message? [y/n] ")
    if choice.lower() == "y":
        existing_files = [f for f in files if os.path.exists(f)]
        deleted_files = [f for f in files if not os.path.exists(f)]

        if existing_files:
            repo.index.add(existing_files)

        if deleted_files:
            repo.index.remove(deleted_files, working_tree=False)

        repo.index.commit(final_message)
        print("Commit successful!")
    else:
        print("Commit skipped.")