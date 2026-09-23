#ifndef AST_VISITORS_H_
#define AST_VISITORS_H_

#include <limits.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <csa/ast/ast_data.h>
#include <csa/misc/warns.h>
#include <csa/misc/restore.h>
#include <csa/misc/trace.h>
#include <ast/ast.h>
#include <ast/astgen.h>

#define REBUILD_CODE_0TRG(nd)                  \
        set_t __s;                             \
        set_init(&__s, SET_NO_CMP);            \
        RST_restore_code(stdout, nd, &__s, 0); \
        set_free(&__s);                        \

#define REBUILD_CODE_1TRG(nd, trg)             \
        set_t __s;                             \
        set_init(&__s, SET_NO_CMP);            \
        set_add(&__s, trg);                    \
        RST_restore_code(stdout, nd, &__s, 0); \
        set_free(&__s);                        \

#define REBUILD_CODE_2TRG(nd, ftrg, strg)      \
        set_t __s;                             \
        set_init(&__s, SET_NO_CMP);            \
        set_add(&__s, ftrg);                   \
        set_add(&__s, strg);                   \
        RST_restore_code(stdout, nd, &__s, 0); \
        set_free(&__s);                        \

/*
ASTWLKR_ro_assign checks illegal read-only assign.
Example:
```cpl
    ro i32 a = 10;
    a = 11; : <= Will fire a warning :
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_ro_assign(AST_VISITOR_ARGS);

/*
ASTWLKR_rtype_assign checks function's return type and new location's type.
Example:
```cpl
    function a() -> i32 { return 0; }
    i8 b = a(); : <= Will fire a warning :
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_rtype_assign(AST_VISITOR_ARGS);

/*
ASTWLKR_not_init checks if declaration of new variable invoked without initial value.
Example:
```cpl
    i32 a; : <= Will fire a warning :
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_not_init(AST_VISITOR_ARGS);

/*
ASTWLKR_no_exit checks if start block has exit statement at every path.
Example:
```cpl
    start {
        if 1; {
            exit 1;
        }
        : <= Will fire a warning :
    }
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_no_exit(AST_VISITOR_ARGS);

/*
ASTWLKR_wrong_arg_type checks provided arguments into the function call body.
Example:
```cpl
    function a(i32 b, i8 c) -> i0 { return; }
    a(123, 1000); : <= Will fire a warning : 
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_wrong_arg_type(AST_VISITOR_ARGS);

/*
ASTWLKR_duplicated_branches checks if branches are similar to each other.
Note: This approach mostly bases on the Merkle Tree approache.
      See: https://en.wikipedia.org/wiki/Merkle_tree
For instance the next code:
```cpl
    if 1; {
        i32 a = 10;
        return 1;
    }
    else {
        i32 a = 10;
        return 1;
    }
```

will produce branch redundancy warning.
Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_duplicated_branches(AST_VISITOR_ARGS);

/*
ASTWLKR_valid_function_name checks function name for next options:
- Fire a warning if function's name starts with underscore.
  Example: "_fname", "__fname1".
- Fire a warning if function's name isn't in snake-case.
  Example: "camaleCase", "PascalCase", "cebab-case".

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_valid_function_name(AST_VISITOR_ARGS);

/*
Check the function's return type is matching to the actual return's value type.
For instance:
```cpl
    function foo() -> i0 { return 1; } : i0 implies there is no a return value, but we are returning something! :
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_wrong_rtype(AST_VISITOR_ARGS);

/*
Check if there is a dead code.
For instance:
```cpl
    function foo() {
        return 1;
        i32 a = 1; : <= Dead code! :
        return a;
    }
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_deadcode(AST_VISITOR_ARGS);

/*
This checker checks for inefficient while statements such as statements with a constant value.
Example:
```cpl
    while 1; {
    }
    :
    Can be replaced with a
    loop {
    }
    :
```

Such statements can be easily replaced with the 'loop' statement. It actually increases the final
performance.

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_inefficient_while(AST_VISITOR_ARGS);

/*
This checker checks for the wrong exit operation in the entry function.
Main idea that the entry function must use the 'exit' statement. If it uses
the 'return' statement instead, it may cause errors.
For instance:
```cpl
    : There is no 'start' function :
    function main() -> u8 {
        return 0; : <= Will raise a error! Need to change it to the 'exit' statement! :
    }
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_wrong_exit(AST_VISITOR_ARGS);

/*
This checker checks for a 'break' statement usage in a wrong context.
The 'wrong context' means a usage out from the 'while's, 'switch's, 'loop's, etc.
For instance:
```cpl
break; : <= This is the error :
```

But the code below is a correct code:
```cpl
loop {
    break;
}
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_break_without_statement(AST_VISITOR_ARGS);

/*
This checker checks for correctness of align for variables.
For instance:
```cpl
align (7) glob i32 a;
```

May be a mistake. We need fire a warning.
Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_incorrect_align(AST_VISITOR_ARGS);

/*
Check whether this switch ifficient or not. It depends on cases count
and annotations.
Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Returns 1 if the node is correct, otherwise 0.
*/
int ASTWLKR_inefficient_switch(AST_VISITOR_ARGS);

#endif