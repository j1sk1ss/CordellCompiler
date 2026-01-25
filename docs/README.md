# Summary
The **Cordell Programming Language (CPL)** is a system-level programming language designed for learning and experimenting with modern compiler concepts. It combines low-level capabilities from `ASM` with practices inspired by modern languages like `Rust` and `C`. `CPL` is intended for:
- **Systems programming** — operating systems, DBMSs, compilers, interpreters, and embedded software.  
- **Educational purposes** — a language to study compiler design, interpreters, and programming language concepts.
- **Radical simplicity** - Similar to RISC, *we can construct any complex abstraction without complex abstractions*. 

## Key Features
- **Partial strong-typing**: variables can't hold values of different types; the compiler attempts implicit conversions when assigning.  
- **Explicit memory model**: ownership rules and manual memory management are core features.  
- **Minimalistic syntax**: designed for readability and precision.  
- **Deterministic control flow**: no hidden behaviors; all execution paths are explicit.  
- **Extensibility**: functions and inbuilt macros allow both low-level operations and high-level abstractions.
- **Optimization**: input code can be optimized in a lot of ways. 

# Main idea of this project
Main goal of this project is learning of compilers architecture and porting one to CordellOS project (I want to code apps for my own OS inside my own OS). Also, according to my bias to assembly and C languages (I just love them), this language will stay "low-level" as it possible, but some features can be added in future with strings (inbuild concat, comparison and etc).

# Used links and literature
- Aarne Ranta. *Implementing Programming Languages. An Introduction to Compilers and Interpreters*
- Aho, Lam, Sethi, Ullman. *Compilers: Principles, Techniques, and Tools (Dragon Book)*
- Andrew W. Appel. *Modern Compiler Implementation in C (Tiger Book)*
- Cytron et al. *Efficiently Computing Static Single Assignment Form and the Control Dependence Graph* (1991)
- Daniel Kusswurm. *Modern x86 Assembly Language Programming. Covers x86 64-bit, AVX, AVX2 and AVX-512. Third Edition*

# Hello, World! example
That's how we can write a 'hello-world' program with CPL language.
```cpl
{
    : Define the strlen function
      that accepts a pointer to a char array :
    function strlen(ptr i8 s) => i64 {
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
      that accepts a pointer to string object :
    function puts(ptr str s) => i0 {
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

    : Program entry point similar to C's entry point
      main(int argc, char* argv[]); :
    start(i64 argc, ptr u64 argv) {
        puts("Hello, World!");
        exit 0;
    }
}
``` 

## Code conventions
CPL encourages consistent and readable code mostly based on C-code conventions.
- **Variables**: use lowercase letters and underscores [C-code convention]
```cpl
i32 counter = 0;
ptr i32 data_ptr = ref counter;
dref data_ptr = 1;
```

- **Constants**: use uppercase letters with underscores [Python-code convention]
```cpl
extern ptr u8 FRAMEBUFFER;
glob ro i32 WIN_X = 1080;
glob ro i32 WIN_Y = 1920;
```

- **Functions**: use lowercase letters with underscores [C-code convention]
```cpl
function calculate_sum(ptr i32 arr, i64 length) => i32 { return 0; }
```

- **Scopes**: K&R style
```cpl
if cond; {
}
while cond; {
}
start(i64 argc, ptr u64 argv) {
}
```

- **Comments**: Comments can be written in the one line with the start and the end symbol `:` and in several lines with the same logic. [ASM-code convention + C-code convention]
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

- **File names**: Sneaky case for file names. If this is a 'header' file, add the `_h` path to the name.
```
print_h.cpl <- Prototypes and includes
print.cpl   <- Implementation
``` 

# Program entry point
Function becomes an entry point in two cases:
- If this is a `start` function.
- If this is the lowest function in the file (And! There is no any `start` function in this project).

