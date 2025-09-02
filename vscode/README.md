
# CPL (Cordell) — VS Code Extension
## Quick install
```bash
npm install
npm run build
vsce package
```

## Docker
```bash
docker build -t cpl-extension .
docker run --rm -v $(pwd):/app -v $(pwd)/output:/output cpl-extension
```
