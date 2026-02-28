# Summary
The **Cordell Programming Language (CPL)** is a system-level programming language designed for learning and experimenting with modern compiler concepts. It combines low-level capabilities from `ASM` with practices inspired by modern languages like `Rust` and `C`. `CPL` is intended for:
- **Educational purpose** — a language to study compiler design, interpreters, and programming language concepts.
- **Radical simplicity** - let's make this language simple as it possible. 
- **System programming** — OS (bootloaders, kernels), DBMSs, FSs, compilers, interpreters, etc.  
- **Experiments** - a compiler to test some techiques in real compilation without any overcomplexity.

## Key Features
- **Statically-typed**: variables can't hold values of different types; the compiler attempts implicit conversions when assigning.
- **Minimalistic syntax**: cpl is a simple programming language that is beign based on C language.  
- **Deterministic control flow**: no hidden behaviors; all execution paths are explicit.  
- **Extensibility**: functions and inbuilt macros allow both low-level operations and high-level abstractions.
- **Optimization**: input code can be optimized by techniques such as constant propagation, linear invariant code motion, peephole optimization, function inline, tail recursion elimination.
- **Function overloading**: compiler supports overloaded functions.
- **Default-args**: compiler supports default values in function arguments.
- **Headers-Modules**: cpl supports headers as it does C/++ language and module system as it does Go/Python etc.
- **Strings**: lagnuage supports and distinguishs strings from raw pointers.
- **Local functions**: Functions can define local function as it do functions in Rust.
- **Assembly**: Compiler generates an assembly file for the further compilation.

# Main idea of this project
Main goal of this project is learning of compilers architecture and porting one to CordellOS project (I want to code apps for my own OS inside my own OS). Also, according to my bias to assembly and C languages (I just love them), this language will stay "low-level" as it possible, but some features can be added in future with strings (inbuild concat, comparison and etc).

# Used links and literature
- Aarne Ranta. *Implementing Programming Languages. An Introduction to Compilers and Interpreters*
- Aho, Lam, Sethi, Ullman. *Compilers: Principles, Techniques, and Tools (Dragon Book)*
- Andrew W. Appel. *Modern Compiler Implementation in C (Tiger Book)*
- Cytron et al. *Efficiently Computing Static Single Assignment Form and the Control Dependence Graph* (1991)
- Daniel Kusswurm. *Modern x86 Assembly Language Programming. Covers x86 64-bit, AVX, AVX2 and AVX-512. Third Edition*

# Hello, World! example
That's how we can write a 'hello-world' program with CPL language for x86-64 GNU architecture.
```cpl
{
    : Define the strlen function
      that accepts a pointer to a char array :
    function strlen(ptr i8 s) -> i64 {
        i64 l = 0;

        : While pointed symbol isn't a zero value
          continue iteration :
        while dref s; {
            s += 1;
            l += 1;
        }

        : Return the length of the provided
          string :
        return l;
    }

    : Define the puts function
      that accepts a pointer to a string object :
    function puts(str s) -> i0 {
        : Start ASM inline block with
          a support of the argument list :
        asm (s, strlen(s)) {
            "mov rax, 1",
            "mov rdi, 1",
            "mov rsi, %1", : Send the 'strlen(s) result' to the RSI register :
            "mov rdx, %0", : Send the 's' variable to the RDX register       :
            "syscall"
        }
    }

    : Program entry point similar to the C's entry point
      main(int argc, char* argv[]); :
    start(i64 argc, ptr ptr i8 argv) {
        puts("Hello, World!");
        exit 0;
    }
}
``` 

The same code snippet on C language:
```c
#include <stdio.h>
int main(int argc, char* argv[]) {
    puts("Hello, World!");
}
```

Actually, regardless of the essential 'basic' scope (the initial {} in the code above), with usage of the same header file, the CPL code can looks really close to the C above:
```cpl
{
    #inclide <stdio_h.cpl>
    start(i32 argc, ptr ptr i8 argv) {
        puts("Hello, World!");
    }
}
```

# Code conventions
*P.S. It's not a thing, but I'd like to share my prefered code style through this conventions. The compiler itself doesn't care about how a code is written.* </br>
CPL encourages code mostly based on C-code conventions.
- **Variables**: use lowercase letters and underscores
```cpl
i32 counter = 0;
ptr i32 data_ptr = ref counter;
dref data_ptr = 1;
```

- **Constants**: use uppercase letters with underscores
```cpl
extern ptr u8 FRAMEBUFFER;
glob ro i32 WIN_X = 1080;
glob ro i32 WIN_Y = 1920;
```

- **Functions**: use lowercase letters with underscores
```cpl
function calculate_sum(ptr i32 arr, i64 length) -> i32 { return 0; }
```

- **Private functions**: use an underscore before a name of a function, if it is a private function
```cpl
function _private();
```

- **Scopes**: K&R style
Functions:
```cpl
function foo() -> i0 {
}
```

- **Ifs**:
```cpl
if <cond0>; {
}
else if <cond1>; {
}
else {
}
```

- **Loops**:
```cpl
while <cond0>; {
}

loop {
}
```

