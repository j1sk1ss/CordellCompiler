# Functions and inbuilt macros
## Functions
Functions are defined with the `function` keyword. If you want to use a function from another `.cpl` file (or from another language that supports the `extern` mechanism), mark it with `glob`.

One important detail: CPL renames local functions internally using a pattern such as `__cpl_{name}{id}`. If you want to preserve the symbol name for external use, prefer `glob`.

```cpl
function min(i32 a) -> i0 { return; }
glob function chloe(i32 a = 10) -> u64 { return a + 10; }
function max(u64 a = chloe(11)) -> i32 { return a + 10; }
```

Name transformation example:
```cpl
function foo();      : __cpl_foo0 :
glob function foo(); : foo        :
```

**Note:** Global functions do not support overloading or scoping.

## Prototypes
A function may have a prototype, similarly to C. A prototype is a declaration without a body.

```cpl
function chloe(i32 a = 10) -> u64;

function max() -> i32 {
    return chloe();
}

function chloe(i32 a = 10) -> u64 {
    return a + 10;
}
```

## Default arguments
CPL supports default function arguments. If you omit trailing arguments, the compiler inserts the defaults.

```cpl
chloe();              : chloe(10); :
max();                : max(chloe(11)); :
min(max() & chloe()); : min(max(chloe(11)) & chloe(10)); :
```

Restrictions:
- A default argument cannot appear before a non-default one:
```cpl
function foo(i32 a = 1, i32 b); : <= Forbidden :
```
- Defaults must be duplicated in both the prototype and the definition.

## Function overloading
CPL supports function overloading, but with several restrictions that come from the compiler architecture.

Rules:
- **No return-type overloading** — CPL does not distinguish overloads by return type.
- **Local-only overloading** — overloaded functions are neither `glob` nor `extern`.
- **Same scope** - function will search overload candidates only in the same scope with the original one.

This works:
```cpl
function foo(i32 a);
function foo();
function foo(i8 a);
```

To make overload resolution explicit, use the `as` keyword:
```cpl
foo(10 as i32); : function foo(i32 a); :
: i64 a; :
foo(a as i8);   : function foo(i8 a);  :
```

**Important:** the same type-system rule applies here as everywhere else in CPL: **widening may be implicit, but narrowing is allowed only with `as`.** In practice, `as` is also the safest way to select the exact overload you want.

These overload sets are invalid:
```cpl
function foo() -> i32;
function foo() -> i0;
```

```cpl
function foo(i32 a, i32 b = 1, i32 c = 1);
function foo(i32 a, i32 b = 1);
```

## Function pointers
A function can be stored as a pointer:

```cpl
function foo(i32 a) -> i32;
start() {
    ptr i0 a = foo;
    i32 res = a(100);
}
```

A function pointer has no signature information. Because of that, static analysis, overload resolution, and default arguments no longer apply.

```cpl
function foo(i32 a) -> i32;
function foo(u32 a = 10) -> i0;
function bar(i8 a = 'a');

start() {
    ptr i0 a = foo; : Will store the first matching function symbol :
    ptr i0 b = bar;
    b();            : Undefined behavior: no signature, no defaults checked :
}
```

That flexibility can still be useful when several functions share the same effective calling pattern:

```cpl
function min(i32 a, i32 b);
function max(i32 a, i32 b);

function logic(ptr i0 func, i32 a, i32 b) {
    func(a, b);
}

start() {
    logic(min, 10, 20);
    logic(max, 10, 20);
}
```

A function pointer does not even have to originate from a function symbol. Any `ptr i0` can be called:

```cpl
((0x100 + 0xB045) as ptr i0)(100); : <- Calls code at 0xB145 if valid :
ptr i0 a = 123;
a(100 + 123);                      : <- Calls whatever is located there :
```

## Local functions
A function may define another function inside its body:

```cpl
function foo() {
    function bar() {
    }
}
```

