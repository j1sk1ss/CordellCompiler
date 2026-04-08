#ifndef HIR_VISITORS_H_
#define HIR_VISITORS_H_

#include <std/mem.h>
#include <std/queue.h>
#include <std/tuple.h>
#include <sem/misc/trace.h>
#include <sem/misc/warns.h>
#include <sem/hir/hir_data.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <hir/hir.h>
#include <hir/hirgen.h>
#include <hir/hir_types.h>
#include <hir/cfg.h>
#include <hir/dag.h>

int HIRWLKR_visit_setpos_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_phi_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_gdref_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_ldref_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_ifop2_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_wrong_arg_type(HIR_VISITOR_ARGS);
int HIRWLKR_wrong_ret_type(HIR_VISITOR_ARGS);

#endif
