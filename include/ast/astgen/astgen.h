/* Main parser logic. This parser bases on the BNF that is provided below:
identifier       = "IDENTIFIER" ;
integer_literal  = "INTEGER_LITERAL" ;
float_literal    = "FLOAT_LITERAL" ;
string_literal   = "STRING_LITERAL" ;
char_literal     = "CHAR_LITERAL" ;
comment          = "COMMENT" ;
eol              = "EOL" ;

program        = "{" , { top_item } , "}" ;

top_item       = start_function
               | import_op
               | extern_op
               | pp_directive
               | storage_opt , top_decl
               | section_stmt
               | align_stmt ;

import_op      = "from" , string_literal , "import" , [ import_list ] ;
import_list    = import_item , { "," , import_item } ;
import_item    = identifier ;

extern_op = "extern" , ( extern_function_proto | var_prototype ) ;
extern_function_proto = "function" , identifier , "(" , [ param_list ] , ")" , [ "->" , type ] , ";" ;
var_prototype      = type , identifier ;

storage_opt    = [ "glob" | "ro" ] ;

top_decl       = declaration | function_def | function_proto ;

declaration    = var_decl | arr_decl ;

section_stmt   = "section" , "(" , string_literal , ")" , "{" , { section_item } , "}" ;
section_item   = [ storage_opt ] , ( declaration | function_def | function_proto )
               | align_stmt
               | pp_directive ;

align_stmt       = "align" , "(" , integer_literal , ")" , ( declaration | align_decl_block ) ;
align_decl_block = "{" , { declaration } , "}" ;

function_def   = "function" , identifier , "(" , [ param_list ] , ")" , [ "->" , type ] , block ;
function_proto = "function" , identifier , "(" , [ param_list ] , ")" , [ "->" , type ] , ";" ;
start_function = "start" , "(" , [ param_list ] , ")" , block ;

param_list     = param , { "," , param } ;
param          = type , identifier , [ "=" , expression ] | "..." ;

block          = "{" , { block_item } , "}" ;

block_item     = statement
               | function_def
               | function_proto ;

statement =
    "arr" , arr_stmt_tail
  | pp_directive
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
  | align_stmt
  | comment
  | block
  | var_decl_starting_not_arr
  | expression_statement ;

arr_stmt_tail =
    identifier , "[" , integer_literal , "," , type , "]" , [ "=" , ( expression | arr_value ) ] , ";"
  | "[" , integer_literal , "," , type , "]" , identifier , [ "=" , expression ] , ";" ;

pp_directive   = "#" , pp_body , pp_end ;
pp_end         = eol | ";" ;
pp_body        = pp_line
               | pp_include
               | pp_define
               | pp_undef
               | pp_ifdef
               | pp_ifndef
               | pp_endif ;

pp_line        = "line" , integer_literal , [ string_literal ] ;
pp_include     = "include" , string_literal ;
pp_define      = "define" , identifier , { pp_token } ;
pp_undef       = "undef" , identifier ;
pp_ifdef       = "ifdef" , identifier ;
pp_ifndef      = "ifndef" , identifier ;
pp_endif       = "endif" ;

pp_token       = identifier
               | literal
               | keyword
               | punct_no_semi
               | operator ;

keyword        = "from" | "import" | "extern" | "exfunc" | "glob" | "ro"
               | "function" | "start"
               | "arr"
               | "if" | "else" | "loop" | "while"
               | "switch" | "case" | "default"
               | "return" | "exit" | "break" | "lis"
               | "syscall" | "asm"
               | "not" | "ref" | "dref" | "as"
               | "ptr" | "str"
               | "f64" | "i64" | "u64" | "f32" | "i32" | "u32" | "i16" | "u16" | "i8" | "u8"
               | "line" | "include" | "define" | "undef" | "ifdef" | "ifndef" | "endif"
               | "section" | "align" ;

punct_no_semi  = "{" | "}" | "(" | ")" | "[" | "]" | "," ;

operator       = "->"
               | "==" | "!=" | "<=" | ">="
               | "<<" | ">>"
               | "||" | "&&"
               | "+=" | "-=" | "*=" | "/=" | "%=" | "|=" | "^=" | "&=" | "||=" | "&&="
               | "="  | "<"  | ">"  | "+"  | "-"  | "*"  | "/"  | "%"  | "|"  | "^"  | "&" ;

var_decl       = type , identifier , [ "=" , expression ] , ";" ;

var_decl_starting_not_arr = non_arr_type , identifier , [ "=" , expression ] , ";" ;

non_arr_type   = "f64" | "i64" | "u64" | "f32" | "i32" | "u32" | "i16" | "u16" | "i8" | "u8"
               | "str"
               | "ptr" , type ;

arr_decl       = "arr" , identifier , "[" , integer_literal , "," , type , "]" ,
                 [ "=" , ( expression | arr_value ) ] , ";" ;

arr_value      = "{" , [ arr_value_list ] , "}" ;
arr_value_list = expression , { "," , expression } ;

if_statement     = "if" , expression , ";" , statement , [ "else" , statement ] ;
loop_statement   = "loop" , statement ;
while_statement  = "while" , expression , ";" , statement ;

switch_statement = "switch" , expression , ";" ,
                   "{" , { case_block } , [ default_block ] , "}" ;
case_block       = "case" , literal , ";" , block ;
default_block    = "default" , [ ";" ] , block ;

return_statement = "return" , [ expression ] , ";" ;
exit_statement   = "exit" , expression , ";" ;
break_statement  = "break" , ";" ;
lis_statement    = "lis" , [ string_literal ] , ";" ;

expression_statement = expression , ";" ;

syscall_statement = "syscall" , "(" , [ expression_list ] , ")" , ";" ;
expression_list   = expression , { "," , expression } ;

asm_block      = "asm" , "(" , [ asm_args ] , ")" , "{" , { asm_line } , "}" ;
asm_args       = asm_arg , { "," , asm_arg } ;
asm_arg        = identifier | literal ;
asm_line       = string_literal , [ "," ] ;

expression     = assign ;

assign         = logical_or , [ assign_op , assign ] ;
assign_op      = "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "|=" | "^=" | "&=" | "||=" | "&&=" ;

logical_or     = logical_and , { "||" , logical_and } ;
logical_and    = bit_or      , { "&&" , bit_or } ;

bit_or         = bit_xor     , { "|"  , bit_xor } ;
bit_xor        = bit_and     , { "^"  , bit_and } ;
bit_and        = equality    , { "&"  , equality } ;

equality       = relational  , { ( "==" | "!=" ) , relational } ;
relational     = shift       , { ( "<" | "<=" | ">" | ">=" ) , shift } ;
shift          = add         , { ( "<<" | ">>" ) , add } ;
add            = mul         , { ( "+" | "-" ) , mul } ;
mul            = unary       , { ( "*" | "/" | "%" ) , unary } ;

unary          = unary_op , unary
               | postfix ;

unary_op       = "not" | "+" | "-" | ref_op | dref_op ;
ref_op         = "ref" ;
dref_op        = "dref" ;

postfix        = primary , { postfix_op } ;
postfix_op     = "(" , [ arg_list ] , ")"
               | "[" , expression , { "," , expression } , "]"
               | "as" , type ;

primary        = literal
               | identifier
               | "(" , expression , ")" ;

arg_list       = expression , { "," , expression } ;

type           = "f64" | "i64" | "u64" | "f32" | "i32" | "u32" | "i16" | "u16" | "i8" | "u8"
               | "str"
               | "arr" , "[" , integer_literal , "," , type , "]"
               | "ptr" , type ;

literal        = integer_literal | float_literal | string_literal | char_literal ;
*/

