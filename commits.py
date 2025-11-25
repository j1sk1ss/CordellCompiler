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

parser = argparse.ArgumentParser(description="Interactive atomic Git commit script")
parser.add_argument(
    "--folder", type=str, default=".",
    help="Folder to commit (default: entire project)"
)
args = parser.parse_args()

repo = Repo(".")
changed_files = [item.a_path for item in repo.index.diff(None)]
changed_files += repo.untracked_files

if args.folder == ".":
    files_to_commit = changed_files
else:
    files_to_commit = [f for f in changed_files if f.startswith(args.folder)]

if not files_to_commit:
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
}

commits_dict = {}
for f in files_to_commit:
    matched = False
    for folder, ctype in folder_to_commit_type.items():
        if f.startswith(folder):
            commits_dict.setdefault(ctype, []).append(f)
            matched = True
            break
    if not matched:
        commits_dict.setdefault("Code - Misc", []).append(f)

for commit_type, files in commits_dict.items():
    commit_tag = f"[{commit_type}]"
    print(f"\nFiles for commit {commit_tag}:")
    for f in files:
        print(f"  {f}")

    message_input = input("\nEnter commit message for this group: ").strip()
    if not message_input:
        print("Empty message. Skipping this group.")
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
