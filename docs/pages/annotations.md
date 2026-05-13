# Annotations

Annotations extend the small core syntax without adding many dedicated keywords. They are written before the construct they affect:

```cpl
@[entry("_main")]
function main() -> i0 {
    exit 0;
}
```

## Available annotations

| Annotation | Applies to | Meaning |
|---|---|---|
| `@[entry]`, `@[entry("name")]` | function | mark function as program entry point |
| `@[naked]` | function or `start` | suppress normal entry/exit routines |
| `@[align(N)]` | variable or array declaration | request memory alignment |
| `@[section("name")]` | global variable or function | place symbol into a section |
| `@[address(N)]` | function | request a fixed address where supported by backend/configuration |
| `@[no_fall]` | `switch` | insert implicit breaks for cases |
| `@[straight]` | `switch` | generate linear case selection |
| `@[counter(N)]` | `loop` | create a counted loop |
| `@[hot]` | branch/case | mark branch as hot for layout |
| `@[cold]` | branch/case | mark branch as cold for layout |
| `@[not_lazy]` | logical expression | evaluate both sides of `&&`/`\|\|` |
| `@[register(N)]` | local primitive variable | bind variable to a target register index |
| `@[poparg]` | local declaration | read the next function argument |

## Entry and naked

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
```

Use `@[naked]` only for code that fully controls its own prologue, epilogue, and exit behavior.

## Alignment and sections

```cpl
@[align(16)]
glob i32 value = 1;

@[section(".my_data")]
glob i32 other = 2;

@[section(".my_text")]
function helper() -> i0 {
    return;
}
```

Default sections are target/config dependent. The CLI exposes `--ro-section`, `--glob-section`, and `--code-section`.

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

`@[hot]` and `@[cold]` are hints for branch placement on an `if` statement:

```cpl
@[hot] if likely; {
    putc('H');
}
else {
    putc('C');
}
```

Use them as layout hints, not as semantic requirements.

## Counted loop

```cpl
@[counter(10)]
loop {
    putc('x');
}
```

The counter value must be constant.

## Register and poparg

`@[register(N)]` binds a local primitive variable to a target register index:

```cpl
#define RAX 0
@[register(RAX)] i64 value = 10;
```

`@[poparg]` reads arguments from the current call context:

```cpl
function take(...) -> i0 {
    @[poparg] i64 first;
}
```

Both annotations are low-level and target-sensitive.
