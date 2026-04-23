#ifndef BUILDER_H_
#define BUILDER_H_

/* Base libs for STDIO with CLI and files */
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/* Pre-processor part and tokenization part */
#include <preproc/pp.h>
#include <prep/token.h>
#include <prep/markup.h>

/* Semantic (Static analyzer) setup */
#include <sem/semantic.h>

/* AST generation part and AST optimization part */
#include <ast/ast.h>
#include <ast/astgen.h>
#include <ast/astgen/astgen.h>
#include <ast/opt/condunroll.h>
#include <ast/opt/deadscope.h>

/* HIR generation part and CFG generation part */
#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/cfg.h>

/* SSA + constant fold / propagation + TRE + inline + LICM */
#include <hir/ssa.h>
#include <hir/dag.h>
#include <hir/constfold.h>
#include <hir/func.h>
#include <hir/loop.h>

/* HLIR generation */
#include <lir/lirgen.h>
#include <lir/lirgens/lirgens.h>

/* HLIR constfold part */
#include <lir/constfold.h>

/* From HLIR to LLIR (now we're arch dependent) */
#include <lir/selector/instsel.h>
#include <lir/selector/memsel.h>
#include <lir/selector/savereg.h>
#include <lir/selector/x84_64_gnu_nasm.h>
#include <lir/selector/x84_64_macho_nasm.h>

/* Instruction scheduling */
#include <lir/instplan/targinfo.h>
#include <lir/instplan/instplan.h>

/* Liveness analysis + Register allocation */
#include <lir/dfg.h>
#include <lir/regalloc/ra.h>
#include <lir/regalloc/regalloc.h>
#include <lir/regalloc/x84_64_gnu_nasm.h>

/* Peephole optimization */
#include <lir/peephole/peephole.h>
#include <lir/peephole/x84_64_gnu_nasm.h>

/* Codegen */
#include <asm/asmgen.h>
#include <asm/x86_64_gnu_nasm_asmgen.h>
#include <asm/x86_64_macho_nasm_asmgen.h>

#endif
