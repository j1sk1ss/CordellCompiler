# Debugging
The compiler produces a binary file (or an ASM file, if the `--emit-asm` flag is present). To debug the produced binary, you can use the `-d` flag which will include `DWARF` information, and the `LiS` keyword.

## Interrupt point 
Code can be interrupted (use `gdb`/`lldb`) with `lis` keyword. Example below:
```cpl
{
    start() {
        i32 a = 10;
        lis "Debug stop"; : <- Will interrupt execution here :
        exit 0;
    }
}
```

**Note 1!:** Be sure that you've disabled all optimizations before the code debug given the preservation of a code from transformation.
**Note 2!:** To make this works, use any debugging tool such as `gdb` and `lldb`.
