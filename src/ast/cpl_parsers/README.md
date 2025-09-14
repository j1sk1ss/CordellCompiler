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
├─ [...]
```

- Import token (`import`)
```
import_token
└─ src
   ├─ fname1 (str)
   ├─ fname2 (str)
   └─ [...]
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
└─ [...]
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
   │    └─ [...]
   └─ [...]
```

- Condition scope (`if`, `while`)
```
cond_node_token
├─ condition (val | exp)
├─ true_branch (scope)
│  └─ [...]
└─ false_branch (scope)
   └─ [...]
```

- Function definition token (`function`)
```
func_token
├─ name (str)
│ └─ type (str)
└─ scope (scope)
  ├─ arg1 (type)
  │ ├─ name (str)
  │ └─ def_val (exp)
  ├─ [...]
  └─ scope (scope)
    ├─ body1 (exp)
    └─ [...]
```

>> Note: Default values saved in function table. When we call this function, all "empty" cells in args wil be replaced by arguments from default.

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
└─ [...]
```

- Syscall token (`syscall`)
```
syscall_token
├─ arg1 (val | exp)
├─ arg2 (val | exp)
└─ [...]
```

- ASM block token (`asm`)
```
asm_token
├ arg1 (val)
├ [...]
└ body (scope)
 ├─ body1 (exp)
 └─ [...]
```

# EBNF
```
program         = "{" , { global_decl | function_def | start_function } , "}" ;

global_decl     = [ "glob" | "ro" ] , ( var_decl | function_def ) ;

function_def    = [ "glob" ] , "function" , identifier , "(" , [ param_list ] , ")" , "=>" , type , block ;

start_function  = "start" , "(" , [ param_list ] , ")" , block ;

param_list      = param , { "," , param } ;
param           = type , identifier , [ "=" , literal ] ;

block           = "{" , { statement } , "}" ;

statement       = var_decl
                | assignment
                | if_statement
                | while_statement
                | switch_statement
                | return_statement
                | exit_statement
                | expression_statement
                | asm_block
                | comment ;

var_decl        = type , identifier , [ "=" , expression ] , ";" ;
assignment      = identifier , "=" , expression , ";" ;

if_statement    = "if" , expression , ";" , block , [ "else" , block ] ;
while_statement = "while" , expression , ";" , block , [ "else" , block ] ;

switch_statement = "switch" , "(" , expression , ")" , "{" , { case_block } , [ default_block ] , "}" ;
case_block       = "case" , literal , ";" , block ;
default_block    = "default" , block ;

return_statement   = "return" , [ expression ] , ";" ;
exit_statement     = "exit" , literal , ";" ;
expression_statement = expression , ";" ;

asm_block       = "asm" , "(" , { identifier | literal } , ")" , "{" , { asm_line } , "}" ;
asm_line        = string_literal , [ "," ] ;

comment         = ":" , { any_char_except_colon } , ":" ;

expression      = literal
                | identifier
                | "(" , expression , ")"
                | expression , binary_op , expression
                | unary_op , expression
                | function_call
                | array_access ;

function_call   = identifier , "(" , [ arg_list ] , ")" ;
arg_list        = expression , { "," , expression } ;
array_access    = identifier , "[" , expression , { "," , expression } , "]" ;

binary_op       = "+" | "-" | "*" | "/" | "%" 
                | "==" | "!=" | "<" | "<=" | ">" | ">="
                | "&&" | "||"
                | ">>" | "<<" | "&" | "|" | "^" ;

unary_op        = "not" | "-" | "+" ;

type            = "f64" | "i64" | "u64" | "f32" | "i32" | "u32" | "i16" | "u16" | "i8" | "u8"
                | "str"
                | "arr" , "[" , integer_literal , "," , type , "]"
                | "ptr" , type ;

literal         = integer_literal | string_literal | boolean_literal ;

identifier      = letter , { letter | digit | "_" } ;
integer_literal = digit , { digit } ;
string_literal  = '"' , { any_char_except_quote } , '"' ;
boolean_literal = "true" | "false" ;

letter = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" 
       | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
       | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m"
       | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" ;

digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
```