Local functions do not capture the parent environment, but they can be returned as pointers:

```cpl
function foo() -> ptr i0 {
    function getter() {
        return 10;
    }
    return getter;
}
```

Because local functions have their own scope, local and global functions may reuse the same name:

```cpl
function bar() {
    function foo() {
    }
    foo();
}

function foo();

start() {
    foo();
}
```

This is mostly syntax sugar. The same functions could be defined at top level, though then you would need globally unique names.

## Lambda functions
Lambda functions in CPL are based on local functions. They do **not** capture outer local variables, so they serve as lightweight logic containers rather than full closures.

```cpl
@[entry]
function main() {
    function local(i32 a) { return a + 10; }
    ptr i0 lambda = (i32 a) => a + 10;
    : local(10) == lambda(10) :
}
```

The same idea is useful when passing logic to another function:

```cpl
function foo(ptr i0 logic) {
    return logic(10, 10);
}

start() {
    foo((i32 a, i32 b) => { a += b; a + b; });
}
```

Equivalent form with a named local function:

```cpl
function foo(ptr i0 logic) {
    return logic(10, 10);
}

start() {
    function local(i32 a, i32 b) {
        a += b;
        return a + b;
    }
    foo(local);
}
```

The main advantage of lambdas is the lack of naming overhead.

## Variadic arguments
CPL supports variadic arguments in a C-like way. To define one, place `...` as the final argument:

```cpl
function foo(...) -> i0;
```

Use the `poparg` annotation to extract arguments:

```cpl
function foo(...) -> i0 {
    @[poparg] i8 a1;
    @[poparg] i8 a2;
}
```

The same annotation can be used in a non-variadic function body as well:

```cpl
function foo(i32 a, i32 b) -> i0 {
    @[poparg] i8 a1; : a :
    @[poparg] i8 b1; : b :
}
```

## Calling convention (system information)
Calling convention details depend on the target architecture. For example, `x86_64_gnu_nasm` follows the C calling convention. The same idea applies to `x86_32_gnu_nasm` and `x86_16_gnu_nasm` once those targets are fully ready.

Function results are returned in the target return register (for example `rax`, `ax`, or `al`), and the function preserves registers that it uses when required by the target rules.

```cpl
function foo() {
    mov r10, 10
}

push r10 : <= Save r10 :
foo();
i32 res = rax;
pop r10
```

## Built-in macros
CPL provides two especially useful built-in mechanisms for low-level programming: `syscall` and `asm`.

### `syscall`
`syscall` is invoked like a regular function call, but it can accept a variable number of arguments depending on the platform ABI.

```cpl
str msg = "Hello, World!";
syscall(1, 1, ref msg, strlen(ref msg));
```

### `asm`
`asm` allows inline assembly. You may pass any number of CPL values into the argument list and then reference them in the assembly block via `%<num>` placeholders.

```cpl
i32 a = 0;
i32 ret;
asm(a, ret) {
    "push rax",
    "mov rax, %0", : mov rax, a   :
    "syscall",
    "mov %1, rax", : mov ret, rax :
    "pop rax"
}
```

**Note:** inline assembly is not optimized by the compiler.

## How inline assembly works (system information)
Inline assembly is powerful, but also dangerous. The compiler cannot safely reason about everything that happens inside the block, so you should preserve the registers you modify and make sure the assembly matches the selected target architecture.

```cpl
asm() {
    "push rax",
    "mov rax, 10",
    "pop rax"
}
```

The block is copied almost directly into the final output after placeholder substitution. Because of that:
- write assembly in the syntax required by the target backend,
- avoid labels and jumps when possible,
- prefer `--no-optimizations` if the block depends on non-local control flow assumptions.

Example of a fragile pattern:
```cpl
asm() {
    "jmp label1"
}

i32 a; : <= Dead code from the compiler point of view :

asm() {
    "label1:"
}
```
