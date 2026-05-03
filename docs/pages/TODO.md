# TODO

## Simple polymorphic system
The idea is to create a placeholder type for local variables, then copy a function with the provided type. For instance let's consider the function below:
```cpl
function swap<T>(ptr T a, ptr T b) -> i0 {
    T tmp = dref a;
    dref a = dref b;
    dref b = tmp;
}
```

We have `T` as an unknown type. At the AST phase this type is considered as `genertic` type, which doesn't tell us anything. Also, important to note that we don't generate the IR for this function. But when we meet the next code:
```cpl
swap<u8>(ref a, ref b);
```

We:
- determine which type is used for the call
- find the called function AST node in symtable
- generate IR for the function with provided type

This will lead us to the next image:
```cpl
function swap(ptr u8 a, ptr u8 b) -> i0 {
    u8 tmp = dref a;
    dref a = dref b;
    dref b = tmp;
}
swap1(ref a, ref b);
```

It will work perfectly in the existed pipline given the overload system.
