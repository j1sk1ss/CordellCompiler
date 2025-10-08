# HIR
- `cfg/` - All code for Control-Flow-Graph generation and dominant, strict dominance and dominance frontier calculation.
- `dfg/` - All code for Data-Flow analysis (Liveness analyzer, Allias marker).
- `ra/` - Register allocation code
- `ssa/` - All code for SSA-form generation.
- `opt/` - HIR optimizations.
- `hir_types.c` - Misc functions for HIR types.
- `hir.c` - Main logic of this layer.
- `hirgen.c` - Entry point in this layer.