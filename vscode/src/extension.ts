import * as path from "path";
import * as vscode from "vscode";
import { LanguageClient, LanguageClientOptions, ServerOptions, TransportKind } from "vscode-languageclient/node";

let client: LanguageClient | undefined;

export function activate(context: vscode.ExtensionContext) {
  const keywords = [
    "start","exit","function","return",
    "if","else","while","loop","switch","case","default",
    "glob","ro","dref","ref","ptr","lis","break","extern","from","import","syscall","asm","as",
    "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","not","poparg","section","align"
  ];

  const docs: Record<string, string> = {
    // entry / functions
    start: `**start** — Entry point.
  
  \`\`\`cpl
  start(i64 argc, ptr u64 argv) {
    : argc — argument count :
    : argv — pointer to array of pointers :
    exit 0;
  }
  \`\`\`
  `,
  
    function: `**function** — Function definition.
  
  \`\`\`cpl
  function abs(i32 x) -> i32 {
    if x < 0; { return x * -1; }
    return x;
  }
  \`\`\`
  `,
  
    return: `**return** — Return from function (optional expression).
  
  \`\`\`cpl
  function id(i32 x) -> i32 { return x; }
  function noop() -> i0 { return; }
  \`\`\`
  `,
  
    // control flow
    if: `**if** — Conditional statement.
  
  Syntax: \`if <expr> ; <block> [else <block>]\`
  
  \`\`\`cpl
  if a == 0; {
    exit 1;
  } 
  else {
    exit 0;
  }
  \`\`\`
  `,
  
    else: `**else** — Alternative block for **if**.
  
  \`\`\`cpl
  if ok; { exit 0; } else { exit 1; }
  \`\`\`
  `,
  
    while: `**while** — Loop with condition.
  
  Syntax: \`while <expr> ; <block>\`
  
  \`\`\`cpl
  i32 i = 0;
  while i < 10; {
    i += 1;
  }
  \`\`\`
  `,
  
    loop: `**loop** — Infinite loop (no condition).
  
  \`\`\`cpl
  loop {
    lis "loop";
    break;
  }
  \`\`\`
  `,
  
    switch: `**switch** — Multi-branch by value.
  
  \`\`\`cpl
  switch(x) {
    case 0; { exit 0; }
    case 1; { break;  }
    default { exit 2; }
  }
  \`\`\`
  `,
  
    case: `**case** — Case label inside **switch**.
  
  \`\`\`cpl
  switch(x) {
    case 10; { exit 10; }
  }
  \`\`\`
  `,
  
    default: `**default** — Default branch inside **switch** (optional).
  
  \`\`\`cpl
  switch(x) {
    default { exit 0; }
  }
  \`\`\`
  `,
  
    break: `**break** — Break current **while/loop/switch** block.
  
  \`\`\`cpl
  while 1; {
    break;
  }
  \`\`\`
  `,
  
    // storage / linkage
    glob: `**glob** — Global storage: visible across scopes / linked units.
  
  \`\`\`cpl
  glob i32 counter = 0;
  glob function foo(i32 x);
  \`\`\`
  `,
  
    ro: `**ro** — Read-only storage (const-like).
  
  \`\`\`cpl
  ro str hello = "Hello";
  \`\`\`
  `,
  
    extern: `**extern** — Declaration from outside (imported/linked symbol).
  
  \`\`\`cpl
  extern function printf(ptr i8 fmt, ...);
  extern ptr u8 frame_buffer
  \`\`\`
  `,
  
    // modules
    from: `**from** — Import from module/file (frontend directive).
  
  \`\`\`cpl
  from "lib.cpl" import foo, bar
  \`\`\`
  `,
  
    import: `**import** — Import list keyword (used with **from**).
  
  \`\`\`cpl
  from "lib.cpl" import foo, bar
  \`\`\`
  `,

    section: `**section** — Section declaration block for top-level items.

  \`\`\`cpl
  section(".text") {
    function foo() -> i32 { return 0; }
  }
  \`\`\`
  `,

    align: `**align** — Alignment wrapper for declarations.

  Single declaration:
  \`\`\`cpl
  align(16) i32 x = 0;
  \`\`\`

  Declaration block:
  \`\`\`cpl
  align(32) {
    i32 a = 0;
    ptr i8 p = 0;
  }
  \`\`\`
  `,
  
    // low-level
    syscall: `**syscall** — System call invocation.
  
  \`\`\`cpl
  syscall(0x2000004, 1, ref msg, strlen(ref msg));
  \`\`\`
  `,
  
    asm: `**asm** — Inline assembly block (arguments may be any expressions).
  
  \`\`\`cpl
  u64 out = 0;
  asm(10 + 10, foo(), out as ptr i0) {
    "xor rax, rax",
    "mov rax, 1",
    "mov %0, rax"
  }
  \`\`\`
  `,
  
    lis: `**lis** — Debug breakpoint marker / trace hook.
  
  \`\`\`cpl
  lis "comment";
  \`\`\`
  `,
  
    exit: `**exit** — Program termination (exit code expression).
  
  \`\`\`cpl
  exit 0;
  \`\`\`
  `,
  
    // operators / casts
    ref: `**ref** — Address-of (reference) operator.
  
  \`\`\`cpl
  i32 a = 10;
  ptr i32 p = ref a;
  p[0] = 11;
  exit a;
  \`\`\`
  `,
  
    dref: `**dref** — Dereference operator (value by address).
  
  \`\`\`cpl
  ptr i32 p = ref a;
  dref p = 123; : store through pointer :
  \`\`\`
  `,
  
    not: `**not** — Logical NOT (0 -> 1, non-0 -> 0).
  
  \`\`\`cpl
  i32 x = not 0; : 1 :
  \`\`\`
  `,
  
    poparg: `**poparg** — Pop a function's argument.

  \`\`\`cpl
  function max(...) {
    ptr u8 chloe = poparg as ptr u8;
  }
  \`\`\`
  `,
  
    as: `**as** — Cast expression to a type (postfix).
  
  \`\`\`cpl
  u64 x = 255 as u64;
  ptr u8 p = (ref x) as ptr u8;
  \`\`\`
  `,
  
    // types
    f64: `**f64** — 64-bit float.`,
    f32: `**f32** — 32-bit float.`,
    i64: `**i64** — Signed 64-bit integer.`,
    i32: `**i32** — Signed 32-bit integer.`,
    i16: `**i16** — Signed 16-bit integer.`,
    i8:  `**i8** — Signed 8-bit integer.`,
    u64: `**u64** — Unsigned 64-bit integer.`,
    u32: `**u32** — Unsigned 32-bit integer.`,
    u16: `**u16** — Unsigned 16-bit integer.`,
    u8:  `**u8** — Unsigned 8-bit integer.`,
    i0:  `**i0** — Void type (no value).`,
  
    str: `**str** — String (stack allocated literal/array).
  
  \`\`\`cpl
  str msg = "Hello";
  \`\`\`
  `,
  
    ptr: `**ptr** — Pointer type constructor.
  
  \`\`\`cpl
  ptr u8 p = 0;
  ptr i32 q = p as ptr i32;
  \`\`\`
  `,
  
    arr: `**arr** — Array keyword (declaration) OR array type constructor.
  
  Declaration form (grammar \`arr_decl\`):
  \`\`\`cpl
  arr buf[16 u8] = {1,2,3};
  \`\`\`
  
  Type form (grammar \`arr[<int>, <type>]\`):
  \`\`\`cpl
  arr[16, u8] a;
  \`\`\`
  `,
  };  

  const hoverProvider = vscode.languages.registerHoverProvider(
    { language: "cpl", scheme: "file" },
    {
      provideHover(document, position) {
        const range = document.getWordRangeAtPosition(
          position,
          /0x[0-9a-fA-F]+|0b[01]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?|'[^']'|[A-Za-z_][A-Za-z0-9_]*/ 
        );
        if (!range) return;
        const word = document.getText(range);        

        if (/^(0x[0-9a-fA-F]+|0b[01]+|0[0-7]*|[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?|'.')$/.test(word)) {
          let value: number;
          let type: string;

          if (word.startsWith("'") && word.endsWith("'")) {
            value = word.charCodeAt(1);
            type = "u8";
          } 
          else if (word.startsWith("0x") || word.startsWith("0X")) {
            value = parseInt(word, 16);
            type = value > 0xFFFFFFFF ? "u64" : value > 0xFFFF ? "u32" : value > 0xFF ? "u16" : "u8";
          } 
          else if (word.startsWith("0b") || word.startsWith("0B")) {
            value = parseInt(word.slice(2), 2);
            type = value > 0xFFFFFFFF ? "u64" : value > 0xFFFF ? "u32" : value > 0xFF ? "u16" : "u8";
          } 
          else if (word.startsWith("0") && word.length > 1 && !word.includes(".")) {
            value = parseInt(word, 8);
            type = value > 0xFFFFFFFF ? "u64" : value > 0xFFFF ? "u32" : value > 0xFF ? "u16" : "u8";
          } 
          else if (word.includes(".") || /[eE]/.test(word)) {
            const f = parseFloat(word);
            type = "f64";
            const buffer = new ArrayBuffer(8);
            new DataView(buffer).setFloat64(0, f, false);
            const high = new DataView(buffer).getUint32(0, false);
            const low = new DataView(buffer).getUint32(4, false);
            const bits = (BigInt(high) << 32n) | BigInt(low);
            value = Number(bits);
          } 
          else {
            value = parseInt(word, 10);
            type = value > 0xFFFFFFFF ? "u64" : value > 0xFFFF ? "u32" : value > 0xFF ? "u16" : "u8";
          }

          const md = new vscode.MarkdownString();
          md.appendMarkdown(`(${type}) ${value}\n\n`);
          md.appendMarkdown(`(${type}) 0x${value.toString(16).toUpperCase()}\n\n`);
          md.appendMarkdown(`(${type}) 0b${value.toString(2)}\n`);
          return new vscode.Hover(md);
        }

        if (docs[word]) return new vscode.Hover(new vscode.MarkdownString(docs[word]));
        if (keywords.includes(word)) return new vscode.Hover(new vscode.MarkdownString(`**${word}**`));
        return;
      }
    }
  );

  context.subscriptions.push(hoverProvider);

  const serverModule = context.asAbsolutePath(path.join("out", "server.js"));
  const serverOptions: ServerOptions = {
    run:   { module: serverModule, transport: TransportKind.ipc },
    debug: { module: serverModule, transport: TransportKind.ipc }
  };

  const clientOptions: LanguageClientOptions = {
    documentSelector: [{ scheme: "file", language: "cpl" }],
    synchronize: {
      fileEvents: vscode.workspace.createFileSystemWatcher("**/*.cpl")
    }
  };

  client = new LanguageClient("cplLS", "CPL Language Server", serverOptions, clientOptions);
  context.subscriptions.push(client);
  void client.start();
}

export function deactivate() {
  return client?.stop();
}
