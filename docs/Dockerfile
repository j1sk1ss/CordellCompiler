FROM node:18-alpine
RUN npm install -g docsify-cli
WORKDIR /docs
EXPOSE 3000
CMD ["docsify", "serve", ".", "--port", "3000"]
