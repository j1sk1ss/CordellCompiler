# Debugging
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
