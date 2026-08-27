# Functions and built-ins

## Function definitions

Functions are declared with `function`:

```cpl
function add(i64 a, i64 b) -> i64 {
    return a + b;
}
```

A function that returns nothing can use `i0`:

```cpl
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}
```

`return;` is valid for `i0` functions.

## Prototypes

A prototype is a function declaration without a body:

```cpl
function add(i64 a, i64 b = 2) -> i64;

function add(i64 a, i64 b = 2) -> i64 {
    return a + b;
}
```

If a function has default arguments, keep the defaults consistent between the prototype and the definition.

## Default arguments

Default arguments are inserted at the call site when trailing arguments are omitted:

```cpl
function add(i64 a, i64 b = 2) -> i64 {
    return a + b;
}

start() {
    exit add(3) as u8; : exits with 5 :
}
```

A default argument cannot appear before a non-default argument:

```cpl
function bad(i32 a = 1, i32 b) -> i32; : invalid :
```

## Function overloading

CPL supports overloads by argument list:

```cpl
function put(i8 x) -> i0;
function put(i64 x) -> i0;
```

Return type alone is not enough to distinguish overloads:

```cpl
function foo() -> i32;
function foo() -> i0; : invalid overload set :
```

Use `as` to make overload selection explicit:

```cpl
put(1 as i8);
put(1 as i64);
```

Global and external functions are not a good place for overloads because their emitted symbols must be linkable by the outside toolchain.

## Generic functions

Generic functions use type parameters in angle brackets:

```cpl
function sum<U, T>(U a, T b) -> T {
    return a as T + b;
}

start() {
    exit sum<i8, u8>(1, 1);
}
```

Generic functions are instantiated for the selected type arguments. Generic type parameters may also be used inside local functions and lambdas declared within that generic function.

## Function pointers

A function can be stored in `ptr i0` or in a signature `ptr fn()i0` and called through that pointer:

```cpl
function inc(i64 a) -> i64 {
    return a + 1;
}

start() {
    ptr i0 f1         = inc;
    ptr fn(i64)i64 f2 = inc;
    exit (f1(4) + f2(4)) as u8;
}
```

Function pointers do not carry full signature information. When calling through `ptr i0`, the compiler cannot apply the same overload and default-argument checks that it applies to a direct function call.

## Local functions

Functions can be declared inside other functions:

```cpl
function outer() -> i64 {
    function inner(i64 x) -> i64 {
        return x + 1;
    }

    return inner(4);
}
```

Local functions do not capture local variables from the parent function. They are closer to named helper functions scoped inside another function.

## Lambda functions

Lambdas are lightweight local functions. They also do not capture outer local variables.

```cpl
start() {
    ptr i0 add = (i64 a, i64 b) => a + b;
    exit add(2, 3) as u8;
}
```

Block form:

```cpl
ptr fn(i64,i64)i64 logic = (i64 a, i64 b) => {
    a += b;
    return a + b;
};
```

## Variadic arguments and `poparg`

Use `...` as the final argument to declare a variadic function:

```cpl
function take(...) -> i0 {
    @[poparg] i64 first;
    @[poparg] i64 second;
}
```

`@[poparg]` can also read arguments in `start(...)`:

```cpl
start(...) {
    @[poparg] i64 argc;
    @[poparg] ptr ptr i8 argv;
    exit 0;
}
```

The current implementation is low-level: use this feature only when you understand the target calling convention.

## `syscall`

`syscall` invokes a target-dependent system call:

```cpl
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}
```

The example above uses the macOS x86-64 `write` syscall number. Linux uses different numbers and ABI rules.

## `asm`

`asm` inserts inline assembly into generated output. Values from the argument list are referenced with `%0`, `%1`, and so on:

```cpl
i64 input = 10;
i64 output;

asm(input, output) {
    "mov rax, %0",
    "add rax, 1",
    "mov %1, rax"
}
```

Inline assembly is target-specific and opaque to most compiler optimizations. Preserve registers and write assembly that matches the selected backend and assembler format.
