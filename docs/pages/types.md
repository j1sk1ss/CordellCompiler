# Types
This language **DOES NOT** support defined-types such as structures, classes, enums, unions and etc. Actually, it is a part of the experiment, where I'm trying to find the maximum that can perform a language without these abstract constructions. I know that it doesn't make any scence in terms of system programming where almost every driver or data-structure is presented as a 'structure'. </br>
I don't claim that this language doesn't have anything usefull for data managment, and we can't write some sort of drivers or memory managers. Actually, we can, but before the further discussion, let's talk about the basics.

## Static-Typing
This language is a **static-typed** language (And it's on the way of becoming strong-typed as well). To maintain the type consistency in a code, there is the only one way: use cast operations such as the `as` operation. Syntax is similar with the Rust-language cast operation `as`. </br>
For instance:
```cpl
i32 never = 10 as i32;                 : The number itself is a 64-bit wide type (on x64 systems), :
i32 dies  = 20 as i32;                 : that's why we have to narrow it exactly to 32-bits        :
u8 technoblade = (never + dies) as u8; : And here we narow again the product of the expression     :
```

P.S.: *Actually, in this version of the compiler (v3.4 and lower) there is no reasonable reason to use this statement given the unavoidable implict cast. This means that the snippet above, without these `as` statements, anyway involves cast operations. However, to support the static-typing, I'd anyway recommend to use the `as` statement.* </br>
P.S.S.: *Also, the `as` keyword is really useful in terms of function overloading functions usage. We will talk about this in the related section.*

## Primitives
Now, when we've talked about the types system and how to maintain the static-typing, let's discuss about the types itself. A primitive type, in term of the compiler (and most of the compilers as well), is a basic data structure which holds some sort of data. It can handle from 8-bits up to 64-bits (depends on the target platform) of data, and can be treated as the next types:

| Name | Description | Example |
|-|-|-|
| `f64`, `f32` | Real / double and float; non-floating values are converted to double if used in double operations. | <pre><code> f64 a = 0.01; </br> f32 b = 0.01; </code></pre> |
| `i64`, `u64` | Long / 64-bit value. | <pre><code> i64 a = 123321123; </br> u64 b = 0b11111111111111; </code></pre> |
| `i32`, `u32` | Integer / 32-bit value. | <pre><code> i32a = 123321; </br> i32 b = 0xFFFF; </code></pre> |
| `i16`, `u16` | Short / 16-bit value. | <pre><code> i16 a = 12332; </br> u16 b = 0xFFF; </code></pre> |
| `i8`, `u8` | Character / 8-bit value. | <pre><code> i8 a = 'A'; </br> u8 b = 0xFF; </code></pre> |
| `i0` | Void type. Must be used only in the function return type or as a pointer. This type isn't supported by the Compiler as a regular primitive type. | <pre><code> function foo() -> i0; </br> ptr i0 = foo; </code></pre> |

`P.S.` The CPL doesn't support `booleans` itself. For this purpose you can use any `non-real` data type such as `i64`, `i32`, `u8`, `ptr i0` etc. The logic here is pretty simple:
- `Not a Zero` is a `true` value.
- `Zero` is a `false` value.

## Strings and arrays
Here is the only types that are not the primitive types. These structures represent continues data with the pre-defined size, and while most languages stop on the 'array' type, CPL has the 'string' type for convenient work with strings. The reason why we need independent strings is simple - a lot of tasks force us to work with strings, and if we remember, for instance, how this pipline of development proceeds in C, we can find ourselves in need of a new approach. </br>

### String
- `str` - String data type. Acts the same as the `ptr i8` type, but it's used for the high-level in-built operations such as compare, len, etc. [TODO: In the v3.4 there is still no in-built operations, which means, a programmer can safely use `ptr i8` instead].
To declare a string, you need to use the `str` keyword:
```cpl
str msg = "Hello world!";
```
This will allocate data on the stack and place a terminator at the string's end. For instance, the code above will allocate exactly 13 bytes with the `\0` character on the 12 index. </br>

### How string works?
To be honest, a string object acts the same as it does an array object. In particular, the two declarations below are the same in terms of IR presentation:
```cpl
str msg1 = "Hello world!";
arr msg2[13, i8] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
```

IR representation of these declarations almost the same:
```
msg1 = str_alloc("Hello world!");
msg2 = arr_alloc(13);
put_data(msg2, { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' });
```

But in difference with an array, a string has a different behaviour when it becomes a pointer:
```cpl
ptr str msg1 = "Hello world!"; : <= Data allocated in the RO segment, pointer is set to 8 bytes :
str msg2 = "Hello world!";     : <= Data allocated in the stack                                 :
```

An array can't do the same thing. Array can allocate data on the stack, but can't be used as a type of a pointer (you will need to use a primitive instead). That means, that a string is something between an array (non-primitive) and a primitive.

### Array
- `arr` - Array data type. Can contain any primitive type as an element type. Will allocate data on the stack or in a section (depends on the target architecture and annotations).
```cpl
arr Array_1d_1[10, i32]  = { 0 };
arr Array_1d_2[10, i32]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
arr Array_2d[2, ptr i32] = { ref Array_1d_1, ref Array_1d_2 };
i32 a = Array_2d[0][0]; : = 0 :
```

## Pointers
- `ptr` - Pointer modifier that can be add to every primitive (and `str`, `arr`) type.
```cpl
i32 f = 10;
ptr i32 a = ref f;
ptr ptr a_ref = ref a;
ptr str b = "Hello world";
```

**Note:** Pointer has a maximum possible size in the target architecture. For example, in the `x86_64_gnu_nasm`, the `ptr i8` will have the `64-bit` size.

## How to deal with pointers?
Actually, it isn't too hard. This language supports two main commands to make pointers and work with values from these pointers. For example, we have a variable:
```cpl
i32 a = 123;
```

### ref
To obtain a reference link to this variable, we must use the `ref` statement:
```cpl
i32 a = 123;
ptr i32 a_ptr = ref a;

: C alternative is
int a = 123;
int* a_ptr = &a;
:
```

### dref
Similar to C-language, we can 'dereference' a pointer. To perform this, we need to use the `dref` statement:
```cpl
i32 b = dref a_ptr;
: int b = *a_ptr; :
```

Additionally, obtaining of a dereferenced value from a pointer can be performed via an indexing operation:
```cpl
i32 b = a_ptr[0];
: int b = a_ptr[0] :
``` 
