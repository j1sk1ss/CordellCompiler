import * as path from "path";
import * as fs from "fs";
import * as vscode from "vscode";
import { LanguageClient, LanguageClientOptions, ServerOptions, TransportKind } from "vscode-languageclient/node";
import {
  defaultSysTypeForHost,
  expandMakeValue,
  inferSysTypeFromCompilerArgs,
  parseMakefileVarsText,
  sysTypeToPredefinedMacro,
  CplSysType
} from "./cplTarget";

let client: LanguageClient | undefined;

function findMakefileUpwards(startPath?: string): string | undefined {
  if (!startPath) return undefined;
  let dir = fs.existsSync(startPath) && fs.statSync(startPath).isDirectory()
    ? startPath
    : path.dirname(startPath);

  while (true) {
    const candidate = path.join(dir, "Makefile");
    if (fs.existsSync(candidate)) return candidate;

    const parent = path.dirname(dir);
    if (parent === dir) return undefined;
    dir = parent;
  }
}

function parseMakefileVars(makefilePath: string): Map<string, string> {
  try {
    const text = fs.readFileSync(makefilePath, "utf8");
    return parseMakefileVarsText(text);
  } catch {}

  return new Map<string, string>();
}

function findTestsDirUpwards(startPath?: string): string | undefined {
  if (!startPath) return undefined;
  let dir = fs.existsSync(startPath) && fs.statSync(startPath).isDirectory()
    ? startPath
    : path.dirname(startPath);

  while (true) {
    const candidate = path.join(dir, "tests", "module_testing.py");
    if (fs.existsSync(candidate)) return path.join(dir, "tests");

    const parent = path.dirname(dir);
    if (parent === dir) return undefined;
    dir = parent;
  }
}

async function runModuleTest(resource?: vscode.Uri): Promise<void> {
  const uri = resource ?? vscode.window.activeTextEditor?.document.uri;
  if (!uri || uri.scheme !== "file") {
    void vscode.window.showErrorMessage("Open a CPL test file before running module tests.");
    return;
  }

  if (path.extname(uri.fsPath) !== ".cpl") {
    void vscode.window.showErrorMessage("CPL module tests can be run only for .cpl files.");
    return;
  }

  const testsDir = findTestsDirUpwards(uri.fsPath);
  if (!testsDir) {
    void vscode.window.showErrorMessage("Could not find tests/module_testing.py above the selected file.");
    return;
  }

  const workspaceFolder = vscode.workspace.getWorkspaceFolder(uri);
  const execution = new vscode.ShellExecution(
    "python3",
    ["module_testing.py", "--path", uri.fsPath],
    { cwd: testsDir }
  );

  const task = new vscode.Task(
    { type: "cpl", command: "runModuleTest", file: uri.fsPath },
    workspaceFolder ?? vscode.TaskScope.Workspace,
    "Run CPL Module Test",
    "CPL",
    execution,
    []
  );
  task.group = vscode.TaskGroup.Test;
  task.presentationOptions = {
    reveal: vscode.TaskRevealKind.Always,
    panel: vscode.TaskPanelKind.Dedicated,
    clear: true
  };

  await vscode.tasks.executeTask(task);
}

function inferSysTypeForDocument(document: vscode.TextDocument): CplSysType {
  const makefilePath = findMakefileUpwards(document.uri.fsPath);
  if (makefilePath) {
    const vars = parseMakefileVars(makefilePath);
    const runArgsRaw = vars.get("RUN_ARGS");
    if (runArgsRaw) {
      const inferred = inferSysTypeFromCompilerArgs(expandMakeValue(runArgsRaw, vars));
      if (inferred) return inferred;
    }
  }

  return defaultSysTypeForHost(process.platform);
}

