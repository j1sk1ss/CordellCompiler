# Hello, World! example

The smallest useful CPL program has an entry point and exits explicitly:

```cpl
start() {
    exit 0;
}
```

To print a character on macOS x86-64, call the platform `write` syscall directly:

```cpl
start() {
    i8 c = 'S';
    syscall(0x2000004, 1, ref c, 1);
    exit 0;
}
```

`syscall` is target-dependent: syscall numbers and ABI details differ between macOS and Linux.

## A small print helper

```cpl
function strlen(ptr i8 s) -> i64 {
    i64 l = 0;
    while dref s; {
        s += 1;
        l += 1;
    }

    return l;
}

function puts(ptr i8 s) -> i0 {
    syscall(0x2000004, 1, s, strlen(s));
}

start() {
    puts(ref "Hello, World!\n");
    exit 0;
}
```

String literals are stored as read-only byte sequences. Pass them to `ptr i8` parameters with `ref`.

## Entry annotation form

Instead of `start`, a normal function can be marked as the program entry:

```cpl
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

@[entry("_main")]
function main() -> i0 {
    putc('E');
    exit 0;
}
```

An entry function should use `exit`, not `return`, to finish the process.
