# CPL changelog
Logs for the first and second versions are quite short because I don’t remember exactly what was introduced and when. However, this page lists most of the major changes. In fact, it was created mainly to document the project’s evolution in a clear way, without a need to read through all of the commits.

## Structure
```
...
^
[Version v3.4]
^
... minor changes related to v3.3 ...
^
[Version v3.3]
^
... minor changes related to v3.2 ...
^
[Version v3.2]
^
... minor changes related to v3.1 ...
^
[Version v3.1]
^
... minor changes related to v3 ...
^
[Version v3]
^
[Version v2]
^
[Version v1]
```

----------------------------------------

## syscall checker
The static analysis tool now accepts the 'syscall' keyword. At this point, we have MACHO support ('cause I'm testing this on my MacBook tho). </br>
The support implies that the analyzer will check if all of the arguments are correct typed for a selected syscall number. For instance:
```cpl
syscall(0x2000004, 1, 1, 12);
```

This is a write syscall from MACHO, and the second arguments is a STDIN / STDOUT (destination), the third one is a buffer pointer and the last one - the size of a buffer. In the case, I've passed '1' as a buffer, and it reveals that I need to cast it (at least):
```cpl
start() {
    syscall(0x2000004, 1, 1, 12);
    syscall(0x2000004, 1, 1 as ptr i0, 12 as u64);
}

:/ OUTPUT
[WARNING] [2:41] Syscall with number 4 has some wrong typed arguments! It can lead to UB, consider to cast them:
          [2:41]     Argument 2 should have the 'u64' type, but the 'i8' is provided! Consider to cast it.
          [2:41]     Argument 1 should have the 'ptr i0' type, but the 'i8' is provided! Consider to cast it.
/:
```

The information about those syscalls are stored directly in the compiler at this moment. I'd like to move it to a file, but I'll do it later. </br>
This little change addresses the issue with cast problems. The compiler heavily depends on the correct typing (he's making a choice of a register based on the variable's type), and even the wrong cast can pass to a syscall some garbage from the stack.

## No more 'ptr str'
I've faced a lot of troubles with a support of a 'ptr str' object. It does the same what does a 'ptr i8' object, but must be threated separatly, which creates a lot of complex cases. Hense, it's a lot easier to just abolish the 'ptr str' syntax and suggest a user to use the 'ptr i8' alternative instead. It doesn't change any code generation or logic, it just says, that 'str' objects can be only on the stack, same as 'arr' objects. </br>
You'd suggest to remove 'str's completely, but I have some plans in future to add support for an in-built string comparison. For instance, my plan is to recognize the below code as a valid code:
```cpl
str msg = "Hello world!";
if msg == some_pointer as ptr i8; {
    :/ Do something /:
}
```

Such an operation as the presented above **must** include some object which will indicate the compiler, that we're working with a string object. This string object will contain all essential info for code generation and operation generation such as the string's length, the string's body, etc.

## Brainfuck!
Now the compiler compiles a brainfuck intepreter! The code below works!
```cpl
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

glob arr tape[30000, i8];
glob arr bracketmap[10000, i32];
glob arr stack[10000, i32];

start(i32 argc, ptr ptr i8 argv) {
    i32 pos = 0;
    i32 stackptr = 0;
    i32 codelength = strlen(argv[1]);
    while pos < codelength; {
        @[no_fall]
        @[straight]
        switch argv[1][pos]; {
            case '['; {
                stack[stackptr] = pos;
                stackptr += 1;
            }
            case ']'; {
                if stackptr > 0; {
                    stackptr -= 1;
                    i32 matchpos = stack[stackptr];
                    bracketmap[pos] = matchpos;
                    bracketmap[matchpos] = pos;
                }
            }
        }
        
        pos += 1;
    }
    
    i32 pc = 0;
    i32 pointer = 0;
    while pc < codelength; {
        @[no_fall]
        switch argv[1][pc]; {
            case '>'; {
                pointer += 1;
                pc += 1;
            }
            case '<'; {
                pointer -= 1;
                pc += 1;
            }
            case '+'; {
                tape[pointer] += 1;
                pc += 1;
            }
            case '-'; {
                tape[pointer] -= 1;
                pc += 1;
            }
            case '.'; {
                putc(tape[pointer]);
                pc += 1;
            }
            case '['; {
                if not tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            case ']'; {
                if tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            default {
                pc += 1;
            }
        }
    }

    exit 0;
}
```