function inactivePreprocessorRanges(document: vscode.TextDocument): vscode.Range[] {
  const targetMacro = sysTypeToPredefinedMacro(inferSysTypeForDocument(document));
  const defined = new Set<string>();
  if (targetMacro) defined.add(targetMacro.name);

  const ranges: vscode.Range[] = [];
  const activeStack: boolean[] = [];
  let inactiveDepth = 0;
  let inactiveStart: vscode.Position | undefined;

  const isActive = () => activeStack.every(Boolean);

  for (let lineNo = 0; lineNo < document.lineCount; lineNo++) {
    const line = document.lineAt(lineNo);
    const text = line.text;
    const m = text.match(/^\s*#\s*(ifdef|ifndef|endif|define|undef)\b\s*([A-Za-z_]\w*)?/);
    if (!m) continue;

    const directive = m[1];
    const name = m[2];

    if (directive === "ifdef" || directive === "ifndef") {
      const parentActive = isActive();
      const cond = !!name && (directive === "ifdef" ? defined.has(name) : !defined.has(name));
      const branchActive = parentActive && cond;
      activeStack.push(branchActive);

      if (!branchActive) {
        if (inactiveDepth === 0) inactiveStart = new vscode.Position(lineNo, 0);
        inactiveDepth++;
      }

      continue;
    }

    if (directive === "endif") {
      const wasActive = activeStack.pop();
      if (wasActive === false) {
        inactiveDepth = Math.max(0, inactiveDepth - 1);
        if (inactiveDepth === 0 && inactiveStart) {
          ranges.push(new vscode.Range(inactiveStart, line.rangeIncludingLineBreak.end));
          inactiveStart = undefined;
        }
      }
      continue;
    }

    if (!isActive() || !name) continue;
    if (directive === "define") {
      if (!defined.has(name)) defined.add(name);
    } else if (directive === "undef") {
      defined.delete(name);
    }
  }

  if (inactiveDepth > 0 && inactiveStart) {
    const lastLine = document.lineAt(Math.max(0, document.lineCount - 1));
    ranges.push(new vscode.Range(inactiveStart, lastLine.range.end));
  }

  return ranges;
}

export function activate(context: vscode.ExtensionContext) {
  const inactiveBranchDecoration = vscode.window.createTextEditorDecorationType({
    opacity: "0.45"
  });

  const updateInactiveBranches = (editor?: vscode.TextEditor) => {
    if (!editor || editor.document.languageId !== "cpl") return;
    editor.setDecorations(inactiveBranchDecoration, inactivePreprocessorRanges(editor.document));
  };

  const updateVisibleInactiveBranches = () => {
    for (const editor of vscode.window.visibleTextEditors) updateInactiveBranches(editor);
  };

  context.subscriptions.push(
    inactiveBranchDecoration,
    vscode.commands.registerCommand("cpl.runModuleTest", runModuleTest),
    vscode.window.onDidChangeActiveTextEditor((editor) => updateInactiveBranches(editor)),
    vscode.window.onDidChangeVisibleTextEditors(() => updateVisibleInactiveBranches()),
    vscode.workspace.onDidChangeTextDocument((event) => {
      for (const editor of vscode.window.visibleTextEditors) {
        if (editor.document === event.document) updateInactiveBranches(editor);
      }
    })
  );
  updateVisibleInactiveBranches();

  const keywords = [
    "start","exit","function","container","return",
    "if","else","while","loop","switch","case","default",
    "glob","ro","dref","ref","ptr","lis","break","extern","from","import","syscall","asm","as",
    "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","not","neg","poparg","sizeof","section","align"
  ];

  const integerTypeDoc = (name: string, bits: number, signed: boolean) => {
    const bytes = bits / 8;
    const range = signed
      ? `from -2^${bits - 1} to 2^${bits - 1} - 1`
      : `from 0 to 2^${bits} - 1`;

    return `**${name}** - ${signed ? "signed" : "unsigned"} ${bits}-bit integer.

**Memory model**

- Occupies exactly ${bytes} byte${bytes === 1 ? "" : "s"}.
- Value range: ${range}.
- The signed and unsigned types use the same number of bits. They differ in how those bits are interpreted.

\`\`\`cpl
${name} value = 10;
u64 bytes = sizeof(${name}); : ${bytes} :
\`\`\`

**System-level note**

A cast to a narrower integer can discard high bits. A cast between signed and unsigned types preserves the bit pattern only when the compiler's conversion rules permit it, but changes its numerical interpretation. Do not use a small integer type for an address. Use \`ptr T\` instead.`;
  };

  const floatTypeDoc = (name: string, bits: number) => `**${name}** - ${bits}-bit floating-point value.

**Memory model**

- Occupies ${bits / 8} bytes.
- Stores an approximation of a real number, not an exact decimal fraction.
- Very large and very small values, infinities and NaN may be representable depending on the target backend.

\`\`\`cpl
${name} x = 0.1;
${name} y = 0.2;
${name} sum = x + y;
\`\`\`

**Typical pitfall**

Do not compare results of non-trivial floating-point calculations as if every decimal value were exact. Integer types are preferable for counters, sizes, offsets and bit masks.`;

  const docs: Record<string, string> = {
    // Entry points and functions
    start: `**start** - optional program entry point.

The runtime transfers control to \`start\` after the executable has been loaded. Local variables declared inside it normally have automatic lifetime and disappear when the function finishes.

\`\`\`cpl
start(i64 argc, ptr u64 argv) {
  : argc - number of command-line arguments :
  : argv - pointer to an array of argument pointers :
  exit 0;
}
\`\`\`

**System-level view**

The exact initial register and stack state is target-specific. The compiler or runtime converts that platform entry state into the declared CPL parameters. Returning from \`start\` and executing \`exit\` are not necessarily the same operation. \`exit\` explicitly terminates the process.`,

    function: `**function** - declares or defines executable code.

A function has a name, parameters, an optional return type and, for a definition, a body. Parameters are local names whose values are supplied by the caller.

\`\`\`cpl
function abs(i32 x) -> i32 {
  if x < 0; { return x * -1; }
  return x;
}
\`\`\`

Container-associated functions use a qualified name:

\`\`\`cpl
function node::new(i32 value) -> node {
  node result;
  result.value = value;
  return result;
}
\`\`\`

**Call model**

At machine level, a call transfers control to another address and follows a calling convention. The convention defines where arguments and the result are placed, which registers must be preserved and who restores the stack. Declarations used across compilation units must agree exactly on parameter and return types.

**Typical pitfalls**

- Returning a pointer to a local variable creates a dangling pointer after the function ends.
- A declaration and definition with different signatures describe incompatible ABIs.
- Recursive functions consume stack space for every active call.`,

    container: `**container** - a struct-like aggregate containing fields and methods.

Each instance owns storage for its fields. Methods do not occupy space inside each instance. Their machine code is stored separately.

\`\`\`cpl
container storage {
  u32 wood;
  u64 money;

  @[self]
  function sell_wood() -> i0 {
    self.wood -= 100;
    self.money += 100;
  }

  glob function load(ptr u8 source) -> storage;
}

function storage::load(ptr u8 source) -> storage {
  storage result;
  return result;
}
\`\`\`

Use \`.\` for an instance operation and \`::\` for an associated function name.

**Memory layout**

A container's size depends on its fields, their order, alignment and layout annotations. Padding bytes can be inserted between fields or at the end. Therefore the size is not always the sum of field sizes.

- \`@[like_c]\` requests C-like field alignment.
- \`@[union]\` overlays fields at the same starting address, so the size is based on the largest field.
- \`@[align(N)]\` changes the required alignment.

Use \`sizeof(ContainerName)\` to inspect the computed size for the selected target.

**Typical pitfalls**

- Reordering fields can change the binary layout and break file, network or FFI compatibility.
- A pointer to a container is only an address. It does not copy the object.
- A container copied by value may copy padding and every field.`,

    return: `**return** - finishes the current function and optionally supplies a result to its caller.

\`\`\`cpl
function id(i32 x) -> i32 {
  return x;
}

function noop() -> i0 {
  return;
}
\`\`\`

**System-level view**

The result is placed according to the target calling convention, often in a register for small scalar values. Larger containers may be returned through hidden memory supplied by the caller.

A returned pointer must remain valid after the function ends. Pointers to local variables generally do not.`,

    // Control flow
    if: `**if** - executes a block only when its condition is non-zero.

Syntax: \`if <expression>; <block> [else <block>]\`

\`\`\`cpl
if count == 0; {
  exit 1;
} else {
  count -= 1;
}
\`\`\`

The condition is evaluated before the branch is selected. At machine level this commonly becomes a comparison followed by a conditional jump.

**Typical pitfall**

A pointer value can be used as a condition, but that only checks whether the address is zero. It does not prove that the pointed memory is valid.`,

    else: `**else** - alternative branch of an \`if\` statement.

It runs only when the preceding \`if\` condition is zero.

\`\`\`cpl
if ptr_value == 0; {
  exit 1;
} else {
  dref ptr_value = 42;
}
\`\`\`

Keep the null check and dereference in a control-flow relation the reader can see. Low-level code becomes dangerous when pointer validity is only an undocumented assumption.`,

    while: `**while** - repeats a block while its condition remains non-zero.

Syntax: \`while <expression>; <block>\`

\`\`\`cpl
i32 i = 0;
while i < 10; {
  i += 1;
}
\`\`\`

The condition is checked before every iteration. The body may therefore execute zero times.

**System-level note**

A loop over memory should maintain an explicit invariant: current pointer, remaining element count and valid bounds. Pointer arithmetic without a bound can silently walk into unrelated memory.`,

    loop: `**loop** - unconditional repetition.

\`\`\`cpl
loop {
  lis "tick";
  break;
}
\`\`\`

This is useful for event loops, kernels and retry logic. It requires an explicit \`break\`, \`return\`, \`exit\` or another control transfer to terminate.

An infinite busy loop continuously consumes a processor core unless it performs a blocking operation or waits using platform-specific instructions.`,

    switch: `**switch** - selects one branch by comparing a value with \`case\` labels.

\`\`\`cpl
switch(opcode) {
  case 0; { result = 10; }
  case 1; { result = 20; }
  default { result = 0; }
}
\`\`\`

A compiler may lower a dense set of integer cases to a jump table and a sparse set to comparisons. A jump table trades memory for faster branch selection.

Use \`default\` when values outside the known cases are possible, especially for data read from files, devices or the network.`,

    case: `**case** - labels a value-specific branch inside \`switch\`.

\`\`\`cpl
switch(code) {
  case 10; { return 1; }
  case 20; { return 2; }
}
\`\`\`

Case values should be compile-time constants. Keep them unique. Whether execution can fall through to the next case is language-specific, so use explicit blocks and control flow rather than relying on C habits.`,

    default: `**default** - fallback branch of a \`switch\`.

\`\`\`cpl
switch(code) {
  case 0; { return 0; }
  default { return -1; }
}
\`\`\`

In systems code, a default branch is often the place to reject unsupported opcodes, enum values or protocol versions. Silently accepting an unknown value can hide corrupted or hostile input.`,

    break: `**break** - exits the nearest active \`while\`, \`loop\` or \`switch\`.

\`\`\`cpl
while index < count; {
  if data[index] == 0; { break; }
  index += 1;
}
\`\`\`

\`break\` changes control flow only. It does not automatically free manually managed memory or close operating-system resources.`,

    // Storage and linkage
    glob: `**glob** - gives a variable or function global storage/linkage.

\`\`\`cpl
glob i32 counter = 0;
glob function checksum(ptr u8 data, u64 size) -> u32;
\`\`\`

Inside a container, a global function declaration can describe a separately implemented associated function:

\`\`\`cpl
container image {
  glob function load(ptr u8 path) -> image;
}

function image::load(ptr u8 path) -> image {
  image result;
  return result;
}
\`\`\`

**Lifetime and linking**

A global variable exists for the lifetime of the program rather than for one function call. A globally visible symbol can be resolved by the linker from another object file or library.

**Typical pitfalls**

- Mutable global state creates hidden dependencies and is unsafe under concurrency without synchronization.
- Defining the same global symbol in multiple linked units can cause a duplicate-symbol error.
- A global declaration without a matching definition remains an unresolved linker symbol.`,

    ro: `**ro** - read-only storage.

\`\`\`cpl
ro u32 sector_size = 512;
ro str greeting = "Hello";
\`\`\`

The compiler rejects ordinary writes through the read-only name. Depending on the backend and linkage, global read-only data may be placed in a non-writable executable section.

**Important limitation**

Read-only binding and deep immutability are different. A read-only pointer cannot necessarily modify the pointer variable, but the memory reached through that pointer may still be writable unless the type system expresses otherwise.`,

    extern: `**extern** - declares a symbol whose implementation or storage is provided outside the current CPL unit.

\`\`\`cpl
@[abi]
extern function printf(ptr i8 format, ...);

extern ptr u8 frame_buffer;
\`\`\`

The declaration lets the compiler type-check uses and emit a symbol reference. The linker later connects that reference to an object file, static library or shared library.

**ABI requirements**

The CPL declaration must match the external implementation in parameter types, return type, symbol name, calling convention and data layout. A mismatch can compile successfully and still corrupt registers, the stack or memory at runtime.

For C++ implementations, export a stable C symbol with \`extern "C"\` unless CPL deliberately supports C++ name mangling.`,

    // Modules and preprocessing
    from: `**from** - names a source or module from which declarations are imported.

\`\`\`cpl
from "math.cpl" import add, sub
\`\`\`

This is a frontend-level dependency. It is different from \`extern\`: importing makes CPL declarations available, while \`extern\` describes a symbol that must be resolved during linking. Both may be needed when an interface file describes a native library.`,

    import: `**import** - selects names from a \`from\` source.

\`\`\`cpl
from "math.cpl" import add, sub
\`\`\`

Import only the interface needed by the current unit. Explicit imports make symbol ownership and dependencies easier to understand than relying on hidden global declarations.`,

    section: `**section** - places top-level declarations into a named binary section.

\`\`\`cpl
section(".text.fast") {
  function fast_path() -> i32 { return 0; }
}
\`\`\`

Object files are divided into sections such as executable code, read-only data, writable data and zero-initialized storage. A linker script decides where these sections appear in the final address space.

**Typical uses**

- Boot code that must appear at a fixed address.
- Interrupt tables or firmware metadata.
- Separating hot, cold or read-only data.

Section names and their meaning are target- and linker-specific. A wrong placement can make data non-writable, code non-executable or remove it during linking.`,

    align: `**align** - requests alignment for a declaration or declaration block.

Single declaration:

\`\`\`cpl
align(16) i32 value = 0;
\`\`\`

Declaration block:

\`\`\`cpl
align(32) {
  i32 a = 0;
  ptr i8 p = 0;
}
\`\`\`

**What alignment means**

An address aligned to \`N\` is divisible by \`N\`. CPUs and ABIs often require or prefer naturally aligned values. Extra alignment can improve SIMD or device access, but may increase padding and total memory usage.

Alignment is not a size. A 4-byte value aligned to 64 bytes still stores 4 bytes, but its start address obeys the stronger boundary.`,

    // Low-level operations
    syscall: `**syscall** - directly requests a service from the operating-system kernel.

\`\`\`cpl
syscall(0x2000004, 1, ref msg, length);
\`\`\`

**System-level view**

A system call crosses from user mode into the kernel. Its number, arguments, register convention, pointer validity and return/error convention are platform-specific.

**Use with care**

- The same number can mean different operations on Linux, macOS and other systems.
- Every pointer passed to the kernel must reference a valid buffer for the required direction and length.
- A negative or special return value may represent an error rather than useful data.
- Prefer a stable library wrapper when portability matters.`,

    asm: `**asm** - embeds target-specific assembly instructions.

\`\`\`cpl
u64 output = 0;
asm(output) {
  "xor rax, rax",
  "mov rax, 1",
  "mov %0, rax"
}
\`\`\`

Inline assembly bypasses many compiler guarantees. The author must respect the processor ISA, calling convention, register usage, stack alignment and the compiler's operand contract.

**Typical pitfalls**

- Modifying a register the compiler assumes is preserved.
- Reading or writing memory without telling the compiler.
- Depending on one architecture or assembler syntax.
- Forgetting that an optimizer may move surrounding code unless dependencies are represented.

Use ordinary CPL for control flow and arithmetic unless assembly is required for an instruction, ABI boundary or measured performance reason.`,

    lis: `**lis** - debug breakpoint marker or trace hook.

\`\`\`cpl
lis "before device write";
\`\`\`

Its exact generated behavior depends on the compiler/runtime. Treat it as a diagnostic aid, not as program logic. Production behavior should not depend on a debugger being attached.`,

    exit: `**exit** - terminates the current process with an exit status.

\`\`\`cpl
if initialization_failed; {
  exit 1;
}
exit 0;
\`\`\`

By convention, status 0 means success and a non-zero value means failure, although the operating system only preserves a platform-specific portion of the integer.

Immediate process termination may bypass normal function returns and cleanup logic. Flush or close important resources explicitly when required by the runtime.`,

    // Addressing, operators and casts
    ref: `**ref** - obtains the address of an object.

\`\`\`cpl
i32 value = 10;
ptr i32 address = ref value;
dref address = 11;
\`\`\`

**What actually happens**

\`value\` stores the integer itself. \`ref value\` produces the memory address at which that integer is stored. The pointer contains an address, not a second copy of the integer.

\`\`\`text
value storage:   [ 10 ]
                  ^
address ---------+
\`\`\`

**Lifetime rule**

The pointer is valid only while the referenced object still exists and remains at that address. Returning \`ref local_variable\` from a function is generally invalid because the local storage disappears when the function returns.`,

    dref: `**dref** - accesses the object stored at an address.

\`\`\`cpl
i32 value = 10;
ptr i32 address = ref value;

i32 copy = dref address; : load 10 from memory :
dref address = 20;       : store 20 into memory :
\`\`\`

A dereference performs a memory access using the pointer as the address and its target type as the access size and interpretation.

**Safety requirements**

Before dereferencing, the pointer must be non-zero, correctly aligned, point to a live object, allow the requested read or write and cover at least \`sizeof(target_type)\` bytes. A successful null check alone does not prove these conditions.`,

    ptr: `**ptr** - constructs a pointer type.

\`ptr T\` stores the address of a value whose memory is interpreted as type \`T\`.

\`\`\`cpl
i32 value = 42;
ptr i32 p = ref value;
i32 loaded = dref p;
\`\`\`

**Pointer size**

The pointer itself normally occupies one machine word, regardless of the size of \`T\`:

\`\`\`cpl
sizeof(ptr u8);  : 8 on a 64-bit target, 4 on i386 :
sizeof(ptr i64); : same pointer size :
\`\`\`

The target type controls dereference and indexing. It does not allocate memory and does not record the buffer length.

**Null pointer**

A zero pointer represents no object. It may be compared or stored, but dereferencing it is invalid.

\`\`\`cpl
ptr i32 p = 0;
if p == 0; { exit 1; }
\`\`\`

**Pointer arithmetic and indexing**

Indexing conceptually advances by the target type's size. For \`ptr i32 p\`, \`p[1]\` addresses the next \`i32\`, not the next byte. A pointer does not know how many elements are valid, so the programmer must carry a separate count.

**Typed and untyped pointers**

\`ptr i0\` can represent an untyped address at ABI boundaries. Cast it to the correct pointer type before dereferencing. The cast changes the compiler's interpretation of the same address. It does not validate or transform the memory.

**Common invalid states**

- Null pointer.
- Dangling pointer to an object whose lifetime ended.
- Out-of-bounds pointer.
- Misaligned pointer.
- Pointer with the wrong target type.
- Pointer to read-only memory used for a write.`,

    not: `**not** - logical negation.

It converts zero to 1 and a non-zero value to 0.

\`\`\`cpl
i32 a = not 0;  : 1 :
i32 b = not 25; : 0 :
\`\`\`

This is a logical operation, not a bit-by-bit inversion. Use \`neg\` for bit inversion.`,

    neg: `**neg** - bitwise inversion.

Every bit in the operand is flipped.

\`\`\`cpl
u8 mask = 0b00001111;
u8 inverse = neg mask; : 0b11110000 :
\`\`\`

The result depends on the operand width. Inverting an \`u8\` flips 8 bits, while inverting a \`u64\` flips 64 bits. Prefer unsigned types for masks because their bit interpretation is clearer.`,

    poparg: `**poparg** - retrieves a variadic argument according to the CPL calling convention.

\`\`\`cpl
function inspect(...) -> i0 {
  ptr u8 first = poparg as ptr u8;
}
\`\`\`

Variadic arguments carry less type information than normal parameters. The function must know the expected order and types from another argument, a format string or an external protocol.

**ABI warning**

Different types can be passed in different registers or promoted before a call. Reading a variadic argument using the wrong type can consume the wrong bytes or corrupt interpretation of later arguments.`,

    as: `**as** - explicitly converts or reinterprets an expression as another type.

\`\`\`cpl
u64 wide = 255 as u64;
ptr u8 bytes = (ref wide) as ptr u8;
\`\`\`

**Numeric casts**

A wider type can represent more values. A narrower type can discard information. Signedness changes how the same high bit is interpreted.

**Pointer casts**

Casting \`ptr A\` to \`ptr B\` keeps the address and changes the type used for future memory accesses. It does not resize, relocate or validate the object.

\`\`\`cpl
u32 word = 0x11223344;
ptr u8 first_byte = (ref word) as ptr u8;
\`\`\`

The byte observed through \`first_byte\` depends on target endianness. Pointer casts also require correct alignment and sufficient storage for the target type.`,

    sizeof: `**sizeof** - computes the storage size of a type or expression at compile time.

\`\`\`cpl
u64 a = sizeof(i32);       : 4 :
u64 b = sizeof(ptr u8);    : pointer size :
u64 c = sizeof arr[16, u8];: 16 :
u64 d = sizeof(value);    : size of value's type :
\`\`\`

The expression form asks for the expression's type size. It should not read the object or follow a pointer.

**Containers and layout**

For containers, the result includes padding required by the selected layout and annotations. It may therefore exceed the sum of field sizes.

\`\`\`cpl
@[like_c]
container packet {
  u8 tag;
  u32 length;
}

u64 packet_size = sizeof(packet);
\`\`\`

**Important distinction**

\`sizeof(ptr packet)\` is the size of one address. \`sizeof(packet)\` is the size of the complete object. \`sizeof(str)\` describes the string value representation, not the number of characters in the pointed string.`,

    // Primitive and aggregate types
    f64: floatTypeDoc("f64", 64),
    f32: floatTypeDoc("f32", 32),
    i64: integerTypeDoc("i64", 64, true),
    i32: integerTypeDoc("i32", 32, true),
    i16: integerTypeDoc("i16", 16, true),
    i8: integerTypeDoc("i8", 8, true),
    u64: integerTypeDoc("u64", 64, false),
    u32: integerTypeDoc("u32", 32, false),
    u16: integerTypeDoc("u16", 16, false),
    u8: integerTypeDoc("u8", 8, false),

    i0: `**i0** - no-value type, analogous to \`void\`.

Use it as the return type of a function that performs an action but does not produce a value.

\`\`\`cpl
function clear(ptr u8 data, u64 size) -> i0 {
  return;
}
\`\`\`

\`ptr i0\` is different. It is a pointer-sized address with no specific target object type. It is useful at ABI boundaries, but it must be cast to a correctly typed pointer before a meaningful dereference.`,

    str: `**str** - string value.

\`\`\`cpl
str message = "Hello";
\`\`\`

In the language server's type model, \`str\` is pointer-sized. The characters occupy separate storage, so \`sizeof(str)\` does not return the string length.

**System-level questions to keep explicit**

- Is the character data mutable or read-only?
- Is it zero-terminated or accompanied by a length?
- Who owns the storage and how long does it remain valid?
- Which encoding is used?

When calling C APIs, many functions expect \`ptr i8\` to zero-terminated bytes. A CPL \`str\` should only be passed when its runtime representation satisfies that ABI.`,

    arr: `**arr** - fixed-size contiguous array.

Declaration form:

\`\`\`cpl
arr buffer[16, u8] = {1, 2, 3};
\`\`\`

Type form:

\`\`\`cpl
arr[16, u8] buffer;
\`\`\`

**Memory layout**

The elements are stored consecutively. For an array of \`N\` elements of type \`T\`, the basic storage size is \`N * sizeof(T)\`.

\`\`\`text
buffer[0] buffer[1] buffer[2] ... buffer[N-1]
\`\`\`

Array indexing uses an element index, not a byte offset. The valid range is 0 through \`N - 1\`.

**Array versus pointer**

An array owns storage for all elements. A pointer stores only an address. Passing an array to low-level code may produce a pointer to its first element, but the pointer no longer carries the array length. Keep the count as a separate value.`,
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
