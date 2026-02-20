# Integration tests
For a testing purpose, CordellCompiler has a special `tester.c` file with the usage of the entire compilation pipline. The main feature here, is that the pipeline is separated with `#ifdef` keywords. These sections can be either `included` or `excluded` from the compilation by special flags, that are placed in the `misc/paths.js`. Mentioned file is used in the `integrated_testing.py` script, where the `--module` keyword determines wich section is active in a run. List of possible modules:
- `preproc` - Pre-processor testing.
- `prep` - Preparation (Tokenization + Markup) testing.
- `ast` - AST build testing.
- `sem` - AST semantic testing. 
- `hir` - HIR build testing.
- `hir_ssa` - SSA form generator testing.
- `hir_dag` - DAG generator testing.
- `hir_constfold` - Constant fold analysis part testing.
- `lir` - LIR build testing.
- `lir_constfold` - Constant fold usage testing.
- `lir_selector` - Instruction selection testing.
- `lir_instplan` - Instraction planning / scheduling testing.
- `lir_regalloc` - Register allocation and memory allocation testing.
- `lir_peephole` - Peephole optimization testing.
- `asm` - ASM code generator testing.
- `asm_constfold` - ASM code constant forlding testing.

For simple testing, just invoke the tesing script with the next command:
```bash
python3 integrated_testing.py --run --module <name> --debugger <lldb/gdb> --test-code <path> --extra-flags <flag>
```

Flags are:
- `--run` - Build & Run the final executable.
- `--module` - Module name.
- `--debugger` - Enable the debugger (P.S. works only with the `--run` flag).
- `--test-code` - Specify the test code.
- `--extra-flags` - Additional compilation flags such as defines or optimizations.

The basic test of the asm code generation, without additional complex optimizations, can be invoked with the next querry:
```bash
python3 integrated_testing.py --run --module asm
```

According to the existence of a custom memory manager in this project, we have an ability to track meamleaks in a convenient way. For the memleak debbuging, just use the next sequence of commands:
```bash
python3 integrated_testing.py --run --module hir_dag --test-code dummy_data/test2.cpl --extra-flags DMEM_OPERATION_LOGS
python3 leaks.py --log-path <path_to_log>
```

# Module testing
Additionally, the Compiler has a unit tester. To use it, input the next querry:
```bash
python3 module_testing.py --path <path>
```
Here:
- `--path` - Path to the unit test folder.

## Unit test folder
Unit test folder is a folder that contains two special files:
- `base.c` - Implementation of the test script that returns the expected result.
- `dependencies.json` - Dependencies file with all used modules of the compiler.

For instance, check the `code_utesting/ast/` folder.

## How to create a new test?
When you add a new directory with all essential files (see the section above), you need to create a bunch of `.cpl` files. These files must include the `OUTPUT` section, like in the example below:
```cpl
{
    start() {
        i32 a = 0;
    }
}

: OUTPUT
:
``` 

In this `OUTPUT` section you should past the expected output from the compiler. For instance, here is the test of AST generation:
```cpl
{
    start(i64 argc, ptr u64 argv) {
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
         [argc] (I64_VARIABLE_TOKEN, v_id=0, s_id=1)
      [u64] (U64_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
         [argv] (U64_VARIABLE_TOKEN, ptr, v_id=1, s_id=1)
      { scope, id=2 }
:
```

# STD lib testing
To make sure, that the std library works properly, the project has the `std_testing.py` script. It works the same as the `module_testing.py`.