## Refactoring and bugfixes
Have fixed a lot of bugs with register allocation, liveness analysis, etc. Nothing special was implemented.

## Number types
The numbers in the compiler always were treated as `i64` values which isn't incorrect, but also isn't precise. To address this issue numbers now have the type based on its own value. For instance:
```cpl
100 : i8 :
130 : u8 :
1000 : i16 :
50000 : u16 :
: etc :
```

P.S.: This allows the compiler move closer to the strong typing.

## Large comment blocks
Now the compiler supports the second set of a comment creation:
```cpl
: COMMENT LINE :

:/
This is: a comment too!
/:
```

## Sizeof as a keyword
From the sizeof annotation to the sizeof keyword.
```cpl
: old i32 len = @[sizeof]"Hello world"; :
i32 len = sizeof("Hello world");
```

## Hidden return
Same as it does Rust, the compiler now recognizes the next syntax:
```cpl
function simple() {
    10 + 10;
}
```

The `return` statement now optional in cases, when we're talking about the last function's block. This feature is really usefull in terms of lambda functions:
```cpl
: function logic(i32 a, i32 b, ptr i0 f); :
logic(10, 123, (i32 a, i32 b) => { a + b * 100; });
: function foo(ptr i0 f); :
foo(() => 10);
foo((i32 a) => a * a);
```

## Lambdas!
Now the compiler supports lambda functions. Actually, this is a syntax sugar 'cause it copies the behaviour of local functions. The syntax is next:
```cpl
ptr i0 f = (i32 a) => { return a; }
ptr i0 f = () => { return 10; }
```

To start a lambda function definition you need to create a variable placeholder. Let's say we create an empty holder (i8 a). Now with the `=>` symbol we can define any logic in a scope. </br>
Here is a set of possible lambdas:
```cpl
function foo(ptr i0 f);
foo((i32 a, i32 b) => { if 1; return a; return b; });
foo(
    (i32 a, i32 b) => {
        if 1; {
            return a * 10;
        }
        return b - 10 * a;
    }
);
```

## Constant propagation thru parameters list
Now the constant propagation module supports propagation thru function call arguments. If we have function calls (or a function call) with the same arguments (at least on position in arguments the same) it will propagate input arguments (propagate folding further) to the function. For instance:
```cpl
function foo(i32 a) {
    return a + 10;
}

start() {
    foo(10);
}
```

In the code above we will propagate the '10' to the function which will create the next code:
```cpl
function foo(i32 a) {
    return 20;
}

start() {
    foo(10);
}
```

This is a simple example, but it will work with a complext cases as well.

## HIR static analyzer and HIR locations
Now the information about file location is going to HIR via a special operation and a special subject. This feature allows to expand the static analysis to HIR part. For test I've added the NULL-dereference tester and the IF-tester. </br>
The code now looks next:
```
setpos, line=1, column=7, file=<unknown>
{
setpos, line=1, column=7, file=<unknown>
setpos, line=1, column=7, file=<unknown>
    start {
        {
setpos, line=2, column=9, file=<unknown>
            {
setpos, line=2, column=9, file=<unknown>
setpos, line=2, column=9, file=<unknown>
                i32s %0 = alloc(8);
setpos, line=2, column=9, file=<unknown>
setpos, line=2, column=22, file=<unknown>
setpos, line=2, column=18, file=<unknown>
                i32t %1 = i8n 1 as i32;
setpos, line=2, column=12, file=<unknown>
                i32s %0 = i32t %1;
setpos, line=3, column=10, file=<unknown>
setpos, line=3, column=13, file=<unknown>
                exit i32s %0;
            }
        }
    }
}
```

## not_lazy
Lazy logic operators are the default solution for logic in C language, and now, CPL supports the alternative approach. With this annotation the compiler will generate both sides before the final evaluation. </br>
To see the difference, let's consider the next example:
```cpl
function foo(); : Always returns 0 :
function foo(i32 a);
i32 a = foo() && foo(10);
i32 b @[not_lazy] (foo() && foo(10));
```

The `a` variable will obtain '0' before evaluation of the 'foo(10)'. In contrast the `b` variable will obtain the same value but with the 'foo(10)' evaluation.

