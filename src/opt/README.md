# Optimization
## Navigation
- [Dead functions elimination](#dead-functions-elimination)
- [String declaration](#string-declaration)
- [Variable inline](#variable-inline)
- [Constant folding](#constant-folding)
- [Conditional branches unroll](#conditional-branches-unroll)
- [Dead scope elimination](#dead-scope-elimination)
- [Stack reusage](#stack-reusage)
- [Example](#example)

## Dead functions elimination
- [deadfunc](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadfunc.c) - The first optimization algorithm. It removes unused functions by iterating through all input files, registering referenced functions, and deleting all unreferenced ones.

## String declaration
- [strdecl](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/strdecl.c) - The second optimization algorithm handles read-only strings. If the user declares strings with `ro` or `glob` flags, this algorithm allocates them in the `.data` or `.rodata` section instead of the stack. Example:
```CPL
{
    start() {
        glob str fstring = "Global";
        ro str sstring   = "ReadOnly";
        ptr str tstring  = "ptrstring";
        exit 1;
    }
}
```

The `ptr` keyword signals that this is a pointer to the string `"ptrstring"`. Therefore, the data is placed into the `.rodata` section.

## Variable inline
- [varinline](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/varinline.c) - The third optimization works together with the fourth one. It implements variable inlining and constant propagation. If the value of a variable is known, it replaces all occurrences with the value and removes the declaration. Example:
```CPL
{
    start() {
        i32 a = 10;
        i32 b = 11;
        i32 c = a;
        exit c + b;
    }
}
```

Will be transformed into:
```CPL
{
    start() {
        exit 10 + 11;
    }
}
```

## Constant folding
- [constopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/constopt.c) - The fourth optimization is always executed after the third one and continues constant propagation by folding constant expressions. Example:
```CPL
{
    start() {
        exit 10 + 11;
    }
}
```

Becomes:
```CPL
{
    start() {
        exit 21;
    }
}
```

## Conditional branches unroll
- [condunroll](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/condunroll.c) - The fifth optimization works with `switch`, `if`, and `while` statements, similarly to funcopt. Its main goal is to remove unreachable code. For example, code inside `while (0)` is never executed. Example:
```CPL
{
    start() {
        if 1; {
            i32 a = 10;
        }
        else {
            exit 0;
        }

        switch 10; {
            case 10; {
                exit 1;
            }
            default {
                i16 k = 10 - 1;
            }
        }

        exit 1;
    }
}
```

Will be simplified to:
```CPL
{
    start() {
        {
            i32 a = 10;
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
    start() {
        {
            i32 a = 10;
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
    start() {
        {
            exit 1;
        }
        
        exit 1;
    }
}
```

## Dead variables elimination
- [deadvar](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadvar.c)

## Stack reusage
- [offsetopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/offsetopt.c) - The seventh optimization finalizes the previous ones by recalculating local and global variable/array offsets. It also implements stack reuse by reassigning memory slots of variables that are no longer `live`. Example:
```CPL
{
    start() {
        i32 a = 0;  : offset 8 :
        i32 b = 10; : offset 8, since variable a is never used below :

        {
            i16 k = 1;     : offset 16 :
            i16 g = 1 + k; : offset 24 :
        }

        exit b;
    }
}
```

## Example
For visual example, let's optimize this code:
- Source.cpl
```CPL
0  {
1      from "string.cpl" import strlen;
2      extern exfunc printf;
3      function putc(ptr i8 str) {
4          return syscall(1, 1, string, strlen(string));
5      }
6      start() {
7          printf("Hello from program!");
8          if 1; {
9              i32 a = 10;
10             a = a + 20;
11         }
12         else {
13             i32 b = 20;
14             i32 c = b + 10;
15         }
16 
17         i32 a = 5;
18         while a > 0; {
19             a = a * 8;
20         }
21 
22         i32 c = 0;
23         while 0; {
24             c = c + 1;
25         }
26         else {
27             c = 100;
28         }
29 
30         i8 d = 0;
31         switch d; {
32             case 10; {
33                 i32 b = 10;
34                 b = b + 10;
35             }
36             case 11; {
37                 i32 y = 10;
38                 y = y + 10;
39             }
40             default {
41                 i32 k = 10;
42                 k = k + 10;
43             }
44         }
45         
46         exit d;
47     }
48 }
```

- DeadFunc.cpl (`import` and `putc` was removed)
```CPL
0  {
2      extern exfunc printf;
6      start() {
7          printf("Hello from program!");
8          if 1; {
9              i32 a = 10;
10             a = a + 20;
11         }
12         else {
13             i32 b = 20;
14             i32 c = b + 10;
15         }
16 
17         i32 a = 5;
18         while a > 0; {
19             a = a * 8;
20         }
21 
22         i32 c = 0;
23         while 0; {
24             c = c + 1;
25         }
26         else {
27             c = 100;
28         }
29 
30         i8 d = 0;
31         switch d; {
32             case 10; {
33                 i32 b = 10;
34                 b = b + 10;
35             }
36             case 11; {
37                 i32 y = 10;
38                 y = y + 10;
39             }
40             default {
41                 i32 k = 10;
42                 k = k + 10;
43             }
44         }
45         
46         exit d;
47     }
48 }
```

- StringDecl.cpl (`"Hello from program!"` moved to global `str_0` variable)
```CPL
0  {
2      extern exfunc printf;
+      glob str str_0 = "Hello from program!";
6      start() {
7          printf(str_0);
8          if 1; {
9              i32 a = 10;
10             a = a + 20;
11         }
12         else {
13             i32 b = 20;
14             i32 c = b + 10;
15         }
16 
17         i32 a = 5;
18         while a > 0; {
19             a = a * 8;
20         }
21 
22         i32 c = 0;
23         while 0; {
24             c = c + 1;
25         }
26         else {
27             c = 100;
28         }
29 
30         i8 d = 0;
31         switch d; {
32             case 10; {
33                 i32 b = 10;
34                 b = b + 10;
35             }
36             case 11; {
37                 i32 y = 10;
38                 y = y + 10;
39             }
40             default {
41                 i32 k = 10;
42                 k = k + 10;
43             }
44         }
45         
46         exit d;
47     }
48 }
```

- VarInline.cpl (`b` from `i32 c = b + 10;`(14) replaced by value. Result: `i32 c = 20 + 10;`. Same situation with `d` and `exit d;`(46))
```CPL
0  {
2      extern exfunc printf;
+      glob str str_0 = "Hello from program!";
6      start() {
7          printf(str_0);
8          if 1; {
9              i32 a = 10;
10             a = a + 20;
11         }
12         else {
14             i32 c = 20 + 10;
15         }
16 
17         i32 a = 5;
18         while a > 0; {
19             a = a * 8;
20         }
21 
22         i32 c = 0;
23         while 0; {
24             c = c + 1;
25         }
26         else {
27             c = 100;
28         }
29 
31         switch 0; {
32             case 10; {
33                 i32 b = 10;
34                 b = b + 10;
35             }
36             case 11; {
37                 i32 y = 10;
38                 y = y + 10;
39             }
40             default {
41                 i32 k = 10;
42                 k = k + 10;
43             }
44         }
45         
46         exit 0;
47     }
48 }
```

- ConstFold.cpl (`20 + 10`(14) fold to `30`, `a = a * 8`(19) optimized to `a = a << 8`)
```CPL
0  {
2      extern exfunc printf;
+      glob str str_0 = "Hello from program!";
6      start() {
7          printf(str_0);
8          if 1; {
9              i32 a = 10;
10             a = a + 20;
11         }
12         else {
13             i32 b = 20;
14             i32 c = 30;
15         }
16 
17         i32 a = 5;
18         while a > 0; {
19             a = a << 8;
20         }
21 
22         i32 c = 0;
23         while 0; {
24             c = c + 1;
25         }
26         else {
27             c = 100;
28         }
29 
31         switch 0; {
32             case 10; {
33                 i32 b = 10;
34                 b = b + 10;
35             }
36             case 11; {
37                 i32 y = 10;
38                 y = y + 10;
39             }
40             default {
41                 i32 k = 10;
42                 k = k + 10;
43             }
44         }
45         
46         exit 0;
47     }
48 }
```

- CondUnroll.cpl (`if 1;` unrolled to straightforward block, `while 0;` replaced with `else` section, `switch 0;` replaced with `default` section)
```CPL
0  {
2      extern exfunc printf;
+      glob str str_0 = "Hello from program!";
6      start() {
7          printf(str_0);
8          {
9              i32 a = 10;
10             a = a + 20;
11         }
16 
17         i32 a = 5;
18         while a > 0; {
19             a = a << 8;
20         }
21 
22         i32 c = 0;
26         {
27             c = 100;
28         }
29 
40         {
41             i32 k = 10;
42             k = k + 10;
43         }
45         
46         exit 0;
47     }
48 }
```

- DeadScope.cpl (Scopes that don't affect on enviroment removed)
```CPL
0  {
2      extern exfunc printf;
+      glob str str_0 = "Hello from program!";
6      start() {
7          printf(str_0);
16 
17         i32 a = 5;
18         while a > 0; {
19             a = a << 8;
20         }
21 
22         i32 c = 0;
26         {
27             c = 100;
28         }
29
45         
46         exit 0;
47     }
48 }
```

- DeadVar.cpl (Variables that affect only on itselves removed)
```CPL
0  {
2      extern exfunc printf;
+      glob str str_0 = "Hello from program!";
6      start() {
7          printf(str_0);
16 
21
29
45         
46         exit 0;
47     }
48 }
```
