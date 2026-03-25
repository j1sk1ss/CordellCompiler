#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <std/list.h>
#include <std/queue.h>
#include <preproc/pp.h>
#include <prep/token.h>
#include <prep/markup.h>
#include <ast/ast.h>
#include <ast/astgen/annot.h>
#include <ast/astgen.h>
#include <ast/astgen/astgen.h>
#include <sem/misc/restore.h>
#include "../../../../misc/ast_helper.h"

static const char* _fmt_annot_type(annotation_type_t t) {
    switch (t) {
        case ALIGN_ANNOTATION:    return "@[align]";
        case SECTION_ANNOTATION:  return "@[section]";
        case NAKED_ANNOTATION:    return "@[naked]";
        case ADDRESS_ANNOTATION:  return "@[address]";
        case ENTRY_ANNOTATION:    return "@[entry]";
        case NOFALL_ANNOTATION:   return "@[no_fall]";
        case NOTLAZY_ANNOTATION:  return "@[not_lazy]";
        case STRAIGHT_ANNOTATION: return "@[straight]";
        case COUNTER_ANNOTATION:  return "@[counter]";
        case HOT_ANNOTATION:      return "@[hot]";
        case COLD_ANNOTATION:     return "@[cold]";
        case REGISTER_ANNOTATION: return "@[register]";
        case SIZEOF_ANNOTATION:   return "@[sizeof]";
        default: return "<unknown>";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments! Expected 3, got %i!\n", argc);
        return 1;
    }

    mm_init();

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s isn't found!\n", argv[1]);
        return 1;
    }

    finder_ctx_t finctx = { .bpath = argv[2] };
    fd = PP_perform(fd, &finctx);
    if (fd < 0) {
        fprintf(stderr, "Processed file %s isn't found!\n", argv[1]);
        return 1;
    }

    char pdata[2048] = { 0 };
    pread(fd, pdata, 2048, 0);

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens) || !list_size(&tokens)) {
        fprintf(stderr, "ERROR! tkn == NULL!\n");
        return 1;
    }

    MRKP_mnemonics(&tokens);
    MRKP_variables(&tokens);

    sym_table_t smt;
    SMT_init(&smt);

    ast_ctx_t sctx;
    AST_init_ctx(&sctx);

    if (!AST_parse_tokens(&tokens, &sctx, &smt)) {
        fprintf(stderr, "AST tree creation error!\n");
        return 1;
    }

    queue_t work;
    queue_init(&work);
    queue_push(&work, sctx.r);
    while (!queue_isempty(&work)) {
        ast_node_t* nd;
        if (!queue_pop(&work, (void**)&nd)) continue;
        if (list_size(&nd->annots)) {
            foreach (annotation_t* annot, &nd->annots) {
                printf("%s ", _fmt_annot_type(annot->t));
            }
            printf("base_token=%s\n", nd->t->body ? nd->t->body->body : "<unknown>");
        }

        for (ast_node_t* child = nd->c; child; child = child->siblings.n) {
            queue_push(&work, child);
        }
    }

    queue_free(&work);

    list_free_force_op(&tokens, (int (*)(void *))TKN_unload_token);
    AST_unload_ctx(&sctx);

    SMT_unload(&smt);
    close(fd);

    if (mm_get_allocated()) {
        printf("\n<<ERROR>>\tMemory leak!\t%i != 0!\n", mm_get_allocated());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
