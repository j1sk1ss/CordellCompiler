# Semantic static checker
Cordell Compiler implements a simple static analysis tool for a basic code-checking before compilation. Thes static analysis tool is divided by two different parts: the *AST analysis* and the *IR analysis*. While the *AST analysis* commonly address general problems with typos and programmer errors (duplicated branches, wrong names, wrong arguments count, etc.), the *IR analysis* gives us essential information about possible program behaviour (null-dereference, wrong casts, etc.).

## AST part
The list of all possible AST warnings that are supported by the static analyzator is below:
- Read-only variable update. *If we have a `ro` variable, we must be sure that it never being updated somewhere*
- Invalid variable for a function's return value. *If a function returns, for instance, a `i8` value, it must be stored in a variable with the same (or larger) type*
- Declaration without initialization. *If we declare a variable, it'd be safer, if we add an initial value*
- Wrong value type for a variable declaration. *If we declare a variable with an initial value, we must be sure, that this value can be stored in this variable*
- Function without return. *If function has a return type not equals to the `i0` return type, it must have a `return` statement in all paths*
- Start block without the exit. *The initial block must have an `exit` statement in all paths*
- Function arguments number mismatch. *Function (not a function pointer) must have the exact amount of arguments as it registered in function's definition*
- Function argument type mismatch. *We must be sure, that a provided argument to a function has a data type, that can be processed by a function*
- Unused function return value. *If a function has a return type not equal to the `i0` return type, it must be used somewhere*
- Wrong variable for a function's return type. *If we store a function's result in a variable, we must check if the variable has a valid type and can handle the function's return value*
- Function's return type mismatch with an actual return type. *If we return some value from a function, we must check if this is a corrent type regarding the registered information about the function*
- Illegal array access. *We must check if the index (constant) that being used in an array expression is valid and non-negative*
- Duplicated branches. *We can check if there is a two same branches in one `if` construction*
- Invalid function name. *Some function names are reserved by the compiler. We can't allow user to use them*
- Dead code. *We can find is there is a dead code in the code, and if this was an intent product*
- Possible implicit convertion. *The compiler is a static typed, but not a strict typed. To fill this gap, the checker will inform if there is a possible future implicit cast*
- Inefficient `while`. *Sometimes the `loop` keyword is better than `while 1`*
- Incorrect exit type for a function. *The `exit` keyword must be used in a function (not in a `start` function) only by one condition - there is no `start` function and this is the lowest non-local function in the file*
- Break usage without a target. *The `break` keyword must be used only to break `loop`s and `while`s*
- `i0` function's return value usage. *If a function has a `i0` return type, its value can't be stored in any variable*
- Unused expression. *Any expression that doesn't stored in a variable, used in a function, evaluated in a `if`, a `while` or a `switch` statements is an unused expression*
- Reference to an expression. *Reference operation could be performed only on variable*
- Non-even align. *If align of a variable or an array isn't even, we must inform the programmer about this*

**Note:** By default the static analysis is turned off. To turn it on, use the `--static-analysis` flag.

For instance, let's consider the code below:
```cpl
{
    #include "string_h.cpl"
    function foo() -> i32 { return 1; }

    function barBar() -> i0 { }

    function BazBaz() { return 1; } 

    function baz(i32 a) -> i0 {
        if a == 0; { return 1; }
        else { return 1; }

        if a == 0; { return 1; }
        else { }
    }

    function fang(i32 a) -> i8 {
        if not a; { return 123321; }
        else { }
        return 1;
        i32 b = 1;
        return b;
    }

    start() {
        i8 b;
        ptr i8 bref = ref b;
        ptr i8 bref1 = bref;
        ptr i8 bref2 = bref1;

        i8 a = foo();
        i8 c = 123123;
        u8 asd = barBar();
        baz(1);
        fang(10);
        
        break;
        a + c;

        arr array[10, i32];
        array[-1] = 0;
        array[12] = 0;
        strlen("Hello!");
    }
}
```

The code above will produce a ton of errors and warnings.
<details>
<summary><strong>Semantic analysis output</strong></summary>

