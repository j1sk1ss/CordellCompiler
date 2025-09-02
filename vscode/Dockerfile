FROM node:20-alpine

RUN apk add --no-cache bash git python3 make g++
RUN npm install -g @vscode/vsce

WORKDIR /app

ENTRYPOINT [ "sh", "-c", "npm install && npm run build && vsce package --allow-missing-repository -o /output/extension.vsix" ]
