"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;

import * as vscode from 'vscode';

export function activate(context: vscode.ExtensionContext) {
  const keywords = [
    'start','exit', 'exfunc', 'function','return',
    'if','else','while','switch','case','default',
    'glob','ro','ptr','arr','extern','from','import', 'extern', 'syscall',
    'long','int','short','char','str'
  ];

  const provider = vscode.languages.registerCompletionItemProvider(
    { language: 'cpl', scheme: 'file' },
    {
      provideCompletionItems(document, position) {
        const items: vscode.CompletionItem[] = [];

        for (const kw of keywords) {
          const item = new vscode.CompletionItem(kw, vscode.CompletionItemKind.Keyword);
          item.insertText = kw;
          items.push(item);
        }

        // Snippets
        const fn = new vscode.CompletionItem('function …', vscode.CompletionItemKind.Snippet);
        fn.insertText = new vscode.SnippetString('function ${1:name}(${2:args}) {\n\t$0\n}');
        fn.detail = 'CPL function';
        items.push(fn);

        const whileSn = new vscode.CompletionItem('while … else …', vscode.CompletionItemKind.Snippet);
        whileSn.insertText = new vscode.SnippetString('while ${1:cond}; {\n\t$0\n}\nelse {\n\t$0\n}');
        whileSn.detail = 'CPL while loop';
        items.push(whileSn);

        const ifElse = new vscode.CompletionItem('if … else …', vscode.CompletionItemKind.Snippet);
        ifElse.insertText = new vscode.SnippetString('if ${1:cond}; {\n\t$2\n}\nelse {\n\t$0\n}');
        ifElse.detail = 'CPL if/else';
        items.push(ifElse);

        const startTemplate = new vscode.CompletionItem('program template', vscode.CompletionItemKind.Snippet);
        startTemplate.insertText = new vscode.SnippetString('{' + '\n\tstart {' + '\n\t\t$0' + '\n\t} exit ${1:0};' + '\n}');
        startTemplate.detail = 'CPL program skeleton';
        items.push(startTemplate);

        const syscallWrap = new vscode.CompletionItem('syscall(…)', vscode.CompletionItemKind.Snippet);
        syscallWrap.insertText = new vscode.SnippetString('syscall(${1:a}, ${2:b}, ${3:c}, ${4:d});');
        syscallWrap.detail = 'CPL syscall';
        items.push(syscallWrap);

        const externExfunc = new vscode.CompletionItem('extern exfunc …', vscode.CompletionItemKind.Snippet);
        externExfunc.insertText = new vscode.SnippetString('extern exfunc ${1:name}(${2:args});');
        externExfunc.detail = 'CPL extern function declaration';
        items.push(externExfunc);
        return items;
      }
    }
  );

  const hoverProvider = vscode.languages.registerHoverProvider(
    { language: 'cpl', scheme: 'file' },
    {
      provideHover(document, position) {
        const range = document.getWordRangeAtPosition(position);
        if (!range) return;
        const word = document.getText(range);

        const docs: Record<string, string> = {
          start: `**start** — Program entry point.

        Example:
        \`\`\`cpl
        start {
          int a = 10;
          short b = a + 20;
          exit a;
        }
        \`\`\``,
          ptr: `**ptr** — Pointer data type.

        Example:
        \`\`\`cpl
        start {
          ptr char a = 0;
          ptr short b = a + 255;
          exit 0;
        }
        \`\`\``,
          glob: `**glob** — Global data type. Can be used in every scope and all linked files.

        Example:
        \`\`\`cpl
        start {
          glob char a = 0;
          exit 0;
        }
        \`\`\``,
          ro: `**ro** — Read only (const) data type. Can't be changed, but can be used in every scope and linked file.

        Example:
        \`\`\`cpl
        start {
          ro char a = 0;
          exit 0;
        }
        \`\`\``,

          exit: `**exit** — Program exit statement. Will invoke exit syscall and terminate program.

        Example:
        \`\`\`cpl
        int a = 0;
        exit a;
        \`\`\``,
          
          extern: `**extern** — Extern function or variable from outer space.

        Example:
        \`\`\`cpl
        extern exfunc printf;
        extern ptr char frame_buffer;
        \`\`\``,
          
          exfunc: `**exfunc** — External function type.

        Example:
        \`\`\`cpl
        extern exfunc printf;
        \`\`\``,
          
          syscall: `**syscall** — Invoke system call with optional argument count.
          Will push arguments to stack / registers according their position in function call.

        Example:
        \`\`\`cpl
        syscall(arg1, arg2, arg3, ...);
        \`\`\``,
          
          function: `**function** — Function definition keyword. CPL language don't provide return type selection.

        Example:
        \`\`\`cpl
        function abs(int n) {
            return n;
        }
        \`\`\``
        };


        if (docs[word]) {
          return new vscode.Hover(new vscode.MarkdownString(docs[word]));
        }
      }
    }
  );

  context.subscriptions.push(provider);
  context.subscriptions.push(hoverProvider);
}

export function deactivate() {}
