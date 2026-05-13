# Types

CPL does not support user-defined structures, classes, enums, or unions. The current language core consists of primitive types, pointers, strings, and arrays.

## Primitive types

| Type | Meaning |
|---|---|
| `i8`, `i16`, `i32`, `i64` | signed integers |
| `u8`, `u16`, `u32`, `u64` | unsigned integers |
| `f32`, `f64` | floating-point values |
| `i0` | void-like type, mainly for function return types and `ptr i0` |

Examples:

```cpl
i8  c = 'A';
u8  b = 0xFF;
i32 x = 10 as i32;
i64 y = 123456;
f64 z = 0.25;
```

## Static typing and casts

CPL is statically typed, but the compiler may insert implicit widening conversions. Narrowing conversions must be written explicitly with `as`.

```cpl
i64 wide = 10;
i8 narrow = wide as i8;
```

`as` is also useful when selecting an overloaded function:

```cpl
function put(i8 x) -> i0;
function put(i64 x) -> i0;

put(1 as i8);
```

## Boolean-like logic

CPL has no dedicated boolean type. Conditions use primitive values:

- `0` is false;
- any non-zero value is true.

## Pointers

Use `ptr` to declare a pointer, `ref` to take an address, and `dref` to read through a pointer.

```cpl
i32 value = 123;
ptr i32 p = ref value;
i32 copy = dref p;
```

Indexed pointer access is also supported:

```cpl
i32 first = p[0];
```

Pointers can be chained:

```cpl
ptr i32 p = ref value;
ptr ptr i32 pp = ref p;
```

`ptr i0` is commonly used for untyped pointers and function pointers.

## Strings

`str` allocates a mutable string object:

```cpl
str msg = "Hello";
```

A string literal by itself is stored as read-only data. Pass it to a `ptr i8` parameter with `ref`:

```cpl
function puts(ptr i8 s) -> i0;

puts(ref "Hello\n");
```

You can also take a pointer to a `str` object:

```cpl
str msg = "Hello\n";
puts(ref msg);
```

## Arrays

Arrays use the syntax `arr name[length, type]`.

```cpl
arr a[3, i8] = { 'A', 'B', 'C' };
i8 second = a[1];
```

Arrays can contain primitive types or pointers:

```cpl
arr row1[3, i32] = { 1, 2, 3 };
arr row2[3, i32] = { 4, 5, 6 };
arr rows[2, ptr i32] = { ref row1, ref row2 };

i32 x = rows[1][0];
```

Global and read-only arrays are placed into target-dependent sections. Local arrays are allocated in function-local storage.

## `glob`, `ro`, and `extern`

`glob` makes a symbol global:

```cpl
glob i32 counter = 0;
glob function exported() -> i0;
```

`ro` marks data as read-only:

```cpl
ro i32 answer = 42;
```

`extern` declares a symbol provided by another object or library:

```cpl
extern function puts(ptr i8 s) -> i32;
extern i32 errno;
```

External symbols must be linked correctly by the final toolchain.
