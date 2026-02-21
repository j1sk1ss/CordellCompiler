
# CPL — VS Code Extension
This is a little extension for VScode for working with this hobby language. Source code can be found in `snippets/`, `src/` and `syntaxes/`. This extension uses a simple code server to a spell-checking, a gramma-checking and a basic semantic checking.

<p align="center">
    <img src="syntax_highlight.png" alt="Extension work example"/>
</p>

It works similary to the first steps in the compiler. It breaks a code by tokens, parses these tokens (without AST build) and checks gramma with the functions and variables registration. 

## Quick install
If you want to build extention on your host machine, just execute the next sequence of commands:
```bash
npm install
npm run build
vsce package
```

## Docker
If you'd prefer the Docker as a base for the building, execute the next querry:
```bash
docker build -t cpl-extension .
docker run --rm -v $(pwd):/app -v $(pwd)/output:/output cpl-extension
```
