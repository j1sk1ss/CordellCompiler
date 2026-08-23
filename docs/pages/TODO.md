# TODO

## Complete strict (strong) typing! (Completed)
We need to complete the strong typing support in the compiler. TO do this, the compiler should allow to create function-types:

```cpl
ptr fn(i32,i32)i32 function;
```

Actually, I think it can be solved via a new type in the types' table which is linked to a function from the functions' table. Let's say it will create a new dummy function:

```cpl
function __cpl_custom_typed_function(i32 _, i32 _) -> i32; :/ ID: X /:
```

Then this function will be linked to a type. With this type we will be able to check whether a function has the same signature or not. Also, if we declare two pointers with the same type, the logic of name creation won't allow us to duplicate the template. The one problem here is scopes - It may create a new function if this is a new scope. </br>
The second idea is to store info in the type. Link a list of token types and the return type of a function. It will look like that:

```c
typedef struct {
    unsigned long long hash; // Fast search for an entry when we create a new one
    list_t             args;
    token_type_t       rtype;
} function_type_t;
```

In the HIR level it will be a `ptr i0` pointer with a linked type Id. This means it won't change anything and will allow the CSA be more precise with pointer functions. Also it may help with lambdas:

```cpl
ptr fn(i8,i8)i8 lambda = (i8 a, i8 b) => a + b;
exit lamda(1, 1);
```

## Containers (Completed)
The idea is to create structures but with some features. For instance, a container will have an ability of function holding with self argument support. It means, the CPL will support the next syntax:
```cpl
container storage {
    u32 wood  = 100;
    u32 steel = 100;
    u64 money;

    function sell_wood(self) -> i0 {
        self.wood -= 100;
        self.money += 100;
    }
}

start() {
    storage s;
    s.sell_wood();
}
```

In a nutshell, this code will be translated in the next IR:
```cpl
start() {
    u8* s = arrdecl(16);
    u32* tmp1 = s;
    *tmp1 = 100;
    u32* tmp2 = s + sizeof(u32);
    *tmp2 = 100;
    u64* tmp3 = s + sizeof(u32) + sizeof(u32);
    *tmp3 = 0;
    storage__sell_wood(s);
}

fn storage__sell_wood(u8* ptr) {
    u32* tmp1 = ptr;
    *tmp1 -= 100;
    u64* tmp2 = ptr + sizeof(u32);
    *tmp2 += 100;
}
```

This is the final feature that I want to add to the CPL. </br>
**Important note:** Overloads and generics must work the same, which means the next code must be valid as well:
```cpl
container generic {
    function sum<T, U>(self, ptr T a, U b) -> i0 {
        return;
    }
}

start() {
    generic g;
    g.sum<i8, i8>(1, 1);
}
```

*P.S.:* Generics won't support containers. No ```sum<generic, i0>()```, etc. </br>
*P.P.S.:* No nested containers:
```cpl
container a {
    container b {

    } :/ Illegal /:
}
``` 
*P.P.P.S.:* By the way, there is a way how you can use a container in a container:
```cpl
container a {
    ptr a next;
    ptr b next;
}
```

## Simple polymorphic system (Completed)
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

It should work well with the existing pipeline and overload system. It should not conflict with overloads because these features solve different problems. Generics are a convenient way to reuse the same logic for different types, while overloads make the same name available for different logic. Overloads could replace generics in some cases, but that would require writing a lot of duplicated code for each type. For instance:
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
