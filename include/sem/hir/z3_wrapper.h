#ifndef SEM_HIR_Z3_WRAPPER_H_
#define SEM_HIR_Z3_WRAPPER_H_

#include <hir/cfg.h>
#include <symtab/symtab_id.h>
#include <symtab/symtab.h>

typedef struct z3_analyzer z3_analyzer_t;

z3_analyzer_t* Z3A_create(cfg_ctx_t* cfg, sym_table_t* smt);
int Z3A_unload(z3_analyzer_t* analyzer);
int Z3_can_vid_be_equal_ctx(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value);
int Z3_can_reach_label_ctx(z3_analyzer_t* analyzer, cfg_func_t* function, long l_id);

#endif