- **Entry point**:
```cpl
start(i64 argc, ptr ptr i8 argv) {
}
```

- **Comments**: Comments can be written in one line with the start and the end symbol `:` and in several lines with the same logic. Actually, this is a copy of the C's comments '/**/' (without support of the '//' comment style).
```cpl
: Hello there
:
:
Hello there :
: Hello there :
:
Hello there
:
```

- **File names**: Sneaky case for file names. If this is a 'header' file, add the `_h` path to a name
```
print_h.cpl <- Prototypes and includes
print.cpl   <- Implementation
``` 

# Program entry point
Function becomes an entry point in two cases:
- If this is a `start` function.
- If this is an annotated with the `entry` annotation function.

Example without a `start` function:
```cpl
function fang() -> i0; { return; }
@[entry]
function naomi() -> i0; { exit 0; } : <= Becomes an entry point :
```

Example with a `start` start function:
```cpl
start() { exit 0; } : <= Becomes an entry point :
function fang() -> i0; { }
function naomi() -> i0; { }
```

As you've noticed, the `start` keyword can have arguments (`start(i32 argc, ptr ptr i8 argv)`) and can ignore them (`start()`). The both cases are valid. Also, the `start` keyword can use variadic arguments, which says that the code below:
```cpl
start(...) {
    i32 argc = poparg as i32;
    ptr ptr i8 argv = poparg as ptr ptr i8;
}
```
, is valid.

**Note 1:** 'Start' function doesn't have a return type (you can't use the '->' modificator) and requires usage of the 'exit' keyword instead of the 'return'. Also the maximum type that can be used as a value in the 'exit' keyword is the 'u8' type. </br>
**Note 2:** Actually, with usage of the logic that the lowest function becomes an entry point, we can set a return type:
```cpl
function main(i32 argc, ptr ptr i8 argv) -> u8;
```
**Note 3:** Entry point will generate all essential steps (stackframe allocation, entry, exit commands, etc).
**Note 4:** Entry point supports the `naked` annotation which disables default stack frame allocation and exit routine.
**Note 5:** Without any entry point, a file becomes a library file after the compilation.

# Types
Now let's talk about the language basics. This language is a static-typed language (And I'm trying to make him strong-typed as well). That's why CPL supports a cast operation such as the `as` operation. Syntax is similar with the Rust-language cast operation `as`. </br>
For instance:
```cpl
i32 never = 10 as i32;
i32 dies  = 20 as i32;
u8 technoblade = (never + dies) as u8;
```

One note here: Actually, there is no reason to use this statement given an unavoidable implict cast. That means that the snippet above, without these `as` statements, anyway involves a cast operation. However, to support the static-typing, I'd recommend to use the `as` statement. </br>
P.S.: *Also, the `as` keyword is really useful in terms of function overloading functions usage. We will talk about this below.*

## Primitives
Now, when we've talked about the types system, let's discuss about types itself. Primitive type is a basic, supported by this language, data structure. This data structure can be represented as:
- `f64`, `f32` - Real / double and float; non-floating values are converted to double if used in double operations.
```cpl
f64 a = 0.01;
f32 b = 0.01;
i32 d = 1;
f64 c = a + b + d;
```

- `i64`, `u64` - Long / 64-bit value.
```cpl
i64 a = 123123123;
u64 b = 0b1110011;
i64 c = 0xFFFFFFF;
```

- `i32`, `u32` - Integer / 32-bit value.
```cpl
i32 a = 123123;
u32 b = 0b0111;
i32 c = 0xFFFF;
```

- `i16`, `u16` - Short / 16-bit value.
```cpl
i16 a = 123;
u16 b = 0b1;
i16 c = 0xF;
```

- `i8`, `u8` - Character / 8-bit value.
```cpl
i8 a = 255;
u8 b = 0b0;
i8 c = 0xF;
i8 d = 'a';
```

- `i0` - Void type. Must be used only in the function return type or as a pointer. This type isn't supported by the Compiler as a regular primitive type.
```cpl
function cordell() -> i0;
ptr i0 a = cordell;
```

`P.S.` The CPL doesn't support `booleans` itself. For this purpose you can use any `non-real` data type such as `i64`, `i32`, `u8`, `ptr i0` etc. The logic here is pretty simple:
- `Not a Zero` is a `true` value.
- `Zero` is a `false` value.

## Strings and arrays
- `str` - String data type. Similar to the `ptr u8` type, but it is used for the high-level inbuild operations such as compare, len, etc. (WIP).
```cpl
str msg = "Hello world!";
if msg == "Hello world!"; {
}
```

To be honest, the string object acts the same as it does the array object. In particular, the two declaration below are the same in terms of IR presentation:
```cpl
str msg1 = "Hello world!";
arr msg2[13, i8] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
```

But in difference with an array, a string has a different behaviour when it becomes a pointer:
```cpl
ptr str msg1 = "Hello world!"; : <= Data allocated in the RO segment, pointer is set to 8 bytes :
str msg2 = "Hello world!";     : <= Data allocated in the stack                                 :
```

