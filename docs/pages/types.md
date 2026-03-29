# Types
CPL **does not** support user-defined types such as structures, classes, enums, unions, and similar abstractions. This is an intentional part of the experiment: the goal is to see how far a language can go without those constructs.

That limitation doesn't mean the language is useless for low-level work. It simply means the language stays focused on a smaller core. Before discussing arrays, strings, and pointers, it is better to start with the type system itself.

## Static typing
CPL is a **permissively statically typed** language. It is not fully strong-typed, but it still requires explicit control over type changes.

The core rule is simple:
- **Widening conversions** may be inserted implicitly by the compiler.
- **Narrowing conversions are never implicit and are allowed only with the `as` keyword.**

The `as` syntax is similar to Rust:
```cpl
i32 never = 10 as i32;                 : The literal is i64 by default on x64 targets.      :
i32 dies  = 20 as i32;                 : Narrowing to 32 bits must be explicit.             :
u8 technoblade = (never + dies) as u8; : Narrowing the result of an expression is explicit. :
```

Notes:
- The compiler may insert an implicit cast only when it is a widening conversion.
- Otherwise, compilation should fail unless you write the cast explicitly with `as`.
- The `as` keyword is also useful when selecting an overloaded function.
- By default, numeric literals have the type depends on its value, so assigning them to smaller integer types usually requires `as`.

## Primitive types
A primitive type, in compiler terms, is a basic data object that stores a value directly. Depending on the target platform, primitive values can occupy from 8 bits up to 64 bits.

| Name | Description | Example |
|---|---|---|
| `f64`, `f32` | Real types. Non-floating values are converted to floating-point values when used in floating-point expressions. | <pre><code> f64 a = 0.01; </br>f32 b = 0.01; </code></pre> |
| `i64`, `u64` | Signed and unsigned 64-bit integers. | <pre><code>i64 a = 123321123; </br>u64 b = 0b11111111111111;</code></pre> |
| `i32`, `u32` | Signed and unsigned 32-bit integers. | <pre><code>i32 a = 123321; </br>u32 b = 0xFFFF;</code></pre> |
| `i16`, `u16` | Signed and unsigned 16-bit integers. | <pre><code>i16 a = 12332; </br>u16 b = 0x0FFF;</code></pre> |
| `i8`, `u8` | Signed and unsigned 8-bit integers. | <pre><code>i8 a = 'A'; </br>u8 b = 0xFF;</code></pre> |
| `i0` | Void type. It is intended for function return types and `ptr i0`. It is not a regular value type. | <pre><code>function foo() -> i0; </br>ptr i0 p = foo;</code></pre> |

## Boolean-like logic
CPL does not provide a dedicated boolean type. For boolean-like logic, use a non-floating primitive type such as `i64`, `i32`, `u8`, or `ptr i0`.

The convention is simple:
- `0` means `false`
- any non-zero value means `true`

## Strings and arrays
The main non-primitive data containers in CPL are **strings** and **arrays**. Both represent contiguous data with a predefined size.

### `str`
`str` is the built-in string type. In practice, it behaves similarly to `arr [n, i8]`.

```cpl
str msg = "Hello world!";
```

This allocates the string on the stack and appends the terminating `\0` byte. In the example above, the object occupies 13 bytes.

### How `str` works
A string object behaves similarly to an array object. The declarations below are almost equivalent at the IR level:

```cpl
str msg1 = "Hello world!";
arr msg2[13, i8] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
```

Approximate IR form:
```text
msg1 = str_alloc("Hello world!");
msg2 = arr_alloc(13);
put_data(msg2, { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' });
```

The important difference appears when a string is used through a pointer:
```cpl
ptr str msg1 = "Hello world!"; : <= Data is placed in the RO segment, pointer size is target-dependent :
str msg2 = "Hello world!";     : <= Data is allocated on the stack                                  :
```

An array cannot behave exactly the same way. Arrays can allocate data on the stack, but they are not used as pointer element types in the same sense as `str`. In that sense, a string sits somewhere between an array and a primitive convenience type.

### `arr`
`arr` is the array type. It can contain any primitive element type and is allocated either on the stack or in a target-dependent section.

```cpl
arr Array_1d_1[10, i32]  = { 0 };
arr Array_1d_2[10, i32]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
arr Array_2d[2, ptr i32] = { ref Array_1d_1, ref Array_1d_2 };
i32 a = Array_2d[0][0]; : = 0 :
```

## Pointers
`ptr` is a pointer modifier that can be applied to primitive types, `str`, and `arr`.

```cpl
i32 f = 10;
ptr i32 a = ref f;
ptr ptr a_ref = ref a;
ptr str b = "Hello world";
```

Pointer size depends on the target architecture. For example, on `x86_64_gnu_nasm`, `ptr i8` is 64 bits wide.

## Working with pointers
CPL provides two main operations for pointer work: `ref` and `dref`.

### `ref`
Use `ref` to obtain a pointer to an existing value:

```cpl
i32 a = 123;
ptr i32 a_ptr = ref a;

: C alternative:
int a = 123;
int* a_ptr = &a;
:
```

### `dref`
Use `dref` to read the value behind a pointer:

```cpl
i32 b = dref a_ptr;
: int b = *a_ptr; :
```

An indexed form may also be used:
```cpl
i32 b = a_ptr[0];
: int b = a_ptr[0]; :
```
