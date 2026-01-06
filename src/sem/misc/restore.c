#include <sem/misc/restore.h>

const char* RST_restore_type(token_t* t) {
    if (!t) return "";
    switch (t->t_type) {
        case STRING_VALUE_TOKEN:    return t->body->body;
        case ARRAY_TYPE_TOKEN:
        case ARR_VARIABLE_TOKEN:    return ARR_VARIABLE;
        case STR_TYPE_TOKEN:
        case STR_VARIABLE_TOKEN:    return !t->flags.ptr ? STR_VARIABLE : "ptr " STR_VARIABLE;
        case I0_TYPE_TOKEN:         return !t->flags.ptr ? I0_VARIABLE  : "ptr " I0_VARIABLE;
        case I8_VARIABLE_TOKEN:
        case I8_TYPE_TOKEN:         return !t->flags.ptr ? I8_VARIABLE  : "ptr " I8_VARIABLE;
        case U8_VARIABLE_TOKEN:
        case U8_TYPE_TOKEN:         return !t->flags.ptr ? U8_VARIABLE  : "ptr " U8_VARIABLE;
        case I16_VARIABLE_TOKEN:
        case I16_TYPE_TOKEN:        return !t->flags.ptr ? I16_VARIABLE : "ptr " I16_VARIABLE;
        case U16_VARIABLE_TOKEN:
        case U16_TYPE_TOKEN:        return !t->flags.ptr ? U16_VARIABLE : "ptr " U16_VARIABLE;
        case I32_VARIABLE_TOKEN:
        case I32_TYPE_TOKEN:        return !t->flags.ptr ? I32_VARIABLE : "ptr " I32_VARIABLE;
        case U32_VARIABLE_TOKEN:
        case U32_TYPE_TOKEN:        return !t->flags.ptr ? U32_VARIABLE : "ptr " U32_VARIABLE;
        case F32_VARIABLE_TOKEN:
        case F32_TYPE_TOKEN:        return !t->flags.ptr ? F32_VARIABLE : "ptr " F32_VARIABLE;
        case I64_VARIABLE_TOKEN:
        case I64_TYPE_TOKEN:        return !t->flags.ptr ? I64_VARIABLE : "ptr " I64_VARIABLE;
        case U64_VARIABLE_TOKEN:
        case U64_TYPE_TOKEN:        return !t->flags.ptr ? U64_VARIABLE : "ptr " U64_VARIABLE;
        case F64_VARIABLE_TOKEN:
        case F64_TYPE_TOKEN:        return !t->flags.ptr ? F64_VARIABLE : "ptr " F64_VARIABLE;
        default: return "";
    }
}

static inline void _print_indent(FILE* fd, int indent) {
    for (int i = 0; i < indent; ++i) fputs("    ", fd);
}

static inline void _simple_restore(FILE* fd, const char* entry, ast_node_t* nd, set_t* u, int indent) {
    fprintf(fd, "%s", entry);
    RST_restore_code(fd, nd, u, indent);
}