An array can't do the same thing. Array can allocate data in stack, but can't be used as a type of a pointer (you will need to use a primitive instead). That means, that string is somehitng between an array (non-primitive) and a primitive.

- `arr` - Array data type. Can contain any primitive type as an element type. Will allocate data in the stack or in a section (depends on the target architecture and annotations).
```cpl
arr Array_1d_1[10, i32]  = { 0 };
arr Array_1d_2[10, i32]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
arr Array_2d[2, ptr i32] = { ref Array_1d_1, ref Array_1d_2 };
i32 a = Array_2d[0][0]; : = 0 :
```

## Align
A declaration of a primitive or of an array type (an array or a string) can be annotated with the `align` keyword. This keyword will add an additional padding during memory stack allocation.
```cpl
align(16) glob i32 a;
start() {
    align(8) {
        i32 b;
        i32 c = a;
    }
}
```

By default align set to platform `bitness / 8` (For instance on the `gnu_x86_64` this is 8 bytes). </br>
The `align` keyword can be used as a modifier and as a 'scope' block. The both approaches don't change the scope of declared variables. </br>
P.S.: *The same result can be obtained with annotation usage. We will cover this below.*

## Pointers
- `ptr` - Pointer modifier that can be add to every primitive (and `str`, `arr`) type.
```cpl
i32 f = 10;
ptr i32 a = ref f;
ptr ptr a_ref = ref a;
ptr str b = "Hello world";
```

**Note:** Pointer has a maximum possible size in the target architecture. For example, in the `x86_64_gnu_nasm`, the `ptr i8` will have the `64-bit` size.

## How to deal with pointers?
Actually, it isn't too hard. This language supports two main commands to make pointers and work with values from these pointers. For example, we have a variable:
```cpl
i32 a = 123;
```

### ref
To obtain a reference link to this variable, we must use the `ref` statement:
```cpl
i32 a = 123;
ptr i32 a_ptr = ref a;

: C alternative is
int a = 123;
int* a_ptr = &a;
:
```

### dref
Similar to C-language, we can 'dereference' a pointer. To perform this, we need to use the `dref` statement:
```cpl
i32 b = dref a_ptr;
: int b = *a_ptr; :
```

Additionally, obtaining of a dereferenced value from a pointer can be performed via an indexing operation:
```cpl
i32 b = a_ptr[0];
: int b = a_ptr[0] :
``` 

## Function pointers
A function can be stored as a pointer easilly with usage of the next code:
```cpl
function foo(i32 a) -> i32;
start() {
    ptr i0 a = foo;
    i32 res = a(100);
}
```

As it can be seen from the code above, 'pointer' functions don't have a signature. Such a disadvantage disables all efforts from static analysis, function overloading and default arguments. In a nutshell, it will make this:
```cpl
function foo(i32 a) -> i32;
function foo(u32 a = 10) -> i0;
function bar(i8 a = 'a');
start() {
    ptr i0 a = foo; : Will store function foo(i32 a) -> i32; given that this function is the top function :
    ptr i0 b = bar;
    b(); : Will cause an undefined behaviour given the lack of arguemnts :
}
```

However, such an ability can make it possible to use different functions in the same context. For instance:
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

