#ifndef AST_VISITORS_H_
#define AST_VISITORS_H_

#include <stdio.h>
#include <limits.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen.h>

#define AST_VISITOR_ARGS ast_node_t* nd, sym_table_t* smt
#define SEMANTIC_ERROR(message, ...)   fprintf(stdout, "[ERROR] " message "\n", ##__VA_ARGS__)
#define SEMANTIC_WARNING(message, ...) fprintf(stdout, "[WARNING] " message "\n", ##__VA_ARGS__)
#define SEMANTIC_INFO(message, ...)    fprintf(stdout, "[INFO] " message "\n", ##__VA_ARGS__)

/*
ASTWLKR_ro_assign checks illegal read-only assign.
Example:

```
    ro i32 a = 10;
    a = 11; : <= Will fire a warning :
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_ro_assign(AST_VISITOR_ARGS);

/*
ASTWLKR_rtype_assign checks function's return type and new location's type.
Example:

```
    function a() => i32 { return 0; }
    i8 b = a(); : <= Will fire a warning :
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_rtype_assign(AST_VISITOR_ARGS);

/*
ASTWLKR_not_init checks if declaration of new variable invoked without initial value.
Example:

```
    i32 a; : <= Will fire a warning :
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_not_init(AST_VISITOR_ARGS);

/*
ASTWLKR_illegal_declaration checks initial value of declaration node. 
If value has different with declaration type, will file a warning.


```
    i8 a = 123123; : <= Will fire a warning :
    i8 b = 123;
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_illegal_declaration(AST_VISITOR_ARGS);

/*
ASTWLKR_no_return checks if function block has return statement at every path.
Example:

```
    function a() => i0 {
        if 1; {
            return;
        }
        : <= Will fire a warning :
    }
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_no_return(AST_VISITOR_ARGS);

/*
ASTWLKR_no_exit checks if start block has exit statement at every path.
Example:

```
    start {
        if 1; {
            exit 1;
        }
        : <= Will fire a warning :
    }
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_no_exit(AST_VISITOR_ARGS);

/*
ASTWLKR_not_enough_args checks provided argument's count in function call body.
Example:

```
    function a(i32 b) => i0 { return; }
    a(100, 100); : <= Will fire a warning : 
    a(); : <= Will fire a warning : 
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_not_enough_args(AST_VISITOR_ARGS);

/*
ASTWLKR_wrong_arg_type checks provided arguments into the function call body.
Example:

```
    function a(i32 b, i8 c) => i0 { return; }
    a(123, 1000); : <= Will fire a warning : 
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_wrong_arg_type(AST_VISITOR_ARGS);

/*
ASTWLKR_unused_rtype will fire warning in situation when return value from function.
Example:

```
    function a() => i32 { return 0; }
    a(); : <= Will fire a warning : 
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_unused_rtype(AST_VISITOR_ARGS);

/*
ASTWLKR_illegal_array_access checks out-bound array access.
Example: 

```
    arr a[i32, 10];
    a[-1]; a[11];
```

Params:
    - AST_VISITOR_ARGS - Default AST visitor args.

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_illegal_array_access(AST_VISITOR_ARGS);

/*
ASTWLKR_duplicated_branches checks if branches are similar to each other.
For instance next code:

```
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

Return 1 if node is correct, otherwise this function will return 0.
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

Return 1 if node is correct, otherwise this function will return 0.
*/
int ASTWLKR_valid_function_name(AST_VISITOR_ARGS);

#endif