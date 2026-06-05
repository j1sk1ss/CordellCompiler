# Annotations

Annotations extend the small core syntax without adding many dedicated keywords. They are written before the construct they affect:

```cpl
@[entry("_main")]
function main() -> i0 {
    exit 0;
}
```

The parser accepts the form `@[name]` and, for annotations that need a value, `@[name(value)]`. The value is read as the single token inside parentheses: numeric annotations convert it to an integer, while `entry`, `section`, and `inline` keep it as text.

## Available annotations

| Annotation | Applies to | Meaning |
|---|---|---|
| `@[entry]`, `@[entry("name")]` | function or `start` | mark the function as the program entry; without `name`, the configured entry symbol is used |
| `@[naked]` | function or `start` | suppress normal entry/exit routines |
| `@[section("name")]` | global variable, global array, function, or `start` | place the symbol into a named section |
| `@[nosection]` | global function | place the function into the configured no-section bucket |
| `@[align(N)]` | variable, array, or container | request memory/container alignment |
| `@[register(N)]` | variable declaration | bind the variable to a target register index |
| `@[poparg]` | variable declaration in a variadic context | read the next variadic argument into this declaration |
| `@[inline]` | function | increase the inliner preference |
| `@[inline(always)]` | function | force the inline decision toward always inline |
| `@[inline(never)]` | function | force the inline decision toward never inline |
| `@[inline(model)]` | function | use the model-based inline mode |
| `@[self]` | container function | mark the function as an explicit-self method for container call rewriting |
| `@[abi]` | function | mark the function as ABI-compatible |
| `@[weak]` | function | mark the function as a weak symbol |
| `@[like_c]` | container | use C-like field layout handling instead of the requested CPL alignment value |
| `@[no_fall]` | `switch` | make switch cases behave as if they end with `break` |
| `@[straight]` | `switch` | force linear switch selection |
| `@[counter(N)]` | `loop` | generate a counted loop |
| `@[hot]` | `if` | make the false branch cold for layout |
| `@[cold]` | `if` or switch `case` | make the true branch, or the annotated case, cold for layout |
| `@[not_lazy]` | logical expression | evaluate both sides of `&&` or `\|\|` |

The compiler also parses `@[address(N)]` and `@[union]`, but the current implementation does not connect their summary fields to code generation or container layout yet. Treat them as reserved/internal until that changes.

## Entry, naked, sections

```cpl
@[entry("_main")]
function main() -> i0 {
    exit 0;
}

@[naked]
start() {
    asm() {
        "ret"
    }
}

@[section(".my_text")]
function helper() -> i0 {
    return;
}
```

Use `@[naked]` only for code that fully controls its own prologue, epilogue, and exit behavior. Default sections are target/config dependent. The CLI exposes `--ro-section`, `--glob-section`, and `--code-section`.

`@[nosection]` is currently handled for functions:

```cpl
@[nosection] glob function tss_flush() -> i0 {
    asm() {
        "mov ax, 0x28",
        "ltr ax"
    }
}
```

## Data layout

```cpl
@[align(16)]
glob i32 value = 1;

@[section(".my_data")]
glob i32 other = 2;

@[align(1)]
container packed {
    i8  a;
    i16 b;
}

@[like_c]
container c_layout {
    i8  tag;
    i64 value;
}
```

`@[align(N)]` affects variables, arrays, and container field layout. `@[like_c]` is only read when defining a container.

## Function hints and symbols

```cpl
@[inline(always)]
function add(i64 a, i64 b) -> i64 {
    return a + b;
}

@[weak]
@[abi]
function external_hook() -> i0;
```

`@[inline]` without an option is a soft preference. Supported options are `always`, `never`, and `model`.

`@[abi]` and `@[weak]` are low-level symbol/interop flags used by the function table and backend path.

## Container self methods

Use `@[self]` when a container function should receive the object being called on. The function must declare an explicit first parameter for that receiver, usually `ptr <container> self`.

```cpl
container counter {
    i32 value;

    @[self]
    function add(ptr counter self, i32 delta) -> i0 {
        self.value += delta;
    }
}

start() {
    counter c;
    c.value = 10;
    c.add(7);

    exit c.value as u8;
}
```

The call `c.add(7)` is lowered as a normal function call where `ref c` is passed as the explicit `self` argument.

## Switch annotations

`@[no_fall]` removes the need to write `break` at the end of every case:

```cpl
@[no_fall]
switch code; {
    case 'A'; { putc('A'); }
    case 'B'; { putc('B'); }
    default  { putc('?'); }
}
```

`@[straight]` asks the compiler to use a linear search instead of the default binary-search-style generation:

```cpl
@[straight]
switch code; {
    case 1; { putc('1'); }
    default { putc('?'); }
}
```

## Branch layout

`@[hot]` and `@[cold]` are layout hints for `if` statements:

```cpl
@[hot] if likely; {
    putc('H');
}
else {
    putc('C');
}
```

On `if`, `@[hot]` makes the false branch cold; `@[cold]` makes the true branch cold. On `switch`, `@[cold]` can be attached to a case body.

## Counted loop

```cpl
@[counter(10)]
loop {
    putc('x');
}
```

The counter value must be constant.

## Logical evaluation

By default, logical operators are lazy. Attach `@[not_lazy]` to force both sides to be generated:

```cpl
if (@[not_lazy] left() && right()); {
    putc('y');
}
```

## Register and poparg

`@[register(N)]` binds a variable to a target register index:

```cpl
#define RAX 0
@[register(RAX)] i64 value = 10;
```

`@[poparg]` reads arguments from the current variadic call context:

```cpl
function take(...) -> i0 {
    @[poparg] i64 first;
    @[poparg] i64 second;
}

start(...) {
    @[poparg] i64 argc;
    @[poparg] ptr ptr i8 argv;
    exit 0;
}
```

Both annotations are low-level and target-sensitive.
