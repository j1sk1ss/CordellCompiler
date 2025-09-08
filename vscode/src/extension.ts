"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;

import * as vscode from 'vscode';

export function activate(context: vscode.ExtensionContext) {
  const keywords = [
    'start', 'exit', 'exfunc', 'function', 'return',
    'if', 'else', 'while', 'switch', 'case', 'default',
    'glob', 'ro', 'dref', 'ref', 'ptr', 'extern', 'from', 'import', 'extern', 'syscall',
    'i64', 'i32', 'i16', 'i8', 'u64', 'u32', 'u16', 'u8', 'str', 'arr'
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
        
        start(long argc, ptr long argv) {
          long a = 0;
          while a < argc; {
            ptr char arg = argv[a];
            a = a + 1;
          }

          exit 0;
        }
        `,
          ref: `**ref** — Reference keyword. Get element address.

        start() {
          int a = 10;
          ptr long b = ref a;
          b[0] = 11;
          exit a; : 11 :
        }
        `,
        dref: `**dref** — De-reference keyword. Get element value by address.

        start() {
          int a = 10;
          ptr long b = ref a;
          dref b = 11;
          exit a; : 11 :
        }
        `,
          ptr: `**ptr** — Pointer data type.
        
        start() {
          ptr char a = 0;
          ptr short b = a + 255;
          exit 0;
        }
        `,
          glob: `**glob** — Global data type. Can be used in every scope and all linked files.
        
        start() {
          glob char a = 0;
          exit 0;
        }
        `,
          ro: `**ro** — Read only (const) data type. Can't be changed, but can be used in every scope and linked file.
        
        start() {
          ro char a = 0;
          exit 0;
        }
        `,

          exit: `**exit** — Program exit statement. Will invoke exit syscall and terminate program.

        int a = 0;
        exit a;
        `,
          
          extern: `**extern** — Extern function or variable from outer space.

        extern exfunc printf;
        extern ptr char frame_buffer;
        `,
          
          exfunc: `**exfunc** — External function type.

        extern exfunc printf;
        `,
          
          syscall: `**syscall** — Invoke system call with optional argument count.
          Will push arguments to stack / registers according their position in function call.

        syscall(arg1, arg2, arg3, ...);
        `,
          
          function: `**function** — Function definition keyword. CPL language don't provide return type selection.

        function abs(int n) {
            return n;
        } : -> int :
        `,
          i64: `**long** - Long variable type. On 64-bit machines equals 64-bit value. Max: 9,223,372,036,854,775,807`,
          i32: `**int** - Integer variable type. Size equals to 32-bit value. Max: 2,147,483,647`,
          i16: `**short** - Short integer variable type. Size equals to 16-bit value. Max: 32,767`,
          i8: `**char** - Character integer variable type. Size equals to 8-bit value. Max: 128`,
          u64: `**long** - Long variable type. On 64-bit machines equals 64-bit value. Max: 9,223,372,036,854,775,807`,
          u32: `**int** - Integer variable type. Size equals to 32-bit value. Max: 2,147,483,647`,
          u16: `**short** - Short integer variable type. Size equals to 16-bit value. Max: 32,767`,
          u8: `**char** - Character integer variable type. Size equals to 8-bit value. Max: 128`,
            
          arr: `**arr** - Array variable type. Allocate array on stack.

        arr name[size, type] = { ... };
          `,
          
          str: `**str** - String variable type. Allocate string array on stack.

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