```
[WARNING] Possible branch redundancy! The branch at [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:11:25] is similar to the branch at [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:11:25]!
10 | if a == 0;
10 | {
   | ^
10 |     return 1;
   | ^^^^^^^^^^^^^
10 | }
   | ^
11 | else {
   | ^    ^
11 |     return 1;
   | ^^^^^^^^^^^^^
11 | }
   | ^
   | ^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:20:16] 'Dead Code' after the termination statement!
18 | {
18 |     if not a;
18 |     {
18 |         return 123321;
18 |     }
19 |     else {
19 |     }
20 |     return 1;
21 |     i32 b = 1;
   |     ^^^^^^^^^
22 |     return b;
18 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:33:28] The function='barBar' doesn't return anything, but result is used!
33 | u8 asd = barBar();
   |          ^^^^^^^^
[ERROR]   [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:41:20] Array='array' accessed with a negative index!
41 | array[-1];
   |       ^^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:38:14] The expression returns value that never assigns!
26 | {
26 |     i8 b;
27 |     ptr i8 bref = ref b;
28 |     ptr i8 bref1 = bref;
29 |     ptr i8 bref2 = bref1;
31 |     i8 a = foo();
32 |     i8 c = 123123;
33 |     u8 asd = barBar();
34 |     baz(1);
35 |     fang(10);
37 |     break ;
38 |     a + c;
   |     ^^^^^
40 |     arr array = 10;
41 |     array[-1] = 0;
42 |     array[12] = 0;
43 |     strlen(Hello!);
26 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:37:15] The 'break' statement without any statement that uses it!
26 | {
26 |     i8 b;
27 |     ptr i8 bref = ref b;
28 |     ptr i8 bref1 = bref;
29 |     ptr i8 bref2 = bref1;
31 |     i8 a = foo();
32 |     i8 c = 123123;
33 |     u8 asd = barBar();
34 |     baz(1);
35 |     fang(10);
37 |     break ;
   |     ^^^^^^
38 |     a + c;
40 |     arr array = 10;
41 |     array[-1] = 0;
42 |     array[12] = 0;
43 |     strlen(Hello!);
26 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:35:14] Unused the function='fang's result!
26 | {
26 |     i8 b;
27 |     ptr i8 bref = ref b;
28 |     ptr i8 bref1 = bref;
29 |     ptr i8 bref2 = bref1;
31 |     i8 a = foo();
32 |     i8 c = 123123;
33 |     u8 asd = barBar();
34 |     baz(1);
35 |     fang(10);
   |     ^^^^^^^^
37 |     break ;
38 |     a + c;
40 |     arr array = 10;
41 |     array[-1] = 0;
42 |     array[12] = 0;
43 |     strlen(Hello!);
26 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:32:26] Illegal declaration of 'c' with '123123' (Number bitness is=32, but 'i8' can handle bitness=8)!
32 | i8 c = 123123;
   |        ^^^^^^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:31:15] Function='foo' return type='i32' not match to the declaration type='i8'!
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:26:15] Variable='b' without initialization!
26 | i8 b;
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:25:11] Start doesn't have the 'exit' statement in all paths!
25 | start ()
25 | {
26 |     {
26 |         i8 b;
27 |         ptr i8 bref = ref b;
28 |         ptr i8 bref1 = bref;
29 |         ptr i8 bref2 = bref1;
31 |         i8 a = foo();
32 |         i8 c = 123123;
33 |         u8 asd = barBar();
34 |         baz(1);
35 |         fang(10);
37 |         break ;
38 |         a + c;
40 |         arr array = 10;
41 |         array[-1] = 0;
42 |         array[12] = 0;
43 |         strlen(Hello!);
26 |     }
25 | }
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:10:38] Function='baz' has the return value, but isn't supposed to!
10 | return 1;
   |        ^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:11:28] Function='baz' has the return value, but isn't supposed to!
11 | return 1;
   |        ^
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:13:38] Function='baz' has the return value, but isn't supposed to!
13 | return 1;
   |        ^
[INFO]    [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:5:22] Function name='barBar' isn't in a sneaky_case! 'camelCase'
[WARNING] [/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/sem_test.cpl:5:14] Function='barBar' doesn't have the 'return' statement in all paths!
 5 | function barBar() -> i0 
 5 | {
 5 | }
```
</details>

**Note 1:** This isn't an entire analysis output due to the critical error with the array indexing. Such errors will block a compilation process given the importance of this kind of errors. </br>
**Note 2:** The static analyzer doesn't use a source file to show a error place. For these purposes, it uses the 'restorer' module that restores the code from AST.

## IR part
The list of all possible IR warnings that are supported by the static analyzator is below:
- NULL-dereference. *If we're trying to dereference a variable (or a value) which is NULL, we must terminate compilation.*
- Constant IF. *We can warn a user if there is a dead branch presented.*