Example without the `start` function:
```cpl
function fang() => i0; { return; }
function naomi() => i0; { return; } : <= Becomes an entry point :
```

Example with the `start` start function:
```cpl
start() { exit 0; } : <= Becomes an entry point :
function fang() => i0; { return; }
function naomi() => i0; { return; }
```

# Types
Now let's talk about language basics. This language is a strong-typed language. That's why CPL supports a cast operation such as `as` operation. Syntax is similar with a Rust-language cast operation `as`. </br>
For instance:
```cpl
i32 never = 10 as i32;
i32 dies  = 20 as i32;
u8 technoblade = (never + dies) as u8;
```

One note here: Actually, there is no reason to use this statement given an unavoidable implict cast. This means that the snippet above, without these `as` statements, anyway involves a cast operation. However, to support the strong-typing, I'd recommend to use the `as` statement.

## Primitives
Now, when we've talked about the typing system, let's discuss about the types itself. Primitive type is a basic, supported by this language data structure. This data structure supports the next list of avaliable types:
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

- `i0` - Void type. Must be used only in the function return type. This type isn't supported by the Compiler as a regular primitive type.
```cpl
function cordell() => i0; { return; }
```

`P.S.` The CPL doesn't support `booleans` itself. For this purpose you can use any `non-real` data type such as `i64`, `i32`, `u8`, etc. The logic here is simple:
- `Not a Zero` is a `true` value.
- `Zero` is a `false` value.

## Strings and arrays
- `str` - String data type. Similar to the `ptr u8` type, but it is used for the high-level inbuild operations such as compare, len, etc. (WIP).
```cpl
str msg = "Hello world!";
if msg == "Hello world!"; {
}
```

- `arr` - Array data type. Can contain any primitive type.
```cpl
arr 1dArray_1[10, i32];
arr 1dArray_2[10, i32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
arr 2dArray[2, ptr i32] = { ref 1dArray_1, ref 1dArray_2 };
```

Also array can have an unknown in `compile-time` size. This will generate code that allocates memory in heap (WIP). 
```cpl
extern i8 size;
arr arr1[size, i32];
```

`Runtime-sized` arrays will die when code returns from their home scope. That's why the code below still illegal (Works only in the cplv2):
```cpl
extern i8 size;
ptr u8 a;
{
    arr arr1[size, i32];
    a = ref arr1;
}         : <= "arr1" is deallocated. Work with this "a" will cause a SF :
a[0] = 0; : <= SF! :
```

## Pointers
- `ptr` - Pointer modifier that can be add to every primitive (and `str`) type.
```cpl
i32 f = 10;
ptr u64 a = ref f;
ptr str b = "Hello world";
```

## How to deal with pointers?
Actually, pretty simple. This language supports two main commands to make pointers and to work with values from these pointers. For example, we have a variable:
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
Similar to C-language, we can 'dereference' the pointer. To perform this, we need to use the `dref` statement:
```cpl
i32 b = dref a_ptr;
: int b = *a_ptr; :
```

Additionally, obtaining of a dereferenced value from a pointer can be performed via an indexing operation:
```cpl
i32 b = a_ptr[0];
: int b = a_ptr[0] :
``` 

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
`if` keyword similar to the `C`'s `if` statement. Key change here is a `;` after the condition. 
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

## while statement
```cpl
while <condition>; {
    break;
}
```

## loop statement
In difference with the `while` statement, the `loop` statement allows to build efficient infinity loops for a purpose. It is a way efficient than a `while 1;` statement given the empty 'condition' body. </br>
**Importand Note:** You *must* insert the `break` keyword somewhere in a body of this statement. Otherwise it will became an infinity loop.
```cpl
loop {
    break;
}
```

## switch statement
Note: `X` should be constant value (or a primitive variable that can be `inlined`). </br>
Note 2: Similar to C language, the `switch` statement supports the fall 'mechanic'. It implies, that the `case` can ignore the `break` keyword. This will lead to the execution of the next case block.
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

