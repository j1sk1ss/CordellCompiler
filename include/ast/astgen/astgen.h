/* Main parser logic. This parser bases on the BNF that is provided below:
program        = "{" , { top_item } , "}" ;

top_item       = start_function
                 | import_op
                 | extern_op
                 | storage_opt , top_decl ;

import_op      = "from" , string_literal , "import" , [ import_list ] ;
import_list    = import_item , { "," , import_item } ;
import_item    = identifier ;

extern_op          = "extern" , ( function_prototype | var_prototype ) ;
var_prototype      = type , identifier ;
function_prototype = "exfunc" , identifier ;

storage_opt    = [ "glob" | "ro" ] ;
top_decl       = var_decl | function_def ;

function_def   = "function" , identifier , "(" , [ param_list ] , ")" , "=>" , type , block ;
start_function = "start" , "(" , [ param_list ] , ")" , block ;

param_list     = param , { "," , param } ;
param          = type , identifier , [ "=" , literal ] ;

block          = "{" , { statement } , "}" ;

statement      = var_decl
               | if_statement
               | loop_statement
               | while_statement
               | switch_statement
               | return_statement
               | exit_statement
               | break_statement
               | lis_statement
               | syscall_statement
               | asm_block
               | comment
               | block
               | expression_statement ;

var_decl       = type , identifier , [ "=" , expression ] , ";" ;

arr_decl       = "arr" , identifier , "[" , identifier | literal , type ,  "]" , [ "=" , expression | arr_value ] , ";" ;
arr_value      = "{" , [arr_value_list] ,  "}" ;
arr_value_list =  arr_elem , { "," , arr_elem  } ;
arr_elem       = identifier | literal ;

if_statement    = "if" , expression , ";" , block , [ "else" , block ] ;
loop_statement  = "loop", block ;
while_statement = "while" , expression , ";" , block ;

switch_statement = "switch" , "(" , expression , ")" , "{" , { case_block } , [ default_block ] , "}" ;
case_block       = "case" , literal , ";" , block ;
default_block    = "default" , block ;

return_statement = "return" , [ expression ] , ";" ;
exit_statement   = "exit" , expression , ";" ;
break_statement  = "break" , ";" ;
lis_statement    = "lis" , ";" ;

expression_statement = expression , ";" ;

syscall_statement = "syscall" , "(" , [ expression_list ] , ")" , ";" ;
expression_list   = expression , { "," , expression } ;

asm_block      = "asm" , "(" , [ asm_args ] , ")" , "{" , { asm_line } , "}" ;
asm_args       = asm_arg , { "," , asm_arg } ;
asm_arg        = identifier | literal ;
asm_line       = string_literal , [ "," ] ;

comment        = ":" , { any_char_except_colon } , ":" ;

expression     = assign ;

assign         = logical_or , [ assign_op , assign ] ;
assign_op      = "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "|=" | "^=" | "&=" | "||=" | "&&=" ;

logical_or     = logical_and , { "||" , logical_and } ;
logical_and    = bit_or      , { "&&" , bit_or } ;

bit_or         = bit_xor     , { "|"  , bit_xor } ;
bit_xor        = bit_and     , { "^"  , bit_and } ;
bit_and        = equality    , { "&"  , equality } ;

equality       = relational  , { ("==" | "!=") , relational } ;
relational     = shift       , { ("<" | "<=" | ">" | ">=") , shift } ;
shift          = add         , { ("<<" | ">>") , add } ;
add            = mul         , { ("+" | "-") , mul } ;
mul            = unary       , { ("*" | "/" | "%") , unary } ;

unary          = unary_op , unary
               | postfix ;

unary_op       = "not" | "+" | "-" | ref_op | dref_op ;

ref_op         = "ref" ;
dref_op        = "dref" ;

postfix        = primary , { postfix_op } ;
postfix_op     = "(" , [ arg_list ] , ")"
               | "[" , expression , { "," , expression } , "]" ;

primary        = literal
               | identifier
               | "(" , expression , ")" ;

arg_list       = expression , { "," , expression } ;

type = "f64" | "i64" | "u64" | "f32" | "i32" | "u32" | "i16" | "u16" | "i8" | "u8"
     | "str"
     | "arr" , "[" , integer_literal , "," , type , "]"
     | "ptr" , type ;

literal         = integer_literal | string_literal | boolean_literal ;

integer_literal = decimal_literal | hex_literal | bin_literal ;

decimal_literal = digit , { digit } ;
hex_literal     = "0x" , hex_digit , { hex_digit } ;
bin_literal     = "0b" , bin_digit , { bin_digit } ;

hex_digit = digit
          | "a" | "b" | "c" | "d" | "e" | "f"
          | "A" | "B" | "C" | "D" | "E" | "F" ;

bin_digit = "0" | "1" ;

identifier      = letter , { letter | digit | "_" } ;
string_literal  = '"' , { any_char_except_quote } , '"' ;

letter = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" 
       | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
       | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m"
       | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" ;

digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
*/

#ifndef CPL_PARSER_H_
#define CPL_PARSER_H_

#include <std/str.h>
#include <std/stack.h>
#include <prep/dict.h>
#include <prep/token.h>
#include <prep/token_types.h>
#include <prep/dict.h>
#include <symtab/symtab.h>
#include <ast/ast.h>
#include <ast/astgen.h>

