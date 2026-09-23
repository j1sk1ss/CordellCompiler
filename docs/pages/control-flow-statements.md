# Control flow statements

CPL control-flow syntax is C-like, but conditions are followed by `;`.

## `if` and `else`

```cpl
if condition; {
    statement;
}
else {
    statement;
}
```

Single-statement branches are also supported:

```cpl
start() {
    i64 x = 2;
    if x == 1; putc('A');
    else if x == 2; putc('B');
    else putc('C');
    exit 0;
}
```

## `while`

```cpl
while condition; {
    statement;
}

while condition; statement;
```

Example:

```cpl
function strlen(ptr i8 s) -> i64 {
    i64 l = 0;
    while dref s; {
        s += 1;
        l += 1;
    }
    return l;
}
```

## `loop`

`loop` creates an unconditional loop:

```cpl
loop {
    if done; break;
}
```

A counted loop can be created with `@[counter]`:

```cpl
@[counter(10)] loop {
    putc('x');
}
```

`@[counter]` accepts a constant.

## `switch`

```cpl
switch value; {
    case 1; {
        putc('A');
        break;
    }
    case 2; {
        putc('B');
        break;
    }
    default {
        putc('?');
        break;
    }
}
```

Cases fall through unless you use `break` or annotate the switch with `@[no_fall]`:

```cpl
@[no_fall]
switch code; {
    case 'A'; putc('A');
    case 'B'; putc('B');
    default   putc('?');
}
```

By default, `switch` is generated through a binary-search-style decision tree. Use `@[straight]` to request linear case selection.
