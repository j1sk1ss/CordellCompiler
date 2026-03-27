# CPL Keywords
This is a summary information about all avaliable keywords in CPL. For further information, check related pages of documentation.

| Keyword | Desciption | Example |
|-|-|-|
| `extern` | Add a symbol from the environment. Allows to add any variable and function if they are linked correctly to the compiled file. | <pre><code> extern printf(ptr i8 fmt, ...); </br> extern ptr i8 BUFFER; </code></pre> |
| `start` | Static entry function. This is the point where the program starts. In compiler terms, this function will get a name which is recognized by a linker as an entry point. | <pre><code> start() { </br> } </code></pre> |
| `exit` | Static entry exit. The exit keyword is essential in the body of the entry point and requires a 8-bit exit code (from 0 up to 255). | <pre><code> exit 123; </code></pre> |
| `function` | Define a new function or a prototype. | <pre><code> function caulfield() -> i0; </br> function caulfield() -> i0 { </br> } </code></pre> |
| `return` | Return value from a function. This keyword accepts any value and can be used without any value at all. | <pre><code> return 1; </br> return; |
| `switch` | Switch keyword can be usefull in cases where we have a lot of different options. Will work faster than a lot of `if-else if-else`, but slower than small `if-else`. | <pre><code> switch a; { </br> case 1; { </br> } </br> default { </br> } </br> } </code></pre> |
| `case` | Case option in the `switch` keyword. Can accept only primitive values | <pre><code> case 1; { </br> } </code></pre> |
| `default` | Default option in the `switch` keyword. Doesn't accept any values. | <pre><code> default { </br> } </code></pre> |
| `while` | Conditional loop. In CPL where no `for` keyword is present, it the only one option to perform conditional loop. | <pre><code> while a - b < 10; { </br> } </code></pre> |
| `loop` | Endless loop. A little bit faster than the `while` keyword given the ignored comparision of the condition. | <pre><code> loop { </br> } </code></pre> |
| `break` | A keyword which breaks cases and loops. | <pre><code> loop { </br> break; </br> } </br> case 1; { </br> break; </br> } </code></pre> |
| `if` | Conditional operator. | <pre><code> if 1; { </br> } </br> else if 2; { </br> } </br> else { </br> } </code></pre> |
| `else` | The `if`s else option. | <pre><code> if 1; { </br> } </br> else { </br> } </code></pre> |
| `syscall` | Depends on the target platform. Will invoke a system call with the provided arguments. | <pre><code> syscall(1, 0x5, ref msg, strlen(ref msg)); </code></pre> |
| `asm` | Depends on the target platform. Inline assembly will copy all content to the final assembly file. Also, it will insert all used variable to the final assembly block. | <pre><code> asm(a, b) { </br> "mov rax, %0", </br> "mov %1, rdx" </br> } </code></pre> |
| `dref` | Get value by a pointer. | <pre><code> i32 a = dref 0x100 as ptr i32; </code></pre> |
| `ref` | Get a pointer to a value. | <pre><code> ptr i32 a = ref b; </code></pre> |
| `ptr` | A pointer modifier. | <pre><code> ptr i32 a; </code></pre> |
| `ro` | A read-only modifier. | <pre><code> ro i32 a; </code></pre> |
| `glob` | A global modifier. | <pre><code> glob function foo(); </br> glob i32 a; </code></pre> |
| `not` | Get a negative value from a value. Is is a high abstacted operator which doesn't just invert bits in the provided value. It returns 0 if the source value wasn't equals to 0, otherwise will return 1. | <pre><code> i32 a = not 1; </br> i32 b = not 0; </code></pre> |
| `sizeof` | Get the size of an object | <pre><code> arr a[10, i32]; </br> i32 size = sizeof(a); </code></pre> |
| `i8` | Signed 8-bit variable | <pre><code> i8 a = 'A'; </code></pre> |
| `u8` | Unsigned 8-bit variable | <pre><code> u8 a = 0xFF; </code></pre> |
| `i16` | Signed 16-bit variable | <pre><code> i16 a = 0xFFF; </code></pre> |
| `u16` | Unsigned 16-bit variable | <pre><code> u16 a = 0xFFF; </code></pre> |
| `i32` | Signed 32-bit variable | <pre><code> i32 a = 0xFFF; </code></pre> |
| `u32` | Unsigned 32-bit variable | <pre><code> u32 a = 0xFFF; </code></pre> |
| `f32` | Float 32-bit variable | <pre><code> f32 a = 0.01; </code></pre> |
| `i64` | Signed 64-bit variable | <pre><code> i64 a = 0xFFFFF; </code></pre> |
| `u64` | Unsigned 64-bit variable | <pre><code> u64 a = 0xFFFFF; </code></pre> |
| `f64` | Float 64-bit variable | <pre><code> f64 a = 0.0000001; </code></pre> |
| `str` | Wrapper for a pointer to a string | <pre><code> str msg = "Hello world!"; </code></pre> |
| `arr` | Array type | <pre><code> arr a[10, i32] = { 0, 1 }; </code></pre> |
| `as` | Explicit convertion from one type to another. | <pre><code> i8 a = 10 as i8; </code></pre> |