#define SAVE_TOKEN_POINT    void* __dump_tkn = it->curr
#define RESTORE_TOKEN_POINT it->curr = __dump_tkn

/* Support macro for getting the current token from the iterator. */
#define CURRENT_TOKEN ((token_t*)list_iter_current(it))
#define NEXT_TOKEN    ((token_t*)list_iter_next_top(it))

#define PARSE_ERROR(msg, ...) \
    fprintf( \
        stderr,                                          \
        "[%li:%li] " msg "\n",                           \
        CURRENT_TOKEN ? CURRENT_TOKEN->finfo.line : 0,   \
        CURRENT_TOKEN ? CURRENT_TOKEN->finfo.column : 0, \
        ##__VA_ARGS__                                    \
    )

/*
Search for a variable (presented in the node) on the symtable.
Params:
    - `node` - Considering node.
    - `ctx` - AST context.
    - `smt` - Symtable.

Return 1 if succeed.
*/
int var_lookup(ast_node_t* node, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse `.cpl` block with input tokens. Should be invoked on new block.
Note: This is the start gramma symbol (see EBNF in the README).
Snippet:
```cpl
someting {
    : Block :
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.
    - `ex` - Exit token type, that will end block parsing.

Returns an ast node.
*/
ast_node_t* cpl_parse_block(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, token_type_t ex);

/*
Parse .cpl asm block with input tokens. Should be invoked on new ASM token.
Snippet:
```cpl
asm( : arguments, statements : ) {
    "",
    : ... :
    ""
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_asm(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl switch block with input tokens. Should be invoked on switch token.
Snippet:
```cpl
switch : statement : {
    case : value :; {

    }
    default {

    }
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_switch(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl 'if' block with input tokens. Should be invoked on 'if' token.
Snippet:
```cpl
if : statement :; {
}
else {
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_if(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl 'while' block with input tokens. Should be invoked on 'while' token.
Snippet:
```cpl
while : statement :; {
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_while(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl 'loop' block with input tokens. Should be invoked on 'loop' token.
Snippet:
```cpl
loop {
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_loop(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl declaration array block. Should be invoked on array declaration block.
Snippet:
```cpl
arr : name :[: type :, : size :] (opt: = : decl :);
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_array_declaration(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl declaration variable block. Should be invoked on variable declaration block.
Snippet:
```cpl
: type : : name : (opt: = : decl :);
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_variable_declaration(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl extern block. Should be invoked on extern block.
Snippet:
```cpl
extern : type : : name :;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_extern(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl exit block. Should be invoked on a 'exit' token.
Snippet:
```cpl
exit : statement :;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_exit(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl return block. Should be invoked on a 'return' token.
Snippet:
```cpl
return : statement :;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_return(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl function call. Should be invoked on funccall token.
Snippet:
```cpl
: function name :( : statement : );
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_funccall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl function with body and params. Should be invoked on function entry body.
Snippet:
```cpl
function : name :( : type : : name : (opt: = : decl :) ) {
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_function(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl import block. Should be invoked on import token.
Snippet:
```cpl
from : file : import : name :;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_import(list_iter_t* it, sym_table_t* smt);

/*
Parse .cpl expression block (function, arithmetics, etc.). Can be invoked on any token type.
Snippet:
```cpl
: statement : : op : : statement :;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.
    - `na` - No assign.
             Note: By default (0), this function parses an entire 
                   expression with a assign symbol. That means, that
                   expressions such as `a = b`, `a + b = a + b` will
                   be full parsed.
                   If you want to parse only the left part (before assign),
                   set this flag to 1.

Returns an ast node.
*/
ast_node_t* cpl_parse_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, int na);

/*
Parse .cpl scope block. Should be invoked on scope token.
Snippet:
```cpl
{
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_scope(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl start block. Should be invoked on start token.
Snippet:
```cpl
start( : arguments : ) {
}
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_start(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl syscall block. Should be invoked on syscall token.
Snippet:
```cpl
syscall( : arguments : );
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_syscall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl breakpoint block. Should be invoked on a breakpoint token.
Snippet:
```cpl
lis;
```

Params:
    - `it` - Current iterator on token list.

Returns an ast node.
*/
ast_node_t* cpl_parse_breakpoint(list_iter_t* it);

/*
Parse .cpl break block. Should be invoked on a break token.
Snippet:
```cpl
break;
```

Params:
    - `it` - Current iterator on token list.

Returns an ast node.
*/
ast_node_t* cpl_parse_break(list_iter_t* it);

/*
Parse .cpl cast block. Should be invoked on a 'as' token.
Snippet:
```cpl
i32 b = variable as i32;
```

Params:
    - `it` - Current iterator on token list.

Returns an ast node.
*/
ast_node_t* cpl_parse_conv(list_iter_t* it);

/*
Parse .cpl 'ref' command. Should be invoked on a 'ref' token.
Snippet:
```cpl
i32 b = ref variable;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_ref(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl 'dref' command. Should be invoked on a 'dref' token.
Snippet:
```cpl
i32 b = dref variable;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_dref(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl 'neg' command. Should be invoked on a 'neg' token.
Snippet:
```cpl
i32 b = neg variable;
```

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_neg(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

#endif