**Also** a function pointer can be not only a function (sounds weird, isn't?). It can be any pointer as well:
```cpl
(0x100 + 0xB045)(100); : <- Valid function call that will invoke a function at 0x100 + 0xB045 address :
i32 a = 123;
a(100 + 123); : <- Valid function call that will invoke any function (or not) at the 223 address :
```

## Section
A function and a global declaration can be placed in a specific section. To perform this, you will need to use the `section` keyword:
```cpl
section(".text") {
    glob i32 a;
    function foo() {}
}
```

**Note 1:** Function prototype doesn't affected by a section. To put the function's code to a section, you need to define the function. </br>
**Note 2:** By default all global/read-only variables and functions are placed in the platform's code section from the configuration. </br>
**Note 3:** Local functions can't be placed in the specific section. They will stay with their parent function in the same section.

# Binary and unary operations
Obviously this language supports the certain set of binary operations from C-language, Rust-language, Python, etc.

| Operation              | Description                                 | Example    |
|------------------------|---------------------------------------------|------------|
| `+`                    | Addition                                    | `X` + `Y`  |
| `-`                    | Subtraction                                 | `X` - `Y`  |
| `*`                    | Multiplication                              | `X` * `Y`  |
| `/`                    | Division                                    | `X` / `Y`  |
| `%`                    | Module                                      | `X` % `Y`  |
| `==`                   | Equality                                    | `X` == `Y` |
| `!=`                   | Inequality                                  | `X` != `Y` |
| `not`                  | Negation                                    | not `X`    |
| `+=` `-=` `*=` `/=` `&=` `\|=` `%=` | Update operations              | `X` += `Y` |
| `>` `>=` `<` `<=`      | Comparison                                  | `X` >= `Y` |
| `&&` `\|\|`            | Logic operations (Lazy Evaluations support) | `X` && `Y` |
| `>>` `<<` `&` `\|` `^` | Bit operations                              | `X` >> `Y` |

# Control flow statements
## if statement
`if` keyword similar to the `C`'s `if` statement. Key change here is the `;` token after the condition. 
```cpl
if <condition>; {
}
else {
}
```

For instance:
```cpl
i32 a = 10;
i32 b = 11;
if a == 12 && b < 12; {
    a = 10;
}
else {
    a = 12;
}
```

Or without a scope symbol:
```cpl
i32 a1 = 12;
i32 a;

if a1 > 10 && a1 <= 20; a = 10;
else if a1 > 20;        a = 20;
else                    a = 0;
```

## while statement
```cpl
while <condition>; {
    <statements>;
}

while <condition>; <statement>;
```

## loop statement
In difference with the `while` statement, the `loop` statement allows to build efficient infinity loops for a purpose. It is a way efficient than a `while 1;` statement given the empty 'condition' body. </br>
**Importand Note:** You *must* insert the `break` keyword somewhere in a body of this statement. Otherwise it will became an infinity loop.
```cpl
loop {
    <statements>;
}

loop <statement>;
```

## switch statement
**Note 1:** `X` should be constant value (or a primitive variable that can be `inlined`). </br>
**Note 2:** Similar to C language, the `switch` statement supports the fall 'mechanic'. It implies, that the `case` can ignore the `break` keyword. This will lead to the execution of the next case block.
```cpl
switch cond; {
    case X; {}
    case Y; {
        break;
    }
    default; {
        break;
    }
}
```

**Note 3:** *The switch statement is generated with the usage of a binary search approach. That means, consider this structure over the multiple ifs.*

# Functions and inbuilt macros
## Functions
Functions can be defined by the `function` keyword. Also, if you want to use a function in another `.cpl`/(or whatever language that supports the `extern` mechanism) file, you can append the `glob` keyword. One note here, that if you want to invoke this function from another language, keep in mind, that the CPL changes a local function name by the next pattern: `__cpl_{name}{id}`, that's why prefer mark them with the `glob` key (It will preserve a name from a changing). 
```cpl
function min(i32 a) -> i0 { return; }
glob function chloe(i32 a = 10) -> u64 { return a + 10; }
function max(u64 a = chloe(11)) -> i32 { return a + 10; }
```

Name change example is below:
```cpl
function foo(); : _cpl_foo0 :
glob function foo(); : foo :
```

**Note:** Global functions don't support function overloading and scoping.

### Prototypes
Function can have a prototype function. Similar to C-language, a prototype function - is a function without a body:
```cpl
function chloe(i32 a = 10) -> u64;
function max() -> i32 { 
    return chloe();
}

function chloe(i32 a = 10) -> u64 { 
    return a + 10; 
}
```

### Default
CPL supports default values in functions. Compiler will pass these default args in a function call if you don't provide enough:
```cpl
chloe();              : chloe(10); :
max();                : max(chloe(11)); :
min(max() & chloe()); : min(max(chloe(11)) & chloe(10)); :
```

There is some restrictions / flaws / rules:
- Default can't be before a non-default value:
```cpl
function foo(i32 a = 1, i32 b); : <= Forbidden :
```
- Defaults must be duplicated both in a function and a function's prototype. 

### Function overloading
Additionally, CPL supports function overloading with some flaws (All of these are based on the compiler's architecture. See the main README for more information):
- It doesn't support overloading with the 'default' argument.
- CPL overloading doesn't work with the 'return' type of a function.
- Overloaded function can't be marked as a global / extern function.

That means this will work:
```cpl
function foo(i32 a);
function foo();
function foo(i8 a);
```

Note: *To make sure that the compiler will choose the correct version of a function, use the 'as' keyword.*
```cpl
foo(10 as i32);
: i64 a; :
foo(a as i8);
```

But these two snippets won't:
```cpl
function foo() -> i32;
function foo() -> i0;
```
, and
```cpl
function foo(i32 a, i32 b = 1, i32 c = 1);
function foo(i32 a, i32 b = 1);
```

## Local functions
Local function can be easily defined by the next code:
```cpl
function foo() {
    function bar() {
    }
}
```

Such local functions don't have any access to a 'parent' function, but can be returned as a pointer from a 'parent':
```cpl
function foo() -> ptr i0 {
    function getter() {
        return 10;
    }
    return getter;
}
```

Local functions have their own scope, that's why it allows to name local and global functions with the same name:
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

## Variadic arguments
CPL supports variadic arguments in the same way hot it supports C language. To use the variadic arguments in a function, add the `...` lexem **as the final arguement**!
```cpl
function foo(...) -> i0;
```

To 'pop' an arguement from this set, use the `poparg` keyword. It behaves as a variable with a 'variable' value:
```cpl
function foo(...) -> i0 {
    i8 a1 = poparg as i8;
    i8 a2 = poparg as i8;
}
```

Also, the `poparg` keyword can be used in a traditional function:
```cpl
function foo(i32 a, i32 b) -> i0 {
    i8 a1 = poparg as i8; : a :
    i8 b1 = poparg as i8; : b :
}
```

### Calling convention (System information)
Obviously, if we're talking about variadic arguments, we should mention the calling convention. Actually, the calling convention entirely based on the target architecture. For example, the basic architecture is the `x86_64_gnu_nasm` which uses the C's calling convention. The same situation in the `x86_32_gnu_nasm` and the `x86_16_gnu_nasm` architectures (But they aren't ready yet). </br>
The result of a function is placed in the registed (for example `rax`/`ax`/`al`), and a function will 'protect' registers that are used in its body. For instance:
```cpl
function foo() {
    mov r10, 10
}
push r10 : <= Will save the 'r10' register :
foo();
i32 res = rax;
pop r10
```

## Inbuilt macros
There is two inbuild functions that can be usefull for a system programmer. First is the `syscall` function.
- `syscall` function is called similar to default user functions, but can handle an unfixed number of arguments. For example here is the write syscall:
```cpl
str msg = "Hello, World!";
syscall(1, 1, ref msg, strlen(ref msg));
```

- `asm` - Second usefull function that allows to inline an assembly code. Main feature here is a variables line, where you can pass any number of arguments, then use them in the assembly code block via `%<num>` symbols.
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

**Note:** Inlined assembly block isn't optimized by the compiler.

### How works assembly? (System information)
Actually, inline assembly is a dangerous construction. Considering the fact, that we can't predict a code before and after this segment, we need to pay extra attention. For example, protect all registers that are used in the block:
```cpl
asm() {
    "push rax",
    "mov rax, 10",
    "pop rax"
}
```

Also, make sure that you're writting the valid assembly according to a target architecture of compilation. This means, that if you're compiling a code for a `amd` style assembly, or for a `risc` architecture, you must write the correct assembly with the correct style. This construction will just copy and past (and process `%<num>` entries) the entire block into the final output. </br>
I do not recommend to use labels and jums in these blocks. You actually can do this, but it will conflict with many optimizations. That's why use this with the flag `--no-optimizations`. I'm speaking about this code:
```cpl
asm() {
    "jmp label1"
}
i32 a; : <= Dead code :
asm() {
    "label1:"
}
```

**Note:** Actually this construction will accept any assembly code that can be compiled with the target assembly.

# Annotations
Annotations are usefull tool in terms of system development. For non-system programmer, annotations are redundant (even the aligns and sections are avaliable by inbuilt keywords). But let's review all aavaliable annotations (at this moment):
- `naked` - Will disable all entry and exit routines in the final assembly code for an annotated function.
- `align` - Will do the same work as it does the 'align' keyword.
- `section` - Will do the same work as it does the 'section' keyword.
- `address` - Will put a function to a specific address.
- `entry` - Set function as an entry point of the code.

Annotations can be added for functions, function arguments and declarations:
```cpl
@[align(16)] i32 a;
@[entry]
@[naked]
function main() -> i0;
@[section(".bss")] glob i32 b;
```

As you can see, these annotations copy all functionality of the 'align' and 'section' keywords. Actually, it doesn't mean that these keywords now redundant. They still is a pretty convenient way of block code. For example these code:
```cpl
section (".bss") {
    align (16) {
        glob i32 a;
        glob i32 b;
        glob i32 c;
    }
    function foo();
    function bar();
}
``` 

can be reproduced with annotations:
```cpl
@[section(".bss")] @[align(16)] glob i32 a;
@[section(".bss")] @[align(16)] glob i32 b;
@[section(".bss")] @[align(16)] glob i32 c;
@[section(".bss")] function foo();
@[section(".bss")] function bar();
```

, but obviously, now it doesn't look too clean.

# Debugging
## Interrupt point 
Code can be interrupted (use `gdb`/`lldb`) with `lis` keyword. Example below:
```cpl
{
    start() {
        i32 a = 10;
        lis "Debug stop"; : <- Will interrupt execution here :
        exit 0;
    }
}
```

# Macros & include
The compiler includes a preprocessor that will take care about statements such as `#include`, `#define`, `#ifdef`, `#ifndef` and `#undef`. Most of them act similar to `C/C++`. For example, `#include` statement must be used only with a 'header' file. How to create a 'header' file? </br>
For example, we have a file with the implemented string function:
```cpl
{
    function strlen(ptr i8 s) -> i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }
}
```

This function is independent from others and can exist without any dependencies. But how to use this function in other files? We need to create a 'header' file:
```cpl
{
#ifndef STRING_H_
#define STRING_H_ 0
    : Get the size of the provided string
      Params
        - `s` - Input string.

      Returns the size (i64). :
    function strlen(ptr i8 s) -> i64;
#endif
}
``` 

This header file includes only the prototype and guards.

# Semantic static checker
Cordell Compiler implements a simple static analysis tool for a basic code-checking before a compilation. It supports the next list of errors and warnings:
- Read-only variable update. *If we have a `ro` variable, we must be sure that it never being updated somewhere*
- Invalid variable for a function's return value. *If a function returns, for instance, a `i8` value, it must be stored in a variable with the same (or larger) type*
- Declaration without initialization. *If we declare a variable, it'd be safer, if we add an initial value*
- Wrong value type for a variable declaration. *If we declare a variable with an initial value, we must be sure, that this value can be stored in this variable*
- Function without return. *If function has a return type not equals to the `i0` return type, it must have a `return` statement in all paths*
- Start block without the exit. *The initial block must have an `exit` statement in all paths*
- Function arguments number mismatch. *Function (not a function pointer) must have the exact amount of arguments as it registered in function's definition*
- Function argument type mismatch. *We must be sure, that a provided argument to a function has a data type, that can be processed by a function*
- Unused function return value. *If a function has a return type not equal to the `i0` return type, it must be used somewhere*
- Wrong variable for a function's return type. *If we store a function's result in a variable, we must check if the variable has a valid type and can handle the function's return value*
- Function's return type mismatch with an actual return type. *If we return some value from a function, we must check if this is a corrent type regarding the registered information about the function*
- Illegal array access. *We must check if the index (constant) that being used in an array expression is valid and non-negative*
- Duplicated branches. *We can check if there is a two same branches in one `if` construction*
- Invalid function name. *Some function names are reserved by the compiler. We can't allow user to use them*
- Dead code. *We can find is there is a dead code in the code, and if this was an intent product*
- Possible implicit convertion. *The compiler is a static typed, but not a strict typed. To fill this gap, the checker will inform if there is a possible future implicit cast*
- Inefficient `while`. *Sometimes the `loop` keyword is better than `while 1`*
- Incorrect exit type for a function. *The `exit` keyword must be used in a function (not in a `start` function) only by one condition - there is no `start` function and this is the lowest non-local function in the file*
- Break usage without a target. *The `break` keyword must be used only to break `loop`s and `while`s*
- `i0` function's return value usage. *If a function has a `i0` return type, its value can't be stored in any variable*
- Unused expression. *Any expression that doesn't stored in a variable, used in a function, evaluated in a `if`, a `while` or a `switch` statements is an unused expression*
- Reference to an expression. *Reference operation could be performed only on variable*
- Non-even align. *If align of a variable or an array isn't even, we must inform the programmer about this*

**Note:** By default the static analysis is turned off. To turn it on, use the `--static-analysis` flag.

For instance, let's consider the code below:
```cpl
{
    #include "string_h.cpl"
    function foo() -> i32 { return 1; }

    function barBar() -> i0 { }

    function BazBaz() { return 1; } 

    function baz(i32 a) -> i0 {
        if a == 0; { return 1; }
        else { return 1; }

        if a == 0; { return 1; }
        else { }
    }

    function fang(i32 a) -> i8 {
        if not a; { return 123321; }
        else { }
        return 1;
        i32 b = 1;
        return b;
    }

    start() {
        i8 b;
        ptr i8 bref = ref b;
        ptr i8 bref1 = bref;
        ptr i8 bref2 = bref1;

        i8 a = foo();
        i8 c = 123123;
        u8 asd = barBar();
        baz(1);
        fang(10);
        
        break;
        a + c;

        arr array[10, i32];
        array[-1] = 0;
        array[12] = 0;
        strlen("Hello!");
    }
}
```

The code above will produce a ton of errors and warnings.
<details>
<summary><strong>Semantic analysis output</strong></summary>

```
[WARNING] Possible branch redundancy! The branch at [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:11:25] is similar to the branch at [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:11:25]!
10 | if a == 0;
10 | {
   | ^
10 |     return 1;
   | ^^^^^^^^^^^^^
10 | }
   | ^
11 | else {
   | ^    ^
11 |     return 1;
   | ^^^^^^^^^^^^^
11 | }
   | ^
   | ^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:20:16] 'Dead Code' after the termination statement!
18 | {
18 |     if not a;
18 |     {
18 |         return 123321;
18 |     }
19 |     else {
19 |     }
20 |     return 1;
21 |     i32 b = 1;
   |     ^^^^^^^^^
22 |     return b;
18 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:33:28] The function='barBar' doesn't return anything, but result is used!
33 | u8 asd = barBar();
   |          ^^^^^^^^
[ERROR]   [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:41:20] Array='array' accessed with a negative index!
41 | array[-1];
   |       ^^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:38:14] The expression returns value that never assigns!
26 | {
26 |     i8 b;
27 |     ptr i8 bref = ref b;
28 |     ptr i8 bref1 = bref;
29 |     ptr i8 bref2 = bref1;
31 |     i8 a = foo();
32 |     i8 c = 123123;
33 |     u8 asd = barBar();
34 |     baz(1);
35 |     fang(10);
37 |     break ;
38 |     a + c;
   |     ^^^^^
40 |     arr array = 10;
41 |     array[-1] = 0;
42 |     array[12] = 0;
43 |     strlen(Hello!);
26 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:37:15] The 'break' statement without any statement that uses it!
26 | {
26 |     i8 b;
27 |     ptr i8 bref = ref b;
28 |     ptr i8 bref1 = bref;
29 |     ptr i8 bref2 = bref1;
31 |     i8 a = foo();
32 |     i8 c = 123123;
33 |     u8 asd = barBar();
34 |     baz(1);
35 |     fang(10);
37 |     break ;
   |     ^^^^^^
38 |     a + c;
40 |     arr array = 10;
41 |     array[-1] = 0;
42 |     array[12] = 0;
43 |     strlen(Hello!);
26 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:35:14] Unused the function='fang's result!
26 | {
26 |     i8 b;
27 |     ptr i8 bref = ref b;
28 |     ptr i8 bref1 = bref;
29 |     ptr i8 bref2 = bref1;
31 |     i8 a = foo();
32 |     i8 c = 123123;
33 |     u8 asd = barBar();
34 |     baz(1);
35 |     fang(10);
   |     ^^^^^^^^
37 |     break ;
38 |     a + c;
40 |     arr array = 10;
41 |     array[-1] = 0;
42 |     array[12] = 0;
43 |     strlen(Hello!);
26 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:32:26] Illegal declaration of 'c' with '123123' (Number bitness is=32, but 'i8' can handle bitness=8)!
32 | i8 c = 123123;
   |        ^^^^^^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:31:15] Function='foo' return type='i32' not match to the declaration type='i8'!
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:26:15] Variable='b' without initialization!
26 | i8 b;
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:25:11] Start doesn't have the 'exit' statement in all paths!
25 | start ()
25 | {
26 |     {
26 |         i8 b;
27 |         ptr i8 bref = ref b;
28 |         ptr i8 bref1 = bref;
29 |         ptr i8 bref2 = bref1;
31 |         i8 a = foo();
32 |         i8 c = 123123;
33 |         u8 asd = barBar();
34 |         baz(1);
35 |         fang(10);
37 |         break ;
38 |         a + c;
40 |         arr array = 10;
41 |         array[-1] = 0;
42 |         array[12] = 0;
43 |         strlen(Hello!);
26 |     }
25 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:10:38] Function='baz' has the return value, but isn't supposed to!
10 | return 1;
   |        ^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:11:28] Function='baz' has the return value, but isn't supposed to!
11 | return 1;
   |        ^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:13:38] Function='baz' has the return value, but isn't supposed to!
13 | return 1;
   |        ^
[INFO]    [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:5:22] Function name='barBar' isn't in a sneaky_case! 'camelCase'
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:5:14] Function='barBar' doesn't have the 'return' statement in all paths!
 5 | function barBar() -> i0 
 5 | {
 5 | }
```
</details>

**Note 1:** This isn't an entire analysis output due to the critical error with the array indexing. Such errors will block a compilation process given the importance of this kind of errors. </br>
**Note 2:** The static analyzer doesn't use a source file to show a error place. For these purposes, it uses the 'restorer' module that restores the code from AST.

# Scopes
## Variables and lifetime
Variables live in their declared scopes. You cannot point to variables from an outer scope. This makes the manual program stack managment a way easier given the determined behavior of the stack allocator in this compiler. 
```cpl
start() {
   ptr u64 p;
   {
      arr t[10; i32];
      p = ref t; : <= No warning here, but it still illegal :
   }             : <= array "t" died here :

   p[0] = 1;     : <= Pointer to allocated but 'freed' stack :
   exit 0;
}
```

**Note 1:** Example above will cause a memory corruption error instead of the `SF` due the stack allocation method in CPL. (The pointer after the scope is pointing to the already allocated area. However, the compiler can use this area for the another array / variable, etc.). </br>
**Note 2:** This compiler tries to `kill` all variables / arrays / strings outside their scopes, even if they are used as a referenced value somewhere else in the further code. </br>
**Note 3:** In the example above, execution may be success (further code can ignore the 'freed' space in the stack and prefer the register placement for new variables), but it is still the Undefined Behavior. 

## Visibility rules
Outer variables can be seen by current and nested scopes.
```cpl
{
   {
      i32 a = 10; : <= Doesn't see any variables :
   }

   i64 b = 10; : <= Doesn't see any variables :

   {
      i8 c = 9; : <= See the "b" variable :

      {
         f32 a = 10.0; : <= See the "b" and the "c" variables :
      }

      i8 a = 0; : <= See the "b" and the "c" variables :
   }
}
```

# Liveness-based stack slot reuse
CPL performs a compile-time analysis of variable lifetimes to reduce stack usage. The compiler tracks when a local variable is still needed (“live”) and when it is no longer used (“dead”). Once a variable is proven dead, its stack slot may be reused for another variable declared later.

## Idea
- Every local variable has a lifetime interval: from its initialization until its last use.
- A pointer/reference created with ref is treated as a use that may extend the lifetime of the referenced value.
- The compiler may reuse stack memory only when it can prove that the old value and all its references are no longer used.

Think of it as stack coloring or liveness-based stack slot allocation: two variables that are never live at the same time can share the same stack memory

## What this is NOT
Not a memory-safety system

- CPL doesn't prevent all dangling pointers or aliasing issues. The analysis is used to avoid incorrect stack reuse, not to “make pointers safe”.
- Not Rust ownership / borrowing.
- Rust enforces rules like “one mutable reference or many immutable ones” and prevents data races / use-after-free in safe code.

CPL does not enforce these restrictions. CPL only ensures that the compiler does not intentionally reuse a stack slot while it is still provably needed.

## Example
```cpl
{
    start() {
        i32 a = 0; : <= Allocate 8 bytes :
        ptr i32 p; : <= Allocate 8 bytes :
        if 1; {
            p = ref a; : <= "p" becomes a new owner of "a" :
        }

        i32 c = 0; : <= "p" is still alive, so "a" is not reusable yet :
        exit p;
    }
}
```

**Note 1!:** Be sure that you've disabled all optimizations before the code debug given the preservation of a code from transformation.
**Note 2!:** To make this works, use any debugging tool such as `gdb` and `lldb`.

# Building
Compiler compiles an input file to a assembly file. This means, that after this pipeline:
```
Code -> PP -> Tokens -> AST -> [Sem] -> HHIR -> LHIR -> HLIR -> LLIR -> Codegen
```

We will obtain an assembly format code file. After this you can use `NASM` compiler (if was used the `XXX_nasm` architecture) and `ld` linker. If you've added some functions from `stdlib` (with the `extern` keyword usage), you will need to link them together.

# Examples
## strlen
```cpl
{
    function strlen(ptr i8 s) -> i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }
}
```

<details>
<summary><strong>More examples</strong></summary>

## print.cpl
```cpl
: strlen_h.cpl :
{
#ifndef STRLEN_H_
#define STRLEN_H_ 0
    function strlen(ptr i8 s) -> i64;
#endif
}
: print_h.cpl :
{
#ifndef PRINT_H_
#define PRINT_H_ 0
    function print(ptr i8 s) -> i0;
#endif
}
: print.cpl :
{
    #include <print_h.cpl>

    function print(ptr i8 s) -> i0 {
        syscall(1, 1, strlen(s), s);
    }
}
```

## memset.cpl
```cpl
: memset_h.cpl :
{
#ifndef MEMSET_H_
#define MEMSET_H_ 0
    function memset(ptr u8 buffer, u8 val, u64 size) -> i0;
#endif
}
: memset.cpl :
{
    #include <memset_h.cpl>

    function memset(ptr u8 buffer, u8 val, u64 size) -> i0 {
        u64 index = 0;
        while index < size; {
            buffer[index] = val;
            index += 1;
        }
    }
}
```

## file.cpl
```cpl
{
    function puts(ptr i8 s) -> i64 {
        return syscall(1, 1, s, strlen(s));
    }

    function putc(i8 c) -> i64 {
        return syscall(1, 1, ref c, 1);
    }

    function gets(ptr i8 buffer, i64 size) -> i64 {
        return syscall(0, 0, buffer, size);
    }

    function open(ptr i8 path, i32 flags, i32 mode) -> i64 {
        return syscall(2, path, flags, mode);
    }

    function fwrite(i32 fd, ptr u8 buffer, i32 size) -> i64 {
        return syscall(1, fd, buffer, size);
    }

    function fread(i32 fd, ptr u8 buffer, i32 size) -> i64 {
        return syscall(0, fd, buffer, size);
    }

    function close(i32 fd) -> i64 {
        return syscall(3, fd);
    }
}
```

## brainfuck.cpl
```cpl
{
    #include "stdio_h.cpl"

    glob arr tape[30000, i8];
    glob arr code[10000, i8];
    glob arr bracketmap[10000, i32];
    glob arr stack[10000, i32];

    start() {
        puts("Brainfuck interpriter! Input code: ");

        i32 pos = 0;
        i32 stackptr = 0;
        i32 codelength = gets(code, 10000);
        while pos < codelength; {
            switch code[pos]; {
                case '['; {
                    stack[stackptr] = pos;
                    stackptr += 1;
                    break;
                }
                case ']'; {
                    if stackptr > 0; {
                        stackptr -= 1;
                        i32 matchpos = stack[stackptr];
                        bracketmap[pos] = matchpos;
                        bracketmap[matchpos] = pos;
                    }

                    break;
                }
                default { break; }
            }
            
            pos += 1;
        }
        
        i32 pc = 0;
        i32 pointer = 0;

        while pc < codelength; {
            switch code[pc]; {
                case '>'; {
                    pointer += 1;
                    pc += 1;
                    break;
                }
                case '<'; {
                    pointer -= 1;
                    pc += 1;
                    break;
                }
                case '+'; {
                    tape[pointer] += 1;
                    pc += 1;
                    break;
                }
                case '-'; {
                    tape[pointer] -= 1;
                    pc += 1;
                    break;
                }
                case '.'; {
                    putc(tape[pointer]);
                    pc += 1;
                    break;
                }
                case ','; {
                    gets(ref tape[pointer], 1);
                    pc += 1;
                    break;
                }
                case '['; {
                    if not tape[pointer]; { pc = bracketmap[pc]; }
                    else { pc += 1; }
                    break;
                }
                case ']'; {
                    if tape[pointer]; { pc = bracketmap[pc]; }
                    else { pc += 1; }
                    break;
                }
                default {
                    pc += 1;
                    break;
                }
            }
        }

        exit 0;
    }
}
```
</details>
