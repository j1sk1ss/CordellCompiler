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
        
        start {
          int a = 10;
          short b = a + 20;
          exit a;
        }
        `,
          ptr: `**ptr** — Pointer data type.

        Example:
        
        start {
          ptr char a = 0;
          ptr short b = a + 255;
          exit 0;
        }
        `,
          glob: `**glob** — Global data type. Can be used in every scope and all linked files.

        Example:
        
        start {
          glob char a = 0;
          exit 0;
        }
        `,
          ro: `**ro** — Read only (const) data type. Can't be changed, but can be used in every scope and linked file.

        Example:
        
        start {
          ro char a = 0;
          exit 0;
        }
        `,

          exit: `**exit** — Program exit statement. Will invoke exit syscall and terminate program.

        Example:
        
        int a = 0;
        exit a;
        `,
          
          extern: `**extern** — Extern function or variable from outer space.

        Example:
        
        extern exfunc printf;
        extern ptr char frame_buffer;
        `,
          
          exfunc: `**exfunc** — External function type.

        Example:
        
        extern exfunc printf;
        `,
          
          syscall: `**syscall** — Invoke system call with optional argument count.
          Will push arguments to stack / registers according their position in function call.

        Example:
        
        syscall(arg1, arg2, arg3, ...);
        `,
          
          function: `**function** — Function definition keyword. CPL language don't provide return type selection.

        Example:
        
        function abs(int n) {
            return n;
        }
        `,
          long: `**long** - Long variable type. On 64-bit machines equals 64-bit value. Max: 9,223,372,036,854,775,807`,
          int: `**int** - Integer variable type. Size equals to 32-bit value. Max: 2,147,483,647`,
          short: `**short** - Short integer variable type. Size equals to 16-bit value. Max: 32,767`,
          char: `**char** - Character integer variable type. Size equals to 8-bit value. Max: 128`,
            
            arr: `**arr** - Array variable type. Allocate array on stack.

          Example:

          arr name[size, type] = { ... };
          `,
          
            str: `**str** - String variable type. Allocate string array on stack.

          Example:

          str name = "Hello, World!";
          `
        };


        if (docs[word]) {
          return new vscode.Hover(new vscode.MarkdownString(docs[word]));
        }
      }
    }
  );

  context.subscriptions.push(hoverProvider);
}

export function deactivate() {}