## There is no basic scope anymore!
Eventually, the basic scope has gone. Now, the syntax is a lot closer to C:
```cpl
function foo();
start() {
    foo();
    exit 1;
}

: Instead of :
{
    function foo();
    start() {
        foo();
        exit 1;
    }
}
```

P.S.: *It doesn't affect on the existed code, its behaviour or something like that, it just looks better.*

## Remove section, align and import keywords
Section and align was fully duplicated by annotations which is more convenient. The `import` isn't fits to a language's design from this point (headers fit better). </br>
```cpl
: OLD
align(16) {
    i32 a;
    i32 b;
    section(".bss") {
        i32 c;
        i32 d;
    }
}
:

@[align(16)] i32 a;
@[align(16)] i32 b;
@[align(16)] @[section(".bss")] i32 c;
@[align(16)] @[section(".bss")] i32 d;
```

## sizeof
With the `sizeof` annotation now it is possible to support the next code:
```cpl
arr data[256, i32];
i32 index = 0;
while index < @[sizeof]data; {
    data[index] = 0;
}
```

This annotation will give you exact size in bytes of an object:
```cpl
@[sizeof]1; : Will return max bytness of the system :

ptr i32 a;
@[sizeof]a; : Will return max bytness of the system :

arr b[10, ptr i32];
@[sizeof]b; : Will return max bytness of the system * 10 :

str msg = "Hello world!";
@[sizeof]msg; : Will return 13 :

function foo();
@[sizeof]foo; : Will return max bytness of the system :
```

## Register
Now the compiler has the `register` annotation. It merely links a variable (only a variable) with the specific system register (index). </br>
P.S.: *Strongly depends on the target architecture.*
```cpl
#define RAX 0
@[register(RAX)] i32 a = 1;
:
mov rax, 1
:
```

## Cold/Hot
With the `hot` and `cold` annotations now it becomes possible to generate cold sections. It works with simple ifs (if-else) and switches. For instance:
```cpl
@[cold] if 1; { : IF1 :
    : something :
}
else { : ELSE1 :
    : something hot :
}
```

The `cold` annotation will move the `IF1` branch to the end of a function and save the `ELSE1` branch. The same situation with the `switch` statement with one change - the `switch` doesn't support `hot` annotations (they just can't figure it out which sections will go to cold. They all?). For instance:
```cpl
@[no_fall]
switch 1; {
    @[cold] case 1; {}
    case 2; {}
    default {}
}
```

**Note:** This code will move the `case 1;` branch to the end of a function. Also, consider the `no_fall` as an essential annotation in such cases. 

# Version v3.4
I remember that CPL is a system programming language which means it can handle tasks such as a bootloader creation, VGA print, FS, etc. To support these things, the compiler (and the language) now support the next list of features:

## Align and Section keyword
For system programming is essential to have the 'section' and the 'align' modifiers. Now the compiler supports the next syntax:
```cpl
section(".text") {
    glob i32 a;
    align(16) glob i32 b;
    align(64) {
        glob i32 c;
        glob i64 d;
    }
}
```

**Note:** 'Align' and 'Section' scopes don't affect on the target variable declaration scope. This means, that it won't increase the scope id of a variable. 

## Annotations
The second way of the section and align (not only) definition - is an annotation. The syntax is similar to Rust:
```cpl
@[section(".text")]
@[naked]
function foo();
@[align(16)] glob i32 a;
```

At this moment the compiler supports the next list of annotations:
- `naked` - Will disable all entry and exit routines in the final assembly code for an annotated function.
- `align` - Will do the same work as it does the 'align' keyword.
- `section` - Will do the same work as it does the 'section' keyword.
- `address` - Will put a function to a specific address.
- `entry` - Set function as an entry point of the code.

The `align` and the `section` keywords do the same work as it do annotations but in more convenient way. Annotation can't be applied to a many declarations or to a several functions.

----------------------------------------

## i0 variable type
The `i0` variable type now is possible to use for variables. 
```cpl
ptr i0 a;
```

It must have the `ptr` keyword/s. Otherwise it won't work. </br>
Also, now the pointer function by default is the `ptr i0` type.
```cpl
function foo();
ptr i0 a = foo;
a();
```

## Local functions
Same as in Rust, functions can define another functions in their body:
```cpl
function foo() -> i0 {
    function bar() -> i32 {
        return 32;
    }
    return bar();
}
```

