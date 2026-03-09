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
