# Syntax trees
This directory contains parsers for `CPL` language. CordellCompiler itself support different parsers for different languages. Parser should use default `API` and should be integrated into source code.

## Terminology
- exp - Expression presented as another `AST` node.
- val - Single value (Numeric, string or character). `AST` nodes not permitted.
    - int - Numeric value (Numeric, `long`, `int`, `short`, `char`).
    - str - String value (String, `str`, `arr`, `ptr`).
- scope - Separated scope (`{`, `}`).
- exp | val - Single value or `AST` node.

## Token parsers
- Scope token (`{` and `}`)
```
scope (scope)
├─ exp1 (exp)
├─ exp2 (exp)
├─ ...
```

- Import token (`import`)
```
import_token
└─ src
   ├─ fname1 (str)
   ├─ fname2 (str)
   └─ ...
```

- Variable declaration (`str`, `long`, `int`, `short`, `char`)
```
type_token
├─ name (str)
└─ decl_val (val | exp)
```

- Array declaration (`arr`)
```
arr_token
├─ size (int)
├─ el_size (int)
├─ name (str)
├─ element (val | exp)
└─ ...
```

- Binary operation (`+`, `-`, `=`, `/`, `%`, `*`, `&&`, `||`, `|`, `&`, `>`, `<`, `!=`, `==`)
```
operator_token
├─ left_var (val | exp)
└─ right_var (val | exp)
```

- Binary assign operation (`=`) with array (offset)
```
assign_op
├─ arr_name (str)
│ └─ offset (val | exp)
└─ assign_val (val | exp)
```

- Switch token (`switch`)
```
switch_token
├─ stmt (val | exp)
└─ cases (scope)
   ├─ case_stmt (val | exp)
   │ └─ case_body (scope)
   │    └─ ...
   └─ ...
```

- Condition scope (`if`, `while`)
```
cond_node_token
├─ condition (val | exp)
├─ true_branch (scope)
│  └─ ...
└─ false_branch (scope)
   └─ ...
```

- Function definition token (`function`)
```
func_token
├─ name (str)
└─ scope (scope)
  ├─ arg1
  │ ├─ type
  │ └─ name (str)
  ├─ ...
  └─ scope (scope)
    ├─ body1 (exp)
    └─ ...
```

- Return / exit statement (`return`, `exit`)
```
exit/return_token
└─ ret_val (val | exp)
```

- Function call token (defined by `function` key name)
```
call_token
├─ arg1 (val | exp)
├─ arg2 (val | exp)
└─ ...
```

- Syscall token (`syscall`)
```
syscall_token
├─ arg1 (val | exp)
├─ arg2 (val | exp)
└─ ...
```
