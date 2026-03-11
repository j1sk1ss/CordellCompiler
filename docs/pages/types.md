# Types
Now let's talk about the language basics. This language is a static-typed language (And I'm trying to make him strong-typed as well). That's why CPL supports a cast operation such as the `as` operation. Syntax is similar with the Rust-language cast operation `as`. </br>
For instance:
```cpl
i32 never = 10 as i32;
i32 dies  = 20 as i32;
u8 technoblade = (never + dies) as u8;
```

One note here: Actually, there is no reason to use this statement given an unavoidable implict cast. That means that the snippet above, without these `as` statements, anyway involves a cast operation. However, to support the static-typing, I'd recommend to use the `as` statement. </br>
P.S.: *Also, the `as` keyword is really useful in terms of function overloading functions usage. We will talk about this below.*

## Primitives
Now, when we've talked about the types system, let's discuss about types itself. Primitive type is a basic, supported by this language, data structure. This data structure can be represented as:
- `f64`, `f32` - Real / double and float; non-floating values are converted to double if used in double operations.
```cpl
f64 a = 0.01;
f32 b = 0.01;
i32 d = 1;
f64 c = a + b + d;
```

- `i64`, `u64` - Long / 64-bit value.
```cpl
i64 a = 123123123;
u64 b = 0b1110011;
i64 c = 0xFFFFFFF;
```

- `i32`, `u32` - Integer / 32-bit value.
```cpl
i32 a = 123123;
u32 b = 0b0111;
i32 c = 0xFFFF;
```

- `i16`, `u16` - Short / 16-bit value.
```cpl
i16 a = 123;
u16 b = 0b1;
i16 c = 0xF;
```

- `i8`, `u8` - Character / 8-bit value.
```cpl
i8 a = 255;
u8 b = 0b0;
i8 c = 0xF;
i8 d = 'a';
```

- `i0` - Void type. Must be used only in the function return type or as a pointer. This type isn't supported by the Compiler as a regular primitive type.
```cpl
function cordell() -> i0;
ptr i0 a = cordell;
```

`P.S.` The CPL doesn't support `booleans` itself. For this purpose you can use any `non-real` data type such as `i64`, `i32`, `u8`, `ptr i0` etc. The logic here is pretty simple:
- `Not a Zero` is a `true` value.
- `Zero` is a `false` value.

## Strings and arrays
- `str` - String data type. Similar to the `ptr u8` type, but it is used for the high-level inbuild operations such as compare, len, etc. (WIP).
```cpl
str msg = "Hello world!";
if msg == "Hello world!"; {
}
```

To be honest, the string object acts the same as it does the array object. In particular, the two declaration below are the same in terms of IR presentation:
```cpl
str msg1 = "Hello world!";
arr msg2[13, i8] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0' };
```

But in difference with an array, a string has a different behaviour when it becomes a pointer:
```cpl
ptr str msg1 = "Hello world!"; : <= Data allocated in the RO segment, pointer is set to 8 bytes :
str msg2 = "Hello world!";     : <= Data allocated in the stack                                 :
```

An array can't do the same thing. Array can allocate data in stack, but can't be used as a type of a pointer (you will need to use a primitive instead). That means, that string is somehitng between an array (non-primitive) and a primitive.

- `arr` - Array data type. Can contain any primitive type as an element type. Will allocate data in the stack or in a section (depends on the target architecture and annotations).
```cpl
arr Array_1d_1[10, i32]  = { 0 };
arr Array_1d_2[10, i32]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
arr Array_2d[2, ptr i32] = { ref Array_1d_1, ref Array_1d_2 };
i32 a = Array_2d[0][0]; : = 0 :
```

## Align
A declaration of a primitive or of an array type (an array or a string) can be annotated with the `align`. This annotation will add an additional padding during memory stack allocation.
```cpl
@[align(16)] glob i32 a;
start() {
    @[align(8)] i32 b;
    @[align(8)] i32 c = a;
}
```

By default align set to platform `bitness / 8` (For instance on the `gnu_x86_64` this is 8 bytes).

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

## Function pointers
A function can be stored as a pointer easilly with usage of the next code:
```cpl
function foo(i32 a) -> i32;
start() {
    ptr i0 a = foo;
    i32 res = a(100);
}
```

As it can be seen from the code above, 'pointer' functions don't have a signature. Such a disadvantage disables all efforts from static analysis, function overloading and default arguments. In a nutshell, it will make this:
```cpl
function foo(i32 a) -> i32;
function foo(u32 a = 10) -> i0;
function bar(i8 a = 'a');
start() {
    ptr i0 a = foo; : Will store function foo(i32 a) -> i32; given that this function is the top function :
    ptr i0 b = bar;
    b(); : Will cause an undefined behaviour given the lack of arguemnts :
}
```

However, such an ability can make it possible to use different functions in the same context. For instance:
```cpl
function min(i32 a, i32 b);
function max(i32 a, i32 b);
function logic(ptr i0 func, i32 a, i32 b) {
    func(a, b);
}

start() {
    logic(min, 10, 20);
    logic(max, 10, 20);
}
```

**Also** a function pointer can be not only a function (sounds weird, isn't?). It can be any pointer as well:
```cpl
(0x100 + 0xB045)(100); : <- Valid function call that will invoke a function at 0x100 + 0xB045 address :
i32 a = 123;
a(100 + 123); : <- Valid function call that will invoke any function (or not) at the 223 address :
```

## Section
A function and a global declaration can be placed in a specific section. To perform this, you will need to use the `section` annotation:
```cpl
    @[section(".lis")] glob i32 a;
    @[section(".lis")] function foo() {}
```

**Note 1:** Function prototype doesn't affected by a section. To put the function's code to a section, you need to define the function. </br>
**Note 2:** By default all global/read-only variables and functions are placed in the platform's code section from the configuration. </br>
**Note 3:** Local functions can't be placed in the specific section. They will stay with their parent function in the same section.
