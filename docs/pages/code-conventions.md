# Code conventions

The compiler **DOES NOT** require a particular style, but the following conventions keep CPL code easy to read. I **DO NOT** force you to obey the rules, **BUT** strongly encourage you to stick with these rules.

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

Prefer `K&R`-style braces:

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

## Entry point

Prefere `start` over `@[entry]` functions:

```cpl
start() {
    exit 0;
}

:/ Don't use:
@[entry] function main() -> u8 {
    exit 0u8;
}
/:
```

Use `@[entry]` only if you really need this.

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
