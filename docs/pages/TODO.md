# TODO
## Simple polymorphic system (Ongoing)
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
function swap1(ptr u8 a, ptr u8 b) -> i0 {
    u8 tmp = dref a;
    dref a = dref b;
    dref b = tmp;
}
swap1(ref a, ref b);
```

It will work perfectly in the existed pipline given the overload system. It won't conflict with the overload system thougth, 'cause these options chaise two different goals. The generics are a convenient way for handling the same logic for different types, and the overloads are a way of making the same name avaliable for different logic. Indeed, the last one could replace the first, but it will require to create a ton of code for each type (regardless user-defined types as well) to reach the same functionality. For instance:
```cpl
function sum<T>(T a, T b) -> T {
    return a + b;
}
sum<u8>(10, 11);
sum<f64>(10.1, 20.0);
```

To implement the same code with overloads, you will need to create two versions of the 'sum':
```cpl
function sum(u8 a, u8 b) -> u8 {
    return a + b;
}
function sum(f64 a, f64 b) -> f64 {
    return a + b;
}
sum(10 as u8, 11 as u8);
sum(10.1, 20.0);
```

Meanwhile, the overloads are essential to solve the next problem:
```cpl
function sum(u8 a, u8 b) -> u8 {
    return a + b;
}
function sum(f64 a, f64 b) -> f64 {
    return a * b - a;
}
sum(10 as u8, 11 as u8);
sum(10.1, 20.0);
```

That's why 'generic' functions are essential and pretty useful in terms of system programming. They won't consume a lot time to implement, they won't overcomplex the syntax and they are optional. </br> 
P.S.: *Actually, the compiler needs its own user-types system to support this feature. At least allias system for user-defined named types, for instance: typedef a i32; etc.*