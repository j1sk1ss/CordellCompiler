# Testing
For a testing purpose, CordellCompiler has a special `tester.c` file with the usage of the entire compilation pipline. The main feature here, is that the pipeline is separated with `#ifdef` keywords. These sections can be either `included` or `excluded` from the compilation by special flags, that are placed in the `misc/paths.js`. Mentioned file is used in the `tester.py` script, where the `--module` keyword determines wich section is active in a run. List of possible modules:
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

For simple testing, just invoke the next command:
```bash
python3 tester.py --run --module <name> --debugger <lldb/gdb> --test-code <path> --extra-flags <flag>
```

The basic test of an asm code generation, without additional complex optimizations, can be invoked with the next querry:
```bash
python3 tester.py --run --module asm
```

According to existence of a custom memory mannager in this project, we have ability to track meamleaks in a more convenient way. For the memleak debbuging, just use the next sequence of commands:
```bash
python3 tester.py --run --module hir_dag --test-code dummy_data/test2.cpl --extra-flags DMEM_OPERATION_LOGS
python3 leaks.py --log-path <path_to_log>
```
