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

import argparse
from git import Repo
import textwrap
import os
from collections import defaultdict

parser = argparse.ArgumentParser(description="Interactive atomic Git commit script")
parser.add_argument(
    "--folder", type=str, default=".",
    help="Folder to commit (default: entire project)"
)
args = parser.parse_args()

repo = Repo(".")

changed_files = [item.a_path for item in repo.index.diff(None)]
changed_files += repo.untracked_files

if args.folder != ".":
    changed_files = [f for f in changed_files if f.startswith(args.folder)]

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
}

def get_module_root(f):
    parts = f.split(os.sep)
    if parts[0] == "include" and len(parts) > 2:
        return os.path.join("src", parts[1], parts[2])
    elif parts[0] in ["src", "tests"] and len(parts) > 2:
        return os.path.join(parts[0], parts[1], parts[2])
    elif parts[0] in ["docs"]:
        return parts[0]
    else:
        return parts[0]  # misc

module_files = defaultdict(list)
for f in changed_files:
    module_root = get_module_root(f)
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
        repo.index.add(files)
        repo.index.commit(final_message)
        print("Commit successful!")
    else:
        print("Commit skipped.")
