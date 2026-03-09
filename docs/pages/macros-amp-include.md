# Macros & include
The compiler includes a preprocessor that will take care about statements such as `#include`, `#define`, `#ifdef`, `#ifndef` and `#undef`. Most of them act similar to `C/C++`. For example, `#include` statement must be used only with a 'header' file. How to create a 'header' file? </br>
For example, we have a file with the implemented string function:
```cpl
{
    function strlen(ptr i8 s) -> i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }
}
```

This function is independent from others and can exist without any dependencies. But how to use this function in other files? We need to create a 'header' file:
```cpl
{
#ifndef STRING_H_
#define STRING_H_ 0
    : Get the size of the provided string
      Params
        - `s` - Input string.

      Returns the size (i64). :
    function strlen(ptr i8 s) -> i64;
#endif
}
``` 

This header file includes only the prototype and guards.
