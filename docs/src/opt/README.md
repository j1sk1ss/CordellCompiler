# Optimization
## Dead functions elimination
- [deadfunc](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadfunc.c) - The first optimization algorithm. It removes unused functions by iterating through all input files, registering referenced functions, and deleting all unreferenced ones.

## String declaration
- [strdecl](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/strdecl.c) - The second optimization algorithm handles read-only strings. If the user declares strings with `ro` or `glob` flags, this algorithm allocates them in the `.data` or `.rodata` section instead of the stack. Example:
```cpl
{
    start() {
        glob str fstring = "Global";
        ro str sstring   = "ReadOnly";
        ptr str tstring  = "ptrstring";
        exit 1;
    }
}
```

Final output:
```cpl
{
    glob ro str str_0 = "Global";
    ro str str_1 = "ReadOnly";
    ro str str_2 = "ptrstring";
    start() {
        glob str fstring = str_0;
        ro str sstring   = str_1;
        ptr str tstring  = str_2;
        exit 1;
    }
}
```

Note 1: The `ptr` keyword signals that this is a pointer to the string `"ptrstring"`. Therefore, the data is placed into the `.rodata` section. </br>
Note 2: Strings with same data will be united into one shadow variable under `ro` flag. This will reduce memory usage during run-time.

## Variable inline
- [varinline](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/varinline.c) - The third optimization works together with the fourth one. It implements variable inlining and constant propagation. If the value of a variable is known, it replaces all occurrences with the value and removes the declaration. Example:
```cpl
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
```cpl
{
    start() {
        exit 10 + 11;
    }
}
```

Also this method implements something similar to data flow analyze. We track variable value in scope and stop inline operations if we can't predict future. For example this code below:
```cpl
{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        i32 c = a;

        a = 15;
        i32 d = a;

        switch a; { 
            case 1; {
                a = 15;
                i8 b = a;
            }
            case 2; {
                a = 20;
                i8 b = a;
            }
            case 3; {
                a = 25;
                i8 b = a;
            }
            default {
                a = 30;
                i8 b = a;
            }
        }

        exit a;
    }
}
```

Will become:
```cpl
{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        i32 c = 10;

        a = 15;                    <= Variabe "a" value update
        i32 d = 15;

_________________                  <= Now variable "a" is unpredicted in compile time (This is an example, don't look at constant switch condition).

        switch 15; { 
            case 1; {
                a = 15;
                i8 b = 15;         <= Used updated "a" value from this scope
            }
            case 2; {
                a = 20;
                i8 b = 20;
            }
            case 3; {
                a = 25;
                i8 b = 25;
            }
            default {
                a = 30;
                i8 b = 30;
            }
        }

        exit a;                     <= We can't say that switch was ignored, that's why we don't know value here.
    }
}
```

After variable inline, this module remove all unused variables (that never assign elsewhere). This will produce final output:
```cpl
{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        a = 15;

        switch 15; { 
            case 1; {
                a = 15;
            }
            case 2; {
                a = 20;
            }
            case 3; {
                a = 25;
            }
            default {
                a = 30;
            }
        }

        exit a;
    }
}
```

`while` loops change some logic. For example:
```cpl
{
    start(i64 argc, ptr u64 argv) {
        i8 a = 10;
        i8 c = a;                     <= We can say that "a" == 10

        a = 15;
        i8 d = a;                     <= Same as above

        while a < 100; {              <= Here is a loop condition and unpredicted block at the same time
            a = a + 1;                <= We update value in loop block, that's why we can't perform inline operation
        }

        exit d;
    }
}
```

Produced code:
```cpl
{
    start(i64 argc, ptr u64 argv) {
        i8 a = 10;
        a = 15;
        while a < 100; {
            a = a + 1;
        }

        exit 15;
    }
}
```

Note: If we remove `a = a + 1;` line from while, it will invoke inline to `while 15 < 100; {`:
```cpl
{
    start(i64 argc, ptr u64 argv) {
        while 15 < 100; {
        }
        exit 15;
    }
}
```

## Constant folding
- [constopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/constopt.c) - The fourth optimization is always executed after the third one and continues constant propagation by folding constant expressions. Example:
```cpl
{
    start() {
        exit 10 + 11;
    }
}
```

Becomes:
```cpl
{
    start() {
        exit 21;
    }
}
```

## Conditional branches unroll
- [condunroll](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/condunroll.c) - The fifth optimization works with `switch`, `if`, and `while` statements, similarly to funcopt. Its main goal is removing unreachable code in condition scopes. For instance, code inside `while (0)` is never executed, same situation with `else` block in `if 1;` statement. Example:
```cpl
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
```cpl
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
```cpl
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
```cpl
{
    start() {
        {
            exit 1;
        }
        
        exit 1;
    }
}
```

## Dead code elimination
- [deadopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadopt.c) - All code that unreacheble should be eliminated. For example:
```cpl
i32 a = 0;
exit 0;
i32 b = 1;              <= Unreachable code
exit 1;
```

Or how function can "kill" code:
```cpl
function foo() {
    exit 1;
}

i32 a = 1;
foo();                  <= "Killer" function
i32 b = 1;              <= Unreachable code
exit 1;
```

Or how optimization work with branches:
```cpl
if 1; {                 <= "Unpredicted" section. Will "kill" code below only if all branches are "killers"
    exit 1;
}
else {
    i32 a = 1;
}
i32 b = 1;
exit 0;                 <= Still reachable
```

## Dead variables elimination
- [deadvar](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadvar.c)

## Stack reusage
- [offsetopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/offsetopt.c) - This optimization pass finalizes the previous ones by recalculating local and global variable/array offsets. It implements stack slot reuse by reassigning memory locations of variables that are no longer live.
```cpl
{
    start(i64 argc, ptr u64 argv) {
        : 24 : i8 val = 0;                             <= Allocation of 8 bytes in stack
        : 32 : ptr u64 val_ptr = ref val;              <= We can't rewrite "val" due usage below and in declaration. Also we remember, that "val_ptr" now is "val" owner (ref keyword).
        : 40 : ptr u64 val_ptr_1 = ref val;            <= Same situation as above.
        : 40 : ptr u64 val_ptr_ptr = ref val_ptr;      <= "val" variable not appeared below, but "val_ptr" is owner of "val". We can't reuse this offset.
        : 24 : i32 sec_val = 0;                        <= "val_ptr_ptr" is dead, "val_ptr_1" and "val_ptr" is dead too, this means that val finally can be reused. 
        exit sec_val;
    }
}
```

Main idea of this module is to track variable usage and ownership (similar to the Rust compiler’s borrow checker, but in a simplified form).
1) Every variable can have a list of owners (ownership is acquired with the `ref` keyword).
2) When a new declaration is encountered, the module checks all existing variables for usage below this declaration (or within it).
3) If a variable is unused and has no owners, its offset can be safely reused.
4) When a variable’s offset is reused, the variable is unregistered and also removed from all owner lists where it appears.

Stack offset managing based on bitmap allocator (similar to Linux Physical Memory Manager). This mean, that we can handle next case with arrays in stack:
```cpl
{
    start(i64 argc, ptr u64 argv) {
        : 24 : i32 a = 0;               <= Allocate 8 bytes
        : 24 : arr name[10, i32] =;     <= Free above 8 bytes and allocate another 40
        : 64 : i32 b = 0;               <= Allocate 8 bytes
        : 64 : i32 b1 = name[0];        <= Free above and allocate another 8
        : 24 : i32 b2 = 0;              <= Free array's 40 bytes and allocate 8
        exit b2;
    }
}
```

And one last example:
```cpl
{
    start() {
        : 16 : i32 a = 0;               <= Allocate 8 bytes
        : 24 : ptr i32 p;               <= Allocate 8 bytes
        if 1; {
            p = ref a;                  <= "p" is new owner of "a"
        }

        : 32 : i32 c = 0;               <= "p" used below, "p" is owner of "a". Allocate another 8 bytes
        exit p;
    }
}

```

## Example
For visual example, let's optimize this code:
- Source.cpl
```cpl
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
```cpl
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
```cpl
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
```cpl
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
```cpl
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
```cpl
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
```cpl
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
```cpl
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
