# Program entry point
Function becomes an entry point in two cases:
- If this is a `start` function.
- If this is an annotated with the `entry` annotation function.
- If the produced program linked with the GCC linker, the `main` function will became an entry point.

Example without a `start` function:
```cpl
function fang() -> i0 { return; }
@[entry]
function naomi() -> i0 { exit 0; } : <= Becomes an entry point :
```

Example with a `start` start function:
```cpl
start() { exit 0; } : <= Becomes an entry point :
function fang() -> i0; { }
function naomi() -> i0; { }
```

As you've noticed, the `start` keyword can have arguments (`start(i32 argc, ptr ptr i8 argv)`) and can ignore them (`start()`). The both cases are valid. Also, the `start` keyword can use variadic arguments, which says that the code below:
```cpl
start(...) {
    @[poparg] i32 argc;
    @[poparg] ptr ptr i8 argv;
}
```
, is valid.

**Note 1:** 'Start' function doesn't have a return type (you can't use the '->' modification) and requires usage of the 'exit' keyword instead of the 'return'. Also the maximum type that can be used as a value in the 'exit' keyword is the 'u8' type. </br>
**Note 2:** Actually, with usage of the entry annotation, we can set a return type:
```cpl
@[entry]
function main(i32 argc, ptr ptr i8 argv) -> u8;
```
**Note 3:** Entry point will generate all essential steps (stackframe allocation, entry, exit commands, etc).
**Note 4:** Entry point supports the `naked` annotation which disables default stack frame allocation and exit routine.
**Note 5:** Without any entry point, a file becomes a library file after the compilation.
