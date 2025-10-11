# HIR part
Now we need to convert our `AST` into a simpler representation. A common approach here is to convert the `AST` into `Three-Address Code` (3AC). In this compiler, for common operations, the first argument is the destination, and the next two are sources. However, when it comes to function parsing, arrays, or assembly blocks, a different approach is used.
![markup](../../media/HIR.png)

The implementation of the HIR generator from the AST source is not difficult, as we simply convert familiar AST nodes into a linear representation. The main logic of this process in this compiler is located [here](https://github.com/j1sk1ss/CordellCompiler/tree/HIR_LIR_SSA/src/hir), and let’s examine the following code snippet that contains the core logic.

```c
static hir_subject_t* _generation_handler(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node || !node->token) return NULL;
    if (
        TKN_isoperand(node->token) && node->token->t_type != ASSIGN_TOKEN
    ) return HIR_generate_operand(node, ctx, smt);
    switch (node->token->t_type) {
        case CALL_TOKEN:            return HIR_generate_funccall(node, ctx, smt, 1);
        case SYSCALL_TOKEN:         return HIR_generate_syscall(node, ctx, smt, 1);
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        case STRING_VALUE_TOKEN:
        case UNKNOWN_NUMERIC_TOKEN: return HIR_generate_load(node, ctx, smt);
        default: break;
    }

    return NULL;
}

static int _navigation_handler(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node || !node->token) return 0;
    if (TKN_isdecl(node->token)) return HIR_generate_declaration_block(node, ctx, smt);
    if (
        TKN_update_operator(node->token) && node->token->t_type != ASSIGN_TOKEN
    ) return HIR_generate_update_block(node, ctx, smt);
    switch (node->token->t_type) {
        case IF_TOKEN:      HIR_generate_if_block(node, ctx, smt);         break;
        case ASM_TOKEN:     HIR_generate_asmblock(node, ctx, smt);         break;
        case FUNC_TOKEN:    HIR_generate_function_block(node, ctx, smt);   break;
        case EXIT_TOKEN:    HIR_generate_exit_block(node, ctx, smt);       break;
        case CALL_TOKEN:    HIR_generate_funccall(node, ctx, smt, 0);      break;
        case WHILE_TOKEN:   HIR_generate_while_block(node, ctx, smt);      break;
        case START_TOKEN:   HIR_generate_start_block(node, ctx, smt);      break;
        case SWITCH_TOKEN:  HIR_generate_switch_block(node, ctx, smt);     break;
        case RETURN_TOKEN:  HIR_generate_return_block(node, ctx, smt);     break;
        case EXTERN_TOKEN:  HIR_generate_extern_block(node, ctx, smt);     break;
        case IMPORT_TOKEN:  HIR_generate_import_block(node, ctx, smt);     break;
        case ASSIGN_TOKEN:  HIR_generate_assignment_block(node, ctx, smt); break;
        case SYSCALL_TOKEN: HIR_generate_syscall(node, ctx, smt, 0);       break;
        default: break;
    }

    return 1;
}
```

Note: In the HIR part, we encounter implicit casting, which generates those strange artifacts such as:
```ssa
tmp1 = 10 as i32;
a1 = tmp1;
```

## HIR optimization
- Constant folding
- HIR Peephole optimization

## Example of HIR
```
{
    fn sum(i32 a, i32 b) -> i32
    {
        alloc i32s a0;
        load_arg(i32s a0);
        alloc i32s b1;
        load_arg(i32s b1);
        {
            prm_st(i32s a0);
            prm_st(i32s b1);
            alloc arrs c2, size: n2;
            i32t tmp12 = arrs c2[n0];
            i32t tmp13 = arrs c2[n1];
            i32t tmp14 = i32t tmp12 + i32t tmp13;
            return i32t tmp14;
        }
    }
    
    start {
        alloc i64s argc3;
        load_starg(i64s argc3);
        alloc u64s argv4;
        load_starg(u64s argv4);
        {
            alloc i32s a5;
            i32t tmp15 = n10 as i32;
            i32s a5 = i32t tmp15;
            alloc i32s b6;
            i32t tmp16 = n10 as i32;
            i32s b6 = i32t tmp16;
            alloc i32s c7;
            i32t tmp17 = n10 as i32;
            i32s c7 = i32t tmp17;
            alloc i32s d8;
            i32t tmp18 = n10 as i32;
            i32s d8 = i32t tmp18;
            alloc i32s k9;
            i32t tmp19 = n10 as i32;
            i32s k9 = i32t tmp19;
            alloc i32s f10;
            i32t tmp20 = n10 as i32;
            i32s f10 = i32t tmp20;
            store_arg(i32s a5);
            store_arg(i32s b6);
            i32t tmp21 = call sum(i32 a, i32 b) -> i32, argc c2;
            i32t tmp22 = i32s a5 * i32s b6;
            i32t tmp23 = i32t tmp22 + i32s c7;
            i32t tmp24 = i32t tmp23 + i32s d8;
            i32t tmp25 = i32t tmp24 + i32s k9;
            i32t tmp26 = i32t tmp25 + i32s f10;
            i32t tmp27 = i32t tmp21 > i32t tmp26;
            if i32t tmp27, goto l73;
            {
                exit n1;
            }
            l73:
            alloc i32s l11;
            u64t tmp28 = &(i32s f10);
            i32t tmp29 = u64t tmp28 as i32;
            i32s l11 = i32t tmp29;
            exit i32s l11;
        }
    }
}
```