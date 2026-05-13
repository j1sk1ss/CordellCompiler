# Debugging

The compiler can emit intermediate files and can enable its debug compilation flag.

```bash
./builds/ccompiler --debug --emit-ast --emit-ir --emit-asm main.cpl
```

Generated files:

- `output.ast`
- `output.ir`
- `output.s`

For debugging generated programs, prefer `-O0` so the generated code stays closer to the source.

## `lis` breakpoint

The tokenizer recognizes `lis` as a breakpoint statement. It can be used as an intentional interrupt point for debugger-oriented experiments:

```cpl
start() {
    i32 a = 10;
    lis "Debug stop";
    exit 0;
}
```

Use a native debugger such as `gdb` or `lldb` on the produced executable. Inline assembly, optimization passes, and target ABI details can make source-to-assembly correspondence less direct.
