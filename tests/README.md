# Testing
For testing purpose CordellCompiler has an special `teste.c` file with usage of entire compilation pipline. Main feature here, that this pipeline separated with `#ifdef` keywords. This sections can be included in and excluded from compilation with flags, that are written in `misc/paths.js`. Mentioned file is used in `tester.py` script, where `--module` keyword determine wich section is active at this run. List of possible modules:
- `prep`
- `ast`
- `sem`
- `hir`
- `hir_ssa`
- `hir_dag`
- `hir_constfold`
- `lir`
- `lir_constfold`
- `lir_selector`
- `lir_instplan`
- `lir_regalloc`
- `lir_peephole`
- `asm`
- `asm_constfold`

For simple testing, just invoke next command:
```bash
python3 tester.py --run --module <name> --debugger <lldb/gdb> --test-code <path> --extra-flags <flag>
```

The basic test of asm code generation, without additional complex optimizations, can be invoked with next querry:
```bash
python3 tester.py --run --module asm
```

According to existence of custom memmory mannager in this project, we have ability to track meamleaks in more simple and convinient way. For the memleak debbuging, just use the next sequence of commands:
```bash
python3 tester.py --run --module hir_dag --test-code dummy_data/test2.cpl --extra-flags DMEM_OPERATION_LOGS
python3 leaks.py --log-path <path_to_log>
```