#ifndef CPL_PARSER_H_
#define CPL_PARSER_H_

#include <config.h>
#include <std/str.h>
#include <std/set.h>
#include <std/stack.h>
#include <prep/dict.h>
#include <prep/token.h>
#include <prep/token_types.h>
#include <prep/dict.h>
#include <symtab/symtab.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <ast/astgen/annot.h>

#define SAVE_TOKEN_POINT    void* __dump_tkn = it->curr;
#define RESTORE_TOKEN_POINT it->curr = __dump_tkn;

/* Support macro for getting the current token from the iterator. */
#define CURRENT_TOKEN      ((token_t*)list_iter_current(it))
#define CREATE_SCOPE_TOKEN TKN_create_token(SCOPE_TOKEN, NULL, &CURRENT_TOKEN->finfo)
#define CREATE_INDEX_TOKEN TKN_create_token(INDEXATION_TOKEN, NULL, &CURRENT_TOKEN->finfo)
#define CREATE_CALL_TOKEN  TKN_create_token(CALLING_TOKEN, NULL, &CURRENT_TOKEN->finfo)

#define PARSE_ERROR(msg, ...) \
    fprintf( \
        stderr,                                                                                  \
        "[%s:%li:%li] " msg "\n",                                                                \
        (CURRENT_TOKEN && CURRENT_TOKEN->finfo.file) ? CURRENT_TOKEN->finfo.file->body : "base", \
        CURRENT_TOKEN ? CURRENT_TOKEN->finfo.line : 0,                                           \
        CURRENT_TOKEN ? CURRENT_TOKEN->finfo.column : 0,                                         \
        ##__VA_ARGS__                                                                            \
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

#define PARSER_ARGS list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, long carry
#define PARSER_ARGS_USE (void)it; (void)ctx; (void)smt; (void)carry;

#define PRESERVE_AST_CARRY_ARG(l, n) \
    void* __dumped = ctx->carry.ptr; \
    ctx->carry.ptr = n;              \
    l;                               \
    ctx->carry.ptr = __dumped;       \

/*
Parse `.cpl` element with input tokens.
Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_element(PARSER_ARGS);

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
ast_node_t* cpl_parse_block(PARSER_ARGS);

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
ast_node_t* cpl_parse_asm(PARSER_ARGS);

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
ast_node_t* cpl_parse_switch(PARSER_ARGS);

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
ast_node_t* cpl_parse_if(PARSER_ARGS);

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
ast_node_t* cpl_parse_while(PARSER_ARGS);

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
ast_node_t* cpl_parse_loop(PARSER_ARGS);

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
ast_node_t* cpl_parse_array_declaration(PARSER_ARGS);

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
ast_node_t* cpl_parse_variable_declaration(PARSER_ARGS);

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
ast_node_t* cpl_parse_extern(PARSER_ARGS);

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
ast_node_t* cpl_parse_exit(PARSER_ARGS);

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
ast_node_t* cpl_parse_return(PARSER_ARGS);

/*
Parse .cpl function's arguments. Helper function for funccall handlers.
Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.
    - `args` - Output arguments number.

Returns an ast node.
*/
ast_node_t* cpl_parse_call_arguments(PARSER_ARGS);

/*
Parse .cpl function call. Should be invoked on funccall token.
Snippet:
```cpl
: function name :( : statement : ); : or :
: function name :;
```

Note: This parser parses only already defined 'classic' functions.
      That means, this function works only with 'function foo() / foo()', 'foo'.

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_funccall(PARSER_ARGS);

/*
Helper function for parsing function / start arguments.
Can handle declaration-like arguments and variadic arguments.
Params:
    - `trg` - Target arguments node.
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns 0 if something went wrong. Otherwise will return 1.
*/
int cpl_parse_funcdef_args(PARSER_ARGS);

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
ast_node_t* cpl_parse_function(PARSER_ARGS);

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
ast_node_t* cpl_parse_import(PARSER_ARGS);

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
ast_node_t* cpl_parse_expression(PARSER_ARGS);

/*
Parse .cpl scope element.
Note: Will parse only one element in the separated scope. This means,
      that any declared variable will be allocated in a one-line scope.

Params:
    - `it` - Current iterator on token list.
    - `ctx` - AST ctx.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_line_scope(PARSER_ARGS);

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
    - `carry` - Increase the scope id? (0 - no).

Returns an ast node.
*/
ast_node_t* cpl_parse_scope(PARSER_ARGS);

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
ast_node_t* cpl_parse_start(PARSER_ARGS);

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
ast_node_t* cpl_parse_syscall(PARSER_ARGS);

/*
Parse .cpl breakpoint block. Should be invoked on a breakpoint token.
Snippet:
```cpl
lis <msg>;
```

Params:
    - `it` - Current iterator on token list.
    - `smt` - Symtable pointer.

Returns an ast node.
*/
ast_node_t* cpl_parse_breakpoint(PARSER_ARGS);

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
ast_node_t* cpl_parse_break(PARSER_ARGS);

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
ast_node_t* cpl_parse_conv(PARSER_ARGS);

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
ast_node_t* cpl_parse_ref(PARSER_ARGS);

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
ast_node_t* cpl_parse_dref(PARSER_ARGS);

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
ast_node_t* cpl_parse_neg(PARSER_ARGS);

/*
Parse .cpl 'poparg' command. Should be invoked on a 'poparg' token.
Snippet:
```cpl
i32 a = poparg as i32;
```

Params:
    - `it` - Current iterator on token list.

Returns an ast node.
*/
ast_node_t* cpl_parse_poparg(PARSER_ARGS);

/*
Parse align operator. It supports both with and without scope syntaxes.
Also, the scope doesn't increase the iternal scope ID, which means,
all variables that were declared within this structure will live in
the same scope.
Params:
    - <parser_args>

Returns an AST node.
*/
ast_node_t* cpl_parse_align(PARSER_ARGS);

/*
Parse section operator. It supports both with and without scope syntaxes.
The same logic with the align keyword.
Params:
    - <parser_args>

Returns an AST node.
*/
ast_node_t* cpl_parse_section(PARSER_ARGS);

/*
Parse an annotation and push it onto the stack.
Params:
    - <parser_args>

Returns NULL.
*/
ast_node_t* cpl_parse_annot(PARSER_ARGS);

#endif