These functions can be optimized as a regular one. Also, these functions (at this moment) don't have any access for outer variables:
```cpl
function var_decl() -> i0 {
    i32 a;
    function var_try_to_use() -> i0 {
        a += 1; : <= Illegal :
    }
    var_try_to_use();
}
```

Also, such functions can be used as a return value when you want to implement something like a 'function factory':
```cpl
function factory(i32 key) -> ptr u64 {
    switch key; {
        case 1; {
            function foo() {
                return 1;
            }
            return foo;
        }
        default; {}
        case 2; {
            function bar() {
                return 2;
            }
            return bar;
        }
    }
}

start() {
    exit factory(1)();
}
```

## Scope functions
At this moment a pretty useless feature of the compiler:
```cpl
{
    function foo();
}
{
    function foo();
}
```
Scopes now participate in function symbol resolution.

## Function return type new semantic
The semantic of the CPL has moved a bit towards Rust language. Now instead of the '=>' as a rtype, you will need to use the '->'.
```cpl
function foo() -> i0; : instead of function foo() => i0; :
```

It wasn't possible before due to the lack of tokenizer's abillities. Now it's possible.

## Pointer calls
Now the compiler supports function pointers! One disadvantage here: Pointed functions aren't able to support the default-args and function-overloads. To use it, you can consider the next example:
```cpl
{
    function foo(i32 a) => i32 {
        return a;
    }

    start() {
        ptr u64 a = foo;
        i32 b = a(10);
    }
}
```

