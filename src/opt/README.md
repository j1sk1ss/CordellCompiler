# Optimization
## Navigation
- [Dead functions elimination](#dead-functions-elimination)
- [String declaration](#string-declaration)
- [Variable inline](#variable-inline)
- [Constant folding](#constant-folding)
- [Conditional branches unroll](#conditional-branches-unroll)
- [Dead scope elimination](#dead-scope-elimination)
- [Stack reusage](#stack-reusage)

## Dead functions elimination
- [deadfunc](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadfunc.c) - The first optimization algorithm. It removes unused functions by iterating through all input files, registering referenced functions, and deleting all unreferenced ones.

## String declaration
- [strdecl](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/strdecl.c) - The second optimization algorithm handles read-only strings. If the user declares strings with `ro` or `glob` flags, this algorithm allocates them in the `.data` or `.rodata` section instead of the stack. Example:
```CPL
{
    start {
        glob str fstring = "Global";
        ro str sstring = "ReadOnly";
        ptr str tstring = "ptrstring";
        exit 1;
    }
}
```

The `ptr` keyword signals that this is a pointer to the string `"ptrstring"`. Therefore, the data is placed into the `.rodata` section.

## Variable inline
- [varinline](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/varinline.c) - The third optimization works together with the fourth one. It implements variable inlining and constant propagation. If the value of a variable is known, it replaces all occurrences with the value and removes the declaration. Example:
```CPL
{
    start {
        int a = 10;
        int b = 11;
        int c = a;
        exit c + b;
    }
}
```

Will be transformed into:
```CPL
{
    start {
        exit 10 + 11;
    }
}
```

## Constant folding
- [constopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/constopt.c) - The fourth optimization is always executed after the third one and continues constant propagation by folding constant expressions. Example:
```CPL
{
    start {
        exit 10 + 11;
    }
}
```

Becomes:
```CPL
{
    start {
        exit 21;
    }
}
```

## Conditional branches unroll
- [condunroll](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/condunroll.c) - The fifth optimization works with `switch`, `if`, and `while` statements, similarly to funcopt. Its main goal is to remove unreachable code. For example, code inside `while (0)` is never executed. Example:
```CPL
{
    start {
        if 1; {
            int a = 10;
        }
        else {
            exit 0;
        }

        switch 10; {
            case 10; {
                exit 1;
            }
            default {
                short k = 10 - 1;
            }
        }

        exit 1;
    }
}
```

Will be simplified to:
```CPL
{
    start {
        {
            int a = 10;
        }

        {
            exit 1;
        }
        
        exit 1;
    }
}
```

## Dead scope elimination
- [deadscope](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadscope.c) - This optimization cleans the `AST` from dead scopes. A dead scope is one that does not affect the environment (does not invoke functions and does not modify variables from outer scopes). Example:
```CPL
{
    start {
        {
            int a = 10;
        }

        {
            exit 1;
        }
        
        exit 1;
    }
}
```

After eliminating dead scopes:
```CPL
{
    start {
        {
            exit 1;
        }
        
        exit 1;
    }
}
```

## Stack reusage
- [offsetopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/offsetopt.c) - The seventh optimization finalizes the previous ones by recalculating local and global variable/array offsets. It also implements stack reuse by reassigning memory slots of variables that are no longer `live`. Example:
```CPL
{
    start {
        int a = 0;  : offset 8 :
        int b = 10; : offset 8, since variable a is never used below :

        {
            short k = 1;     : offset 16 :
            short g = 1 + k; : offset 24 :
        }

        exit b;
    }
}
```