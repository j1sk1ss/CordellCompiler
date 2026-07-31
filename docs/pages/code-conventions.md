# Code conventions

The compiler **does not** require a particular style, but the following conventions keep CPL code easy to read.

## Names

Use lowercase with underscores for variables and functions:

```cpl
i32 counter = 0;
ptr i32 data_ptr = ref counter;

function calculate_sum(ptr i32 data, i64 length) -> i32 {
    return 0;
}
```

Use uppercase with underscores for constants and macro-like values:

```cpl
#define BUFFER_SIZE 256
glob ro i32 WIN_X = 1080;
glob ro i32 WIN_Y = 1920;
```

Use a leading underscore for private helper functions:

```cpl
function _helper() -> i0 {
    return;
}
```

## Layout

Prefer K&R-style braces:

```cpl
function foo() -> i0 {
    if ready; {
        return;
    }
    else {
        return;
    }
}
```

Use the required semicolon after conditions:

```cpl
while i < n; {
    i += 1;
}
```

## Entry point

Use `start` for small programs:

```cpl
start() {
    exit 0;
}
```

Use `@[entry]` when you need a normal function declaration as the entry point:

```cpl
@[entry("_main")]
function main(i64 argc, ptr ptr i8 argv) -> i0 {
    exit 0;
}
```

## Headers

Use `_h.cpl` for prototype/header files:

```text
print_h.cpl  <- prototypes and include guards
print.cpl    <- implementation
```

Header guard example:

```cpl
#ifndef PRINT_H_
#define PRINT_H_ 0
function print(ptr i8 s) -> i0;
#endif
```

## Comments

CPL comments are colon-based:

```cpl
: one-line comment :

:/ block comment
   across several lines /:
```

Always close comments explicitly.