# Functions and inbuilt macros
## Functions
Functions can be defined by the `function` keyword. Also, if you want to use a function in another `.cpl`/(or whatever language that supports the `extern` mechanism) file, you can append the `glob` keyword. One note here, that if you want to invoke this function from another language, keep in mind, that the CPL changes a local function name by the next pattern: `__cpl_{name}`, that's why prefer mark them with the `glob` key (It will preserve a name from a changing). 
```cpl
function min(i32 a) => i0 { return; }
glob function chloe(i32 a = 10) => u64 { return a + 10; }
function max(u64 a = chloe(11)) => i32 { return a + 10; }
```

Function can have a prototype function. Similar to C-language, a prototype function - is a function without a body:
```cpl
function chloe(i32 a = 10) => u64;
function max() => i32 { 
    return chloe();
}

function chloe(i32 a = 10) => u64 { 
    return a + 10; 
}
```

CPL supports default values in functions. Compiler will pass these default args in a function call if you don't provide enough:
```cpl
chloe();              : chloe(10); :
max();                : max(chloe(11)); :
min(max() & chloe()); : min(max(chloe(11)) & chloe(10)); :
```

## Variadic arguments
CPL supports variadic arguments in the same way hot it supports C language. To use the variadic arguments in a function, add the `...` lexem **as the final arguement**!
```cpl
function foo(...) => i0 {
}
```

To 'pop' an arguement from this set, use the `poparg` keyword. It behaves as a variable with a 'variable' value:
```cpl
function foo(...) => i0 {
    i8 a1 = poparg as i8;
    i8 a2 = poparg as i8;
}
```

Also, the `poparg` keyword can be used in a traditional function:
```cpl
function foo(i32 a, i32 b) => i0 {
    i8 a1 = poparg as i8; : a :
    i8 a2 = poparg as i8; : b :
}
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
i32 ret = 0;
asm(a, ret) {
   "mov rax, %0 ; mov rax, a",
   "syscall",
   "mov %1, rax ; mov ret, rax"
}
```