Here we put the 'foo' function as a pointer to the 'a' variable. Then, we can invoke this function by this pointer. Obviously, we can't expect the default arguments here (at least at this moment). Also, function pointers are 'u64' pointers, that's why store them in 'ptr u64' variables (The compiler doesn't support signature types at this moment). </br>
Old (or traditional) function calls work the same as before this update.

## Matricies!
Now the compiler supports multi-indexation!
```cpl
extern ptr ptr u32 matrix;
matrix[0][0] = 1;
```

To declare matricies, you will still need to use regular arrays:
```cpl
arr a[2, u32];
arr b[2, u32];
arr c[2, ptr u32] = { ref a, ref b };

c[0][0] = 1; : a[0] :
c[1][0] = 1; : b[0] :
```

## if-elseif-else syntax support
Now this compiler supports the next code snippet:
```cpl
if a; {

}
else if b; {

}
else if c; {

}
: ... :
else {
    
}
```

## Function overloads (basics)
CorellCompiler now supports overloaded functions. The symtax is below:
```cpl
{
    function chloe(i32 a, i32 b = 10) => i0;
    function chloe(i64 a, i32 b = 10) => i0;
    start() {
        chloe(10 as i32);
        chloe(10 as i64);
        exit 0;
    }
}
```

The idea that HIR level can choose a function that supports the input arguments. To help the compiler, use the `as` keyword. Additionally, functions with the 'default' argument, for instance:
```cpl
function chloe(i32 a, i32 b = 10) => i0;
```

can't support such polymorphism. The example above works given the same number of the arguemnts. If we consider the next example:
```
function chloe(i32 a, i16 b = 10) => i0;
function chloe(i32 a, i32 b = 10, i32 c = 10) => i0;
```

this mechanism won't work.

## Variadic arguments
Now CPL supports variadic arguments! The syntax is similar to C language:
```cpl
function foo(...) -> i0 {
}
```

To pop an argument from this set, use the `poparg` keyword:
```cpl
function max(...) -> i0 {
    i32 chloe = poparg as i32;
}
```

Also, the `poparg` keyword can be used in any function with arguments. It simply replaces any argument loading:
```cpl
function foo(i32 a, i32 b) {
    i32 c = poparg as i32; : a :
    i32 d = poparg as i32; : b :
}
```

# Version v3.3
CPL-preprocessing derictives and new include system. </br>
Now this compiler supports derictives such as `define`, `undef`, `ifdef` and `ifndef`. Also it supports the `include` statement. For more information, check the main README file. </br>
For instance let's consider the next piece of code:
```cpl
: string_h.cpl :
{
#ifndef STRING_H_
#define STRING_H_ 0
    : Get the size of the provided string
      Params
        - `s` - Input string.

      Returns the size (i64). :
    function strlen(ptr i8 s) => i64;
#endif
}

: print_h.cpl :
{
#ifndef PRINT_H_
#define PRINT_H_ 0
    #include "string_h.cpl"

    : Basic print function that is based on
      a syscall invoke.
      Params
      - `msg` - Input message to print.
      
      Returns i0 aka nothing. :
    function print(ptr i8 msg) => i0;
#endif
}

: include_test.cpl :
{
    #include "print_h.cpl"
    #include "string_h.cpl"

    function foo() => i0;
    
    function bar() => i0 {
        foo();
    }

    function foo() => i0 {
        print("Hello world!\n");
    }

    start(i64 argc, ptr u64 argv) {
        bar();
        exit 0;
    }
}
```

After the PP, we will get a new form of the code:
```cpl
{
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/print_h.cpl"
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/string_h.cpl"    
    function strlen(ptr i8 s) => i64;
#line 4 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/print_h.cpl"
    function print(ptr i8 msg) => i0;
#line 2 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/include_test.cpl"
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/string_h.cpl"
#line 3 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/include_test.cpl"

    function foo() => i0;
    
    function bar() => i0 {
        foo();
    }

    function foo() => i0 {
        print("Hello world!\n");
    }

    start(i64 argc, ptr u64 argv) {
        bar();
        exit 0;
    }
}
```

The `line` derictive here is serving only one purpose - Tell to the tokenizer, where we are.

----------------------------------------

## LiS message
The `lis` statement now accepts a message for a convenient debug.
```cpl
lis;                 : <= Old, but still recognizeble by the compiler :
lis "Debug line #1"; : <= New :
```

These messages will be placed in a final `.asm` file. With the `gdb` the source decompiled file will provide a line where it stops. Produced `.asm` file will include these comments as showed below:
```asm
mov rax, 1
int3 ; Debug line #1
```

## neg, ref and dref now have their own parser
Previously, these staements were affect as flags in tokens. Such a mechanic was unconvenient in cases like below:
```cpl
i32 a = not (c + b);
dref (a + 0x7C00) = 0xDEADBEEF;
```

Now these statements have their own AST and HIR handlers. 

## Explict casting is here!
The CPL language now supports the casting operation. </br>
For instance:
```cpl
i32 a = 10 as i32;
i32 b = 10 as i32;
u8 c = (a + b) as u8;
```

## Additional operators
Implement the next list of operators:
| Operation        | Example   |
|------------------|-----------|
| `%=`             | `X %= Y`  |
| `\|=`            | `X \|= Y` |
| `&=`             | `X &= Y`  |
| `^=`             | `X ^= Y`  |

## Loop statement
Now the `CPL` supports the `loop` statement!
```cpl
loop {
    break;
}
```

This statement the same with the `loop` from Rust.

## Break statement
Now the `CPL` supports the `break` statement!
```cpl
while 1; {
    break;
}

switch 1; {
    case 1; {
        break;
    }
    default {
        break;
    }
}
```

## PTRN DSL
Peephole optimization first phase now is a fully generated by the `PTRN` domain-specific language! Documentation can be seen in the `src/lir/peephole/pattern_generator/README.md`. This allows us to costruct complex and flexible templates for the basic peephole optimization.

## String object
For optimization purpose was implemented a string object. This object responses for the `char*` operations such as `strcat`, `strcmp`, `strcpy`, etc. For better performance the `strcmp` and the `strlen` functions support cache and hash. The `strlen` function simply returns cached value from `string` object, while `strcmp` uses hashes for better string comparison.

# Version v3.2
Now I'm working in the ISP RAS as Research Assistant in the Compiler Department (Static Analyzation team). With some additional experience, now I'm able to implement static analysis in CPL. The basic module contains the semantic entry-point, ast and hir folders. AST folder contains entry point for AST-walker and AST-walker behaviour-scripts. Main idea is simple: We have a walker, that has linked list of actions for different node-types:
```c
typedef enum {
    EXPRESSION_NODE  = 1 << 0,
    ASSIGN_NODE      = 1 << 1,
    DECLARATION_NODE = 1 << 2,
    FUNCTION_NODE    = 1 << 3,
    CALL_NODE        = 1 << 4,
    START_NODE       = 1 << 5,
    DEF_ARRAY_NODE   = 1 << 6,
    UNKNOWN_NODE     = 1 << 7,
} ast_node_type_t;
```

We can register handlers for each node-type. Each handler - different pattern matcher for code check process. Registration is simple:
```c
int SEM_perform_ast_check(ast_ctx_t* actx, sym_table_t* smt) {
    ast_walker_t walker;
    ASTWLK_init_ctx(&walker, smt);

    ASTWLK_register_visitor(ASSIGN_NODE, ASTWLKR_ro_assign, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE | ASSIGN_NODE | EXPRESSION_NODE, ASTWLKR_rtype_assign, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_not_init, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_illegal_declaration, &walker);
    ASTWLK_register_visitor(FUNCTION_NODE, ASTWLKR_no_return, &walker);
    ASTWLK_register_visitor(START_NODE, ASTWLKR_no_exit, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_not_enough_args, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_wrong_arg_type, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_unused_rtype, &walker);
    ASTWLK_register_visitor(DEF_ARRAY_NODE, ASTWLKR_illegal_array_access, &walker);

    ASTWLK_walk(actx, &walker);
    ASTWLK_unload_ctx(&walker);
    return 1;
}
```

The simplest handler is a ASTWLKR_ro_assign handler. Here is the it's source code:
```c
// #define AST_VISITOR_ARGS ast_node_t* nd, sym_table_t* smt
int ASTWLKR_ro_assign(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg) return 1;

    if (larg->token->flags.ro) {
        SEMANTIC_ERROR(" [line=%i] Read-only variable=%s assign!", larg->token->finfo.line, larg->token->value);
        return 0;
    }

    return 1;
}
```

Also, this version of compiler now operated with ACT (automated commit tool). This tool also is simple, but makes commit section more readeble and "atomic".

### Caller-saving
In the instruction and the memory selection stage here is a one additional step. Now we procceed register saving step for pushing and poping all used in function registers.

### Documentation update
LIR part now is more complex then it was earlier, that's why I start documentation sync process.

### Major refactoring
With custom memmory manager, it's really simple to fix memleaks in entier project. Also, for better code reading in future, I spend some time to make large code refactoring.

### Fixes in inline function
Now inline operation will copy not only variables, now it copies arrays with array elements (if array is local and placed in stack). Also, now SSA renames arrays (local arrays) in same way as it works with variables. Additional function for array's symtable copy function implemented.

### LIR peephole [WIP]
Write optimization (removing unused write operations such as redundant movs). 

### Array args list in HIR and LIR
Instead storing array elements in symtable (in hir_subject form), now it stored in hir and lir array declaration directly. This allows us to perform function inline in more efficient and simple way. In other hand it makes difficult to work with global arrays that are defined in object .data and .rodata sections.

### DFG location
DFG (IN, OUT, DEF and USE calculation) moved from HIR to LIR after instruction selection. Main idea here, preserve registers from rewrite by creating additional interference of tmp variables precolored with used registers in operation.

### Constant propagation
Constant propagation based on DAG and updates data in variable's symtable (works only with constants and numbers). Propagate constants thru:
- Arithmetics
- Convertation
- Copying

### LIR peephole optimization
| Original Instruction        | Optimized Instruction | Explanation |
|-----------------------------|-----------------------|-------------|
| `mov rax, 0`                | `xor rax, rax`        | Zeroes the register without writing to memory; resets flags; usually faster and smaller than `mov`. |
| `sub rax, rax`              | `xor rax, rax`        | Equivalent zeroing; `xor` is generally preferred. |
| `add rax, rax`              | `shl rax, 1`          | Multiply by 2 using shift; can be cheaper than `add` on some CPUs. |
| `imul rax, imm_power_of_2`  | `shl rax, log2(imm)`  | Multiplication by power of 2 replaced with shift. |
| `cmp rax, 0`                | `test rax, rax`       | `test` sets flags like `cmp` but is often cheaper. |
| `mov rax, rax`              | remove                | NOP instruction; useless. |
| `add rax, 0`, `sub rax, 0`  | remove                | Adding zero is a no-op. |
| `imul rax, 1`, `div rax, 1` | remove                | Multiplying by 1 is a no-op. |

### Instruction Planning
Instruction planning will create DAG for each base block, then reorder some instruction depending on target info. Target info - special structure for target CPU arch and machine. For simplicity, I make some python scripts in `src/lir/instplan` directory (`build_targinfo.py` and `read_targinfo.py`).

### Regallocation
Regallocation moved from HIR level to LIR level. Planning to add support of linear and graph register allocation. This move allows me usage of rdx, rdi, rsi... registers, without fear of re-writing (Now I'm able to precolor variables and link them to specific register like ABI registers in function call). 

### Instruction selection
Module for instruction selecting (template section) implemented. In few words: This module lower abstraction to be closer with target machine. Example below.
```lirv1
fn strlen(i8* s) -> i64
{
    %16 = ldparam();
    {
        %18 = %17;
        %19 = %16;
        kill(cnst: 0);
        kill(cnst: 1);
        %17 = num: 0;
        %9 = num: 1 as u64;
        %12 = num: 1 as u64;
        lb10:
        %5 = *(%19);
        %7 = %5 as i64;
        %6 = %7 > num: 10;
        cmp %6, cnst: 0;
        je lb11;
        jmp lb12;
        lb11:
        {
            %8 = %19 / %9;
            %20 = %8;
            %11 = %18 as u64;
            %10 = %11 % %12;
            %13 = %10 as i64;
            %21 = %13;
        }
        %18 = %21;
        %19 = %20;
        jmp lb10;
        lb12:
        return %18;
    }
}
```

Transforms into the code with the support of ABI and specific machine registers (`rax`, `rbx`, `rcx`):
```lirv2
fn strlen(i8* s) -> i64
{
    %16 = rdi;
    {
        %18 = %17;
        %19 = %16;
        kill(cnst: 0);
        kill(cnst: 1);
        %17 = num: 0;
        %9 = num: 1 as u64;
        %12 = num: 1 as u64;
        lb10:
        %5 = *(%19);
        %7 = %5 as i64;
        rax = %7;
        rbx = num: 10;
        cmp rax, rbx;
        setg al;
        %6 zx= al;
        cmp %6, cnst: 0;
        je lb11;
        jmp lb12;
        lb11:
        {
            rax = %19;
            rbx = %9;
            rax = rax / rbx;
            %8 = rax;
            %20 = %8;
            %11 = %18 as u64;
            rax = %11;
            rbx = %12;
            rdx = rdx ^ rdx;
            rdx = rax % rbx;
            %10 = rdx;
            %13 = %10 as i64;
            %21 = %13;
        }
        %18 = %21;
        %19 = %20;
        jmp lb10;
        lb12:
        return %18;
    }
}
```

# Version v3.1
New LIR level. Instead straigthforward LIR generation, now this is a another 3AC level, suitable for instruction selection and instruction planning. Also, instead only register allocation based on graph coloring, this level support register allocation based both on linear scanning approach and graph coloring. 

----------------------------------------

### Inlining 
Function inlined if it reach score higher than 2 points.
```c
static int _inline_candidate(cfg_func_t* f, cfg_block_t* pos) {
    if (!f) return 0;
    int score = 0;

    if (
        pos->type == CFG_LOOP_BLOCK ||
        pos->type == CFG_LOOP_LATCH
    ) score += 2;

    int block_count = list_size(&f->blocks);

    if (block_count <= 2)       score += 3;
    else if (block_count <= 5)  score += 2;
    else if (block_count <= 10) score += 1;
    else if (block_count > 15)  score -= 2;
    
    return score > 2;
}
```

### TRE (tail recursion elimination)
TRE implementation simply do rrcursion elimination if next block after recursion is terminator block (without successors).

### IG fix
Now Interference Graph calculated with `IN`, `DEF` and `OUT` instead only `DEF` and `OUT` sets according to [this](https://courses.cs.cornell.edu/cs4120/2022sp/notes/regalloc/index.html) article.

### AST opt deadfunc
From AST level dead function elimination to HIR level based of call graph.

### SSA LICM optimization
Redundand calculations (instead basic inductions) now moved from loop body to loop preheader.

### CFG BB genration changed
Previous version of BB generation includes complex if operations without two jmps support, that's why leaders from DragonBook works incorrect. Now there is no IFLWR, IFGRT and similar operations, only IFOP2.

### LIR generation based on CFG instead raw HIR
Now LIR generator works only with a CFG data instead of a raw HIR list. Also, the LIR generator produces not only a raw LIR list. Now it produces an updated meta information for base blocks in a CFG (entry and exit for LIR list for asm generator).

### Constant propagation
HIR_DAG_sparse_const_propagation function was implemented. Also there is the new types for numbers and contants (constants and numbers for f/u/i 64/32/16/8). 

### Debug features of CPL
Additional instruction called `lis` (Interesting abbreviation, isn't? This is a LinearIsStop? or is a LiveInputStage? Or... nevermind) and used for setting breakpoints in the code. For example:
```cpl
start() {
    i32 a = 10;
    arr b[123, f64];
    lis; : <- Breakpoint :
    exit 1;
}
```

For usage, run program (executable) with debug tool (like `gdb`, `lldb`).

# Version v3
This is the third version of this compiler. Was performed a full structure transform (from the `token` -> `AST` -> `ASM` structure, that hasn't been changed since the first version, to the `token` -> `AST` -> `HIR` (`CFG` -> `SSA` -> `DAG` -> `CFG`) -> `RA` -> `LIR` -> `ASM` structure). Also, during the development of this version, the `changelog` section was created (on 10.20.2025) though. Additionally, this version is the `optimization-implementation` version. List of the implemented optimizations:
- HIR
    - Constant propagation
    - Constant folding
    - LICM
- LIR
    - MOV optimization

----------------------------------------

# Version v2
This is the second version of this compiler (at the moment of 10.20.2025, at least a stable work version). Main features is a full code refactoring of the `token` part and the `AST` generation. Also perform a cleanup and implement the basic `LIR`. The main improvement was in the syntax of the `CP-language` (Improve the gramma).

```cplv2
extern exfunc printf;
function itoa(ptr i8 buffer, i32 dsize, i32 num) => i32 {
    i32 index = dsize - 1;
    i32 tmp = 0;

    i32 isNegative = 0;
    if num < 0; {
        isNegative = 1;
        num *= -1;
    }

    while num > 0; {
        tmp = num % 10;
        buffer[index] = tmp + 48;
        index -= 1;
        num /= 10;
    }

    if isNegative; {
        buffer[index - 1] = '-';
    }

    return 1;
}

start() {
    arr buff[32, i8];
    itoa(buff, 10, 1234567890)
    printf("%s", buff);
    exit 0;
}
```

Some improvements in typing (now we're able to use Rust-like statements such as `i8`, `u8`, etc.), `asm` blocks, `external` functions, `vla` arrays, etc. This version was also tested via implementation of the `brainfuck interpreter`.

----------------------------------------

# Version v1
This is the first version of this compiler. The last commit before v2 was in the middle of summer of 2025. Main features of this version is a [`token` -> `AST` -> `ASM`] structure, basic `NASM`-syntax code generation, examples like `brainfuck interpreter`, etc. The most interesting part, in my opinion, is the syntax:
```cplv1
function itoa ptr buffer; int dsize; int num; {
    int index = dsize - 1;
    int tmp = 0;

    int isNegative = 0;
    if num < 0; {
        isNegative = 1;
        num = num * -1;
    }

    while num > 0; {
        tmp = num / 10;
        tmp = tmp * 10;
        tmp = num - tmp;
        tmp = tmp + 48;
        buffer[index] = tmp;
        index = index - 1;
        num = num / 10;
    }

    if isNegative == 1; {
        char minus = 45;
        buffer[index - 1] = minus;
    }

    return 1;
}
```

This version wasn't "friendly" as it became now (in the terms of a syntax and a code style). Also, here is the how program's body was looks like:

```cplv1
start
    int a = 0;
    int b = 1;
    int c = 0;
    int count = 0;
    while count < 20; {
        c = a + b;
        a = b;
        b = c;
        
        arr buffer 40 char =;
        itoa buffer 40 c;
        printf buffer 40;

        count = count + 1;
    }
exit 1;
```

Here is a sample from the old README of a function declaration:
```cplv1
function [name] [type1] arg1; [type2] arg2; ...; fstart 
: code ... :
fend [expression]; : return value :
```

Functions were able to handle only the one possible output. Also these functions were have only one possible exit: `fend`. Similar situation with the `if` statement:
```cplv1
if a > b; ifstart
: code ... :
ifend
```

Also that's how I was thinking users should define arrays:
```cplv1
arr sarr 5 int = 1 2 3 4 5;
```

In summary, the first version was very simple. It takes care only about forward tokens translation to an asmcode through `AST` generation.
