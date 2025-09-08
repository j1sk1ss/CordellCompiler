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
        
        start(i64 argc, ptr u64 argv) {
          i64 a = 0;
          while a < argc; {
            ptr i8 arg = argv[a];
            a = a + 1;
          }

          exit 0;
        }
        `,
          ref: `**ref** — Reference keyword. Get element address.

        start() {
          i32 a = 10;
          ptr u64 b = ref a;
          b[0] = 11;
          exit a; : 11 :
        }
        `,
        dref: `**dref** — De-reference keyword. Get element value by address.

        start() {
          i32 a = 10;
          ptr u64 b = ref a;
          dref b = 11;
          exit a; : 11 :
        }
        `,
          ptr: `**ptr** — Pointer data type.
        
        start() {
          ptr i8 a = 0;
          ptr i16 b = a + 255;
          exit 0;
        }
        `,
          glob: `**glob** — Global data type. Can be used in every scope and all linked files.
        
        start() {
          glob i8 a = 0;
          exit 0;
        }
        `,
          ro: `**ro** — Read only (const) data type. Can't be changed, but can be used in every scope and linked file.
        
        start() {
          ro i8 a = 0;
          exit 0;
        }
        `,

          exit: `**exit** — Program exit statement. Will invoke exit syscall and terminate program.
          Exit code is u8 value.

        u8 a = 0;
        exit a;
        `,
          
          extern: `**extern** — Extern function or variable from outer space.

        extern exfunc printf;
        extern ptr u8 frame_buffer;
        `,
          
          exfunc: `**exfunc** — External function type.

        extern exfunc printf;
        `,
          
          syscall: `**syscall** — Invoke system call with optional argument count.
          Will push arguments to stack / registers according their position in function call.

        syscall(arg1, arg2, arg3, ...);
        `,
          
          function: `**function** — Function definition keyword. CPL language don't provide return type selection.

        function abs(i32 n) {
            return n;
        } : -> i32 :
        `,
          i64: `**i64** - Long variable type. On 64-bit machines equals 64-bit value. Max: 9,223,372,036,854,775,807`,
          i32: `**i32** - Integer variable type. Size equals to 32-bit value. Max: 2,147,483,647`,
          i16: `**i16** - Short integer variable type. Size equals to 16-bit value. Max: 32,767`,
          i8: `**i8** - Character integer variable type. Size equals to 8-bit value. Max: 128`,
          u64: `**u64** - Long variable type. On 64-bit machines equals 64-bit value. Max: 18,446,744,073,709,551,615`,
          u32: `**u32** - Integer variable type. Size equals to 32-bit value. Max: 4,294,967,295`,
          u16: `**u16** - Short integer variable type. Size equals to 16-bit value. Max: 65,535`,
          u8: `**u8** - Character integer variable type. Size equals to 8-bit value. Max: 256`,
            
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
