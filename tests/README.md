# Testing

Testing:
```bash
python3 tester.py --run --module <name> --debugger <lldb/gdb> --test-code <path> --extra-flags <flag>
```

Example:
```bash
python3 tester.py --run --module asm
```

Memleak check:
```bash
python3 tester.py --run --module hir_dag --test-code dummy_data/test2.cpl --extra-flags DMEM_OPERATION_LOGS
python3 leaks.py --log-path <path_to_log>
```
