#!/usr/bin/env bash
set -e

INCLUDES="-Iinclude"

cd "$(dirname "$0")/.."

declare -A TEST_SRCS
declare -A TEST_CODES

# ==== Base testing ====
TEST_SRCS[test_tok]="src/prep/token.c std/*.c"
TEST_CODES[test_tok]="
    tests/dummy_data/prep/token/token_1.cpl
    tests/dummy_data/prep/token/token_2.cpl
    tests/dummy_data/prep/token/token_3.cpl
"

TEST_SRCS[test_mrk]="src/prep/token.c src/prep/markup.c std/*.c"
TEST_CODES[test_mrk]="
    tests/dummy_data/prep/markup/markup_1.cpl
    tests/dummy_data/prep/markup/markup_2.cpl
    tests/dummy_data/prep/markup/markup_3.cpl
    tests/dummy_data/prep/markup/markup_4.cpl
    tests/dummy_data/prep/markup/markup_5.cpl
    tests/dummy_data/prep/markup/markup_6.cpl
    tests/dummy_data/prep/markup/markup_7.cpl
    tests/dummy_data/prep/markup/markup_8.cpl
    tests/dummy_data/prep/markup/markup_9.cpl
"

TEST_SRCS[test_sem]="src/sem/*.c src/prep/*.c src/ast/*.c src/ast/*/*.c std/*.c"
TEST_CODES[test_sem]="
    tests/dummy_data/sem/sem_1.cpl
    tests/dummy_data/sem/sem_2.cpl
    tests/dummy_data/sem/sem_3.cpl
    tests/dummy_data/sem/sem_4.cpl
    tests/dummy_data/sem/sem_5.cpl
    tests/dummy_data/sem/sem_6.cpl
    tests/dummy_data/sem/sem_7.cpl
    tests/dummy_data/sem/sem_8.cpl
    tests/dummy_data/sem/sem_9.cpl
    tests/dummy_data/sem/sem_10.cpl
    tests/dummy_data/sem/sem_11.cpl
    tests/dummy_data/sem/sem_12.cpl
"

TEST_SRCS[test_ast]="src/prep/*.c src/symtab/*.c src/ast/*.c src/ast/*/*.c std/*.c"
TEST_CODES[test_ast]="
    tests/dummy_data/ast/gen/astgen_1.cpl
    tests/dummy_data/ast/gen/astgen_2.cpl
    tests/dummy_data/ast/gen/astgen_3.cpl
    tests/dummy_data/ast/gen/astgen_4.cpl
    tests/dummy_data/ast/gen/astgen_5.cpl
    tests/dummy_data/ast/gen/astgen_6.cpl
    tests/dummy_data/ast/gen/astgen_7.cpl
    tests/dummy_data/ast/gen/astgen_8.cpl
    tests/dummy_data/ast/gen/astgen_9.cpl
    tests/dummy_data/ast/gen/astgen_10.cpl
    tests/dummy_data/ast/gen/astgen_11.cpl
    tests/dummy_data/ast/gen/astgen_12.cpl
    tests/dummy_data/ast/gen/astgen_13.cpl
    tests/dummy_data/ast/gen/astgen_14.cpl
    tests/dummy_data/ast/gen/astgen_15.cpl
    tests/dummy_data/ast/gen/astgen_16.cpl
    tests/dummy_data/ast/gen/astgen_17.cpl
"

