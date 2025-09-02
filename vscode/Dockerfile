FROM node:20-alpine

RUN apk add --no-cache bash
RUN apk add --no-cache \
    git \
    python3 \
    make \
    g++

RUN npm install -g @vscode/vsce

WORKDIR /app

COPY package*.json ./
COPY . .

RUN npm install
RUN npm run build

RUN vsce package
RUN mkdir /output && mv *.vsix /output/

VOLUME /output