int RST_restore_code(FILE* fd, ast_node_t* nd, set_t* u, int indent) {
    if (!nd) return 0;
    if (u && set_has(u, nd)) fprintf(fd, "%s", UNDERSCORE_OPEN);

    int complex = -1;
    if (TKN_isdecl(nd->t)) {
        fprintf(
            fd, "%s%s%s %s", 
            nd->t->flags.glob ? "glob" : "",
            nd->t->flags.ro ? "ro" : "",
            RST_restore_type(nd->t), 
            nd->c->t->body->body
        );

        if (nd->c->siblings.n) {
            fprintf(fd, " = ");
            RST_restore_code(fd, nd->c->siblings.n, u, indent);
        }
    }
    else if (TKN_isoperand(nd->t)) {
        if (nd->c) RST_restore_code(fd, nd->c, u, indent);
        fprintf(fd, " %s ", nd->t->body->body);
        if (nd->c->siblings.n) RST_restore_code(fd, nd->c->siblings.n, u, indent);
    }
    else if (
        TKN_isnumeric(nd->t)  || 
        TKN_isvariable(nd->t) ||
        nd->t->t_type == STRING_VALUE_TOKEN
    ) {
        fprintf(fd, "%s", nd->t->body->body);
    }

    switch (nd->t->t_type) {
        case START_TOKEN: {
            fprintf(fd, "%s (", START_COMMAND);
            ast_node_t* p = nd->c;
            for (; p && p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                RST_restore_code(fd, p, u, indent);
                if (p->siblings.n && p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) fprintf(fd, ", ");
            }

            fprintf(fd, ")\n");

            _print_indent(fd, indent);
            int r = RST_restore_code(fd, p, u, indent);
            if (r < 0) fprintf(fd, ";\n");

            complex = 1;
            break;
        }

        case FUNC_TOKEN: {
            fprintf(fd, "%s %s(", FUNCTION_COMMAND, nd->c->t->body->body);

            ast_node_t* p = nd->c->siblings.n ? nd->c->siblings.n->c : NULL;
            for (; p && p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                RST_restore_code(fd, p, u, indent);
                if (p->siblings.n && p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) fprintf(fd, ", ");
            }

            fprintf(fd, ") ");
            if (nd->c->c) fprintf(fd, "=> %s ", RST_restore_type(nd->c->c->t));
            fprintf(fd, "\n");

            _print_indent(fd, indent);
            int r = RST_restore_code(fd, p, u, indent);
            if (r < 0) fprintf(fd, ";\n");

            complex = 1;
            break;
        }

        case CALL_TOKEN: {
            fprintf(fd, "%s(", nd->t->body->body);
            for (ast_node_t* p = nd->c; p; p = p->siblings.n) {
                RST_restore_code(fd, p, u, indent);
                if (p->siblings.n) fprintf(fd, ", ");
            }

            fprintf(fd, ")");
            break;
        }

        case SYSCALL_TOKEN: {
            fprintf(fd, "%s(", SYSCALL_COMMAND);
            for (ast_node_t* p = nd->c; p; p = p->siblings.n) {
                RST_restore_code(fd, p, u, indent);
                if (p->siblings.n) fprintf(fd, ", ");
            }

            fprintf(fd, ")");
            break;
        }

        case REF_TYPE_TOKEN:  _simple_restore(fd, REF_COMMAND " ", nd->c, u, indent);      break;
        case DREF_TYPE_TOKEN: _simple_restore(fd, DREF_COMMAND " ", nd->c, u, indent);     break;
        case NEGATIVE_TOKEN:  _simple_restore(fd, NEGATIVE_COMMAND " ", nd->c, u, indent); break;
        case LOOP_TOKEN:      _simple_restore(fd, LOOP_COMMAND, nd->c, u, indent);         break;
        case EXIT_TOKEN:      _simple_restore(fd, EXIT_COMMAND " ", nd->c, u, indent);     break;
        case RETURN_TOKEN:    _simple_restore(fd, RETURN_COMMAND " ", nd->c, u, indent);   break;
        case CONVERT_TOKEN: {
            _simple_restore(fd, "", nd->c->siblings.n, u, indent); 
            fprintf(fd, "%s %s", CONVERT_COMMAND, RST_restore_type(nd->c->t));
            break;
        }

        case WHILE_TOKEN: {
            _simple_restore(fd, WHILE_COMAND " ", nd->c, u, indent);
            fprintf(fd, ";\n");
            _print_indent(fd, indent);
            RST_restore_code(fd, nd->c->siblings.n, u, indent);
            complex = 1;
            break;
        }

        case IF_TOKEN: {
            _simple_restore(fd, IF_COMMAND " ", nd->c, u, indent);
            fprintf(fd, ";\n");

            ast_node_t* tbranch = (nd->c ? nd->c->siblings.n : NULL);
            ast_node_t* fbranch = (tbranch ? tbranch->siblings.n : NULL);

            _print_indent(fd, indent);
            RST_restore_code(fd, tbranch, u, indent);
            if (fbranch) {
                _print_indent(fd, indent);
                RST_restore_code(fd, fbranch, u, indent);
            }

            complex = 1;
            break;
        }

        case SCOPE_TOKEN: {
            fprintf(fd, "{\n");
            for (ast_node_t* c = nd->c; c; c = c->siblings.n) {
                _print_indent(fd, indent + 1);
                if (RST_restore_code(fd, c, u, indent + 1) < 0) fprintf(fd, ";\n");
            }

            _print_indent(fd, indent);
            fprintf(fd, "}\n");

            complex = 1;
            break;
        }

        default: break;
    }

    if (u && set_has(u, nd)) fprintf(fd, "%s", UNDERSCORE_CLOSE);
    return complex;
}