# ==== Optimization testing ====
TEST_SRCS[test_constopt]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/ast/opt/varinline.c src/ast/opt/constopt.c std/*.c"
TEST_CODES[test_constopt]="
    tests/dummy_data/ast/opt/constopt/constopt_1.cpl
    tests/dummy_data/ast/opt/constopt/constopt_2.cpl
    tests/dummy_data/ast/opt/constopt/constopt_3.cpl
"

TEST_SRCS[test_deadfunc]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/ast/opt/deadfunc.c std/*.c"
TEST_CODES[test_deadfunc]="
    tests/dummy_data/ast/opt/deadfunc/deadfunc_1.cpl
    tests/dummy_data/ast/opt/deadfunc/deadfunc_2.cpl
"

TEST_SRCS[test_condunroll]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/ast/opt/condunroll.c std/*.c"
TEST_CODES[test_condunroll]="
    tests/dummy_data/ast/opt/condunroll/condunroll_1.cpl
    tests/dummy_data/ast/opt/condunroll/condunroll_2.cpl
    tests/dummy_data/ast/opt/condunroll/condunroll_3.cpl
"

TEST_SRCS[test_deadscope]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/ast/opt/deadscope.c std/*.c"
TEST_CODES[test_deadscope]="
    tests/dummy_data/ast/opt/deadscope/deadscope_1.cpl
    tests/dummy_data/ast/opt/deadscope/deadscope_2.cpl
    tests/dummy_data/ast/opt/deadscope/deadscope_3.cpl
"

# ==== Generation testing ====
TEST_SRCS[test_hir]="src/prep/*.c src/symtab/*.c src/ast/*.c src/ast/parsers/*.c src/hir/*.c src/hir/*/*.c std/*.c"
TEST_CODES[test_hir]="
    tests/dummy_data/hir/gen/hirgen_1.cpl
    tests/dummy_data/hir/gen/hirgen_2.cpl
    tests/dummy_data/hir/gen/hirgen_3.cpl
    tests/dummy_data/hir/gen/hirgen_4.cpl
    tests/dummy_data/hir/gen/hirgen_5.cpl
"

TEST_SRCS[test_cfg]="src/prep/*.c src/symtab/*.c src/ast/*.c src/ast/parsers/*.c src/hir/*.c src/hir/*/*.c src/hir/opt/cfg/*.c std/*.c"
TEST_CODES[test_cfg]="
    tests/dummy_data/hir/opt/cfg/cfg_1.cpl
    tests/dummy_data/hir/opt/cfg/cfg_2.cpl
    tests/dummy_data/hir/opt/cfg/cfg_3.cpl
"

TEST_SRCS[test_ssa]="src/prep/*.c src/symtab/*.c src/ast/*.c src/ast/parsers/*.c src/hir/*.c src/hir/*/*.c src/hir/opt/cfg/*.c src/hir/opt/ssa/*.c std/*.c"
TEST_CODES[test_ssa]="
    tests/dummy_data/hir/opt/ssa/ssa_1.cpl
    tests/dummy_data/hir/opt/ssa/ssa_2.cpl
    tests/dummy_data/hir/opt/ssa/ssa_3.cpl
    tests/dummy_data/hir/opt/ssa/ssa_4.cpl
    tests/dummy_data/hir/opt/ssa/ssa_5.cpl
"

TEST_SRCS[test_dfg]="
    src/prep/*.c src/symtab/*.c 
    src/ast/*.c src/ast/parsers/*.c 
    src/hir/*.c src/hir/*/*.c src/hir/opt/cfg/*.c src/hir/opt/ssa/*.c src/hir/opt/dfg/*.c 
    std/*.c
"

TEST_CODES[test_dfg]="
    tests/dummy_data/hir/opt/dfg/dfg_1.cpl
    tests/dummy_data/hir/opt/dfg/dfg_2.cpl
    tests/dummy_data/hir/opt/dfg/dfg_3.cpl
    tests/dummy_data/hir/opt/dfg/dfg_4.cpl
    tests/dummy_data/hir/opt/dfg/dfg_5.cpl
"

TEST_SRCS[test_ra]="
    src/prep/*.c src/symtab/*.c 
    src/ast/*.c src/ast/parsers/*.c 
    src/hir/*.c src/hir/*/*.c src/hir/opt/cfg/*.c src/hir/opt/ssa/*.c src/hir/opt/dfg/*.c src/hir/opt/ra/*.c 
    std/*.c
"

TEST_CODES[test_ra]="
    tests/dummy_data/hir/opt/ra/ra_1.cpl
    tests/dummy_data/hir/opt/ra/ra_2.cpl
    tests/dummy_data/hir/opt/ra/ra_3.cpl
    tests/dummy_data/hir/opt/ra/ra_4.cpl
"

TEST_SRCS[test_lir]="
    src/prep/*.c src/symtab/*.c 
    src/ast/*.c src/ast/parsers/*.c 
    src/hir/*.c src/hir/*/*.c src/hir/opt/cfg/*.c src/hir/opt/ssa/*.c src/hir/opt/dfg/*.c src/hir/opt/ra/*.c
    src/lir/*.c src/lir/*/*.c
    std/*.c
"

TEST_CODES[test_lir]="
    tests/dummy_data/lir/gen/lirgen_1.cpl
    tests/dummy_data/lir/gen/lirgen_2.cpl
"

TEST_SRCS[test_asm]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/ast/opt/*.c src/asm/*.c src/asm/*/*.c std/*.c"
TEST_CODES[test_asm]="
    tests/dummy_data/asm/asm_1.cpl
    tests/dummy_data/asm/asm_2.cpl
    tests/dummy_data/asm/asm_3.cpl
    tests/dummy_data/asm/asm_4.cpl
    tests/dummy_data/asm/asm_5.cpl
    tests/dummy_data/asm/asm_6.cpl
    tests/dummy_data/asm/asm_7.cpl
    tests/dummy_data/asm/asm_8.cpl
    tests/dummy_data/asm/asm_9.cpl
    tests/dummy_data/asm/asm_10.cpl
    tests/dummy_data/asm/asm_11.cpl
    tests/dummy_data/asm/asm_12.cpl
    tests/dummy_data/asm/asm_13.cpl
    tests/dummy_data/asm/asm_14.cpl
    tests/dummy_data/asm/asm_15.cpl
    tests/dummy_data/asm/asm_16.cpl
    tests/dummy_data/asm/asm_17.cpl
    tests/dummy_data/asm/asm_18.cpl
    tests/dummy_data/asm/asm_19.cpl
"

TEST_SRCS[test_build]="src/builder.c src/prep/*.c src/sem/*.c src/ast/*.c src/ast/*/*.c src/lir/*.c src/lir/*/*.c src/asm/*.c src/asm/*/*.c std/*.c"
TEST_CODES[test_build]="
    tests/dummy_data/builder/builder_1.cpl
    tests/dummy_data/builder/builder_2.cpl
    tests/dummy_data/builder/builder_3.cpl
    tests/dummy_data/builder/builder_4.cpl
    tests/dummy_data/builder/builder_5.cpl
    tests/dummy_data/builder/builder_6.cpl
    tests/dummy_data/builder/builder_7.cpl
    tests/dummy_data/builder/builder_8.cpl
    tests/dummy_data/builder/builder_9.cpl
    tests/dummy_data/builder/builder_10.cpl
    tests/dummy_data/builder/builder_11.cpl
    tests/dummy_data/builder/builder_12.cpl
"

# ======================================

DEBUGGER=""
if [[ "$1" == "lldebug" ]]; then
    DEBUGGER="lldb"
    shift
elif [[ "$1" == "gdbdebug" ]]; then
    DEBUGGER="gdb"
    shift
fi

if [[ "$1" == "lstests" ]]; then
    echo "Available tests:"
    for t in "${!TEST_SRCS[@]}"; do
        echo "  $t -> ${TEST_CODES[$t]}"
    done
    exit 0
fi

test_names=()
CODE_IDXES=()
while [[ $# -gt 0 ]]; do
    test_names+=("$1")
    shift
    if [[ $# -gt 0 && "$1" =~ ^[0-9]+$ ]]; then
        CODE_IDXES+=("$1")
        shift
    else
        CODE_IDXES+=(0)
    fi
done

for i in "${!test_names[@]}"; do
    test_name="${test_names[$i]}"
    CODE_IDX="${CODE_IDXES[$i]}"
    test_file="tests/$test_name.c"

    if [[ ! -f "$test_file" ]]; then
        echo "Test file not found: $test_file"
        exit 1
    fi

    if [[ -z "${TEST_SRCS[$test_name]}" ]]; then
        echo "No source files for $test_name, append it to TEST_SRCS"
        exit 1
    fi

    codes=(${TEST_CODES[$test_name]})
    if [[ ${#codes[@]} -eq 0 ]]; then
        echo "No code files for $test_name, append it to TEST_CODES"
        exit 1
    fi

    if [[ "$CODE_IDX" -ge ${#codes[@]} ]]; then
        echo "Invalid code index $CODE_IDX for test $test_name"
        exit 1
    fi

    code_file="${codes[$CODE_IDX]}"

    echo "== Compilation: $test_file =="
    gcc-14 $INCLUDES ${TEST_SRCS[$test_name]} "$test_file" \
        -DWARNING_LOGS -DERROR_LOGS -DLOGGING_LOGS -DINFO_LOGS -DDEBUG_LOGS -g -O0 -o "tests/$test_name"

    if [[ -n "$DEBUGGER" ]]; then
        echo "== Debugging with $DEBUGGER: $test_name ($code_file) =="
        if [[ "$DEBUGGER" == "lldb" ]]; then
            lldb "tests/$test_name" -- "$code_file"
        else
            gdb --args "tests/$test_name" "$code_file"
        fi
        exit 0
    else
        echo "== Running: $test_name with $code_file =="
        ./tests/$test_name "$code_file" | tee "tests/$test_name.log"
        rm "tests/$test_name"
        rm -rf "tests/$test_name.dSYM"
        echo
    fi
done

echo "Tests completed!"
