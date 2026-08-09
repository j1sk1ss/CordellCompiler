# Playground
Write CPL code, run it through the compiler backend, and inspect the program output.
In local/static builds the page uses the same request contract and falls back to a
small backend stub until the production runner is connected.

```cpl-run
start() {
    syscall(1, 1, ref "Hello, World!\n", 14);
    exit 0;
}
```
