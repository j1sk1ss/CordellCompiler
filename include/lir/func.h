#ifndef LIR_FUNC_H_
#define LIR_FUNC_H_

#include <lir/lirgen.h>

/* Replace direct tail calls with function jumps before instruction selection.
   P.S.: Will skip calls which use stack for arguments. Why? Because stack re-construction
         will consume the same or more compute time. 
   Params:
      - `cctx` - CFG context. 
      - `smt` - Symbol table.
      
   Returns 1 if succeeds. */
int LIR_FUNC_perform_tco(cfg_ctx_t* cctx, sym_table_t* smt);

#endif