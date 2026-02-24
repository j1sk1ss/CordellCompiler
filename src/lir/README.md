# LIR
- `constfold/` - All essential code for the applying information from HIR on LIR code.
- `dfg/` - Code for data flow analysis (e.g. Liveness Analyzis (DEF-USE-IN-OUT), Deallocation).
- `instplan/` - Arch-dependent instruction replacement.
- `lirgen/` - Convertors from HIR to LIR level.
- `peephole/` - Peephole optimizator and DSL language for optimizatior generation.
- `regalloc/` - Register allocation code.
- `selector/` - All code for the final part of LIR level. These files inserts and selects arch-dependent instructions.
- `lir_types.c` - Misc functions for LIR types.
- `lir.c` - Main logic of this layer.
- `lirgen.c` - Entry point in this layer.

The LIR part is the final level before raw assembly language. There are two easy to distinguish sub-levels. The `HLIR` (High-Low Intermidiate Representation) and the `LLIR` (Low-Low Intermidiate Representation).
- `HLIR` - Still close to the `LHIR` form, but with LIR dependent types and operations. 
- `LLIR` - Planned, register allocated arch-dependent form that is close to assembly language.