Note: Inlined assembly block doesn't optimized by any alghorithms.

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
    function strlen(ptr i8 s) => i64 {
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
    function strlen(ptr i8 s) => i64;
#endif
}
``` 

This header file includes only the prototype and guards.

# Semantic static checker
Cordell Compiler implements a simple static analysis tool for a basic code-checking before a compilation. It supports the next list of errors and warnings:
- Read-only variable update 
```cpl
ro i8 a = 10;
a = 11; : <= RO_ASSIGN! :
```

- Invalid place for function return
Example:
```cpl
function foo() => ptr u64 { :...: }
i8 a = foo(); : <= INVALID_RETURN_TYPE! :
```

- Declaration without initialization
- Illegal declaration
Example:
```cpl
i8 a = 123321; : <= ILLEGAL_DECLARATION! 123321 is a 32-bit value :
```

- Function without return
- Start block without exit
- Function arguments lack
- Function argument type mismatch
- Unused function return value
- Illegal array access
Example:
```cpl
arr a[10, i32];
a[11] = 0; : <= ILLEGAL_ARRAY_ACCESS! :
```

- Invalid function name
Example:
```cpl
function _test() { :...: } : <= INVALID_FUNCTION_NAME! :
```

- Inefficient `while`
Example:
```cpl
while 1; { : ... : : <= INEFFICIENT_WHILE! :
}
: Use the loop { } instead! :
```
- etc.

CPL uses an inbuild static analyzator for the code checking before the compilation. For example, such an analyzator helps programmer to work with the code like below:
```cpl
{
    #include "string_h.cpl"
    function foo() => i32 { return 1; }

    function barBar() => i0 { }

    function BazBaz() { return 1; } 

    function baz(i32 a) => i0 {
        if a == 0; { return 1; }
        else { return 1; }

        if a == 0; { return 1; }
        else { }
    }

    function fang(i32 a) => i8 {
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

The code above will produce a ton of errors and warnings:
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
 5 | function barBar() => i0 
 5 | {
 5 | }
```

Note: This isn't an entire analysis output due to the critical error with the array indexing. Such errors will block a compilation process given the importance of this kind of errors. </br>
Note 2: The static analyzer doesn't use a source file to show a error place. For these purposes, it uses the 'restorer' module that restores the code from AST.

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

Note 1: Example above will cause a memory corruption error instead of the `SF` due the stack allocation method in CPL. (The pointer after the scope is pointing to the already allocated area. However, the compiler can use this area for the another array / variable, etc.).
Note 2: This compiler tries to `kill` all variables / arrays / strings outside their scopes, even if they are used as a referenced value somewhere else in the further code.
Note 3: In the example above, execution may be success (further code can ignore the 'freed' space in the stack and prefer the register placement for new variables), but it is still the Undefined Behavior. 

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
         f32 a = 10; : <= See the "b" and the "c" variables :
      }

      i8 a = 0; : <= See the "b" and the "c" variables :
   }
}
```

# Ownership rules
## Ownership model vs Rust
CPL uses a lightweight ownership model with `register allocation` that resembles Rust’s borrow checker, but it serves a different purpose and operates with fewer restrictions.  

### Similarities
- **Ownership tracking**:  
  Each variable can have one or more owners. Ownership is explicitly transferred with the `ref` keyword.  
- **Lifetime-based reuse**:  
  When a variable and all of its owners are no longer used, its stack slot can be safely reused by another variable.  
- **Compile-time analysis**:  
  The compiler analyzes usage and ownership before code generation, preventing unsafe reuse of stack slots.

### Differences
1. **Multiple owners allowed**  
   In Rust, only one owner exists at a time, while in CPL multiple owners may coexist in the ownership list.
2. **Optimization-oriented**  
   Rust’s borrow checker enforces memory safety rules.  
   Cordell’s ownership tracking exists primarily to enable **stack slot reuse** and reduce stack frame size.
3. **Memory reuse instead of drops**  
   Rust frees resources automatically at the end of a lifetime (`Drop`).  
   CPL does not guarantee cleanup but instead marks the memory slot as reusable once ownership is gone.
4. **Low-level design**  
   CPL's model is closer to compiler optimizations such as **SSA transformation, register allocation, or stack coloring**, while Rust’s model is a high-level safety feature of the language.

### Example
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

Note! Disable optimizations before a code debugging given the preservation the code from a transformation.
Note 2! To make this works, use any debugging tool such as `gdb` and `lldb`.

# Examples
## strlen
```cpl
{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }
}
```

## memset
```cpl
{
    function memset(ptr u8 buffer, u8 val, u64 size) => i0 {
        u64 index = 0;
        while index < size; {
            buffer[index] = val;
            index += 1;
        }
    }
}
```

## fd functions
```cpl
{
    function puts(ptr i8 s) => i64 {
        return syscall(1, 1, s, strlen(s));
    }

    function putc(i8 c) => i64 {
        arr tmp[2, i8] = { c, 0 };
        return syscall(1, 1, tmp, 2);
    }

    function gets(ptr i8 buffer, i64 size) => i64 {
        return syscall(0, 0, buffer, size);
    }

    function open(ptr i8 path, i32 flags, i32 mode) => i64 {
        return syscall(2, path, flags, mode);
    }

    function fwrite(i32 fd, ptr u8 buffer, i32 size) => i64 {
        return syscall(1, fd, buffer, size);
    }

    function fread(i32 fd, ptr u8 buffer, i32 size) => i64 {
        return syscall(0, fd, buffer, size);
    }

    function close(i32 fd) => i64 {
        return syscall(3, fd);
    }
}
```

## Brainfuck
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
            i8 c = code[pos];
            switch c; {
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