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

TEST_SRCS[test_ast]="src/prep/*.c src/ast/*.c src/ast/*/*.c std/*.c"
TEST_CODES[test_ast]="
    tests/dummy_data/ast/ast_1.cpl
    tests/dummy_data/ast/ast_2.cpl
    tests/dummy_data/ast/ast_3.cpl
    tests/dummy_data/ast/ast_4.cpl
    tests/dummy_data/ast/ast_5.cpl
    tests/dummy_data/ast/ast_6.cpl
    tests/dummy_data/ast/ast_7.cpl
    tests/dummy_data/ast/ast_8.cpl
    tests/dummy_data/ast/ast_9.cpl
    tests/dummy_data/ast/ast_10.cpl
    tests/dummy_data/ast/ast_11.cpl
    tests/dummy_data/ast/ast_12.cpl
    tests/dummy_data/ast/ast_13.cpl
    tests/dummy_data/ast/ast_14.cpl
    tests/dummy_data/ast/ast_15.cpl
    tests/dummy_data/ast/ast_16.cpl
    tests/dummy_data/ast/ast_17.cpl
"

# ==== Optimization testing ====
TEST_SRCS[test_varinline]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/varinline.c std/*.c"
TEST_CODES[test_varinline]="
    tests/dummy_data/opt/varinline/varinline_1.cpl
    tests/dummy_data/opt/varinline/varinline_2.cpl
    tests/dummy_data/opt/varinline/varinline_3.cpl
    tests/dummy_data/opt/varinline/varinline_4.cpl
    tests/dummy_data/opt/varinline/varinline_5.cpl
    tests/dummy_data/opt/varinline/varinline_6.cpl
    tests/dummy_data/opt/varinline/varinline_7.cpl
    tests/dummy_data/opt/varinline/varinline_8.cpl
    tests/dummy_data/opt/varinline/varinline_9.cpl
    tests/dummy_data/opt/varinline/varinline_10.cpl
"

TEST_SRCS[test_constopt]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/varinline.c src/opt/constopt.c std/*.c"
TEST_CODES[test_constopt]="
    tests/dummy_data/opt/constopt/constopt_1.cpl
    tests/dummy_data/opt/constopt/constopt_2.cpl
    tests/dummy_data/opt/constopt/constopt_3.cpl
"

TEST_SRCS[test_strdecl]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/strdecl.c std/*.c"
TEST_CODES[test_strdecl]="
    tests/dummy_data/opt/strdecl/strdecl_1.cpl
    tests/dummy_data/opt/strdecl/strdecl_2.cpl
"

TEST_SRCS[test_deadfunc]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/deadfunc.c std/*.c"
TEST_CODES[test_deadfunc]="
    tests/dummy_data/opt/deadfunc/deadfunc_1.cpl
    tests/dummy_data/opt/deadfunc/deadfunc_2.cpl
"

TEST_SRCS[test_condunroll]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/condunroll.c std/*.c"
TEST_CODES[test_condunroll]="
    tests/dummy_data/opt/condunroll/condunroll_1.cpl
    tests/dummy_data/opt/condunroll/condunroll_2.cpl
    tests/dummy_data/opt/condunroll/condunroll_3.cpl
"

TEST_SRCS[test_deadscope]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/deadscope.c std/*.c"
TEST_CODES[test_deadscope]="
    tests/dummy_data/opt/deadscope/deadscope_1.cpl
    tests/dummy_data/opt/deadscope/deadscope_2.cpl
    tests/dummy_data/opt/deadscope/deadscope_3.cpl
"

TEST_SRCS[test_offsetopt]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/offsetopt.c std/*.c"
TEST_CODES[test_offsetopt]="
    tests/dummy_data/opt/offsetopt/offsetopt_1.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_2.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_3.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_4.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_5.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_6.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_7.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_8.cpl
    tests/dummy_data/opt/offsetopt/offsetopt_9.cpl
"

TEST_SRCS[test_deadopt]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/deadopt.c std/*.c"
TEST_CODES[test_deadopt]="
    tests/dummy_data/opt/deadopt/deadopt_1.cpl
    tests/dummy_data/opt/deadopt/deadopt_2.cpl
    tests/dummy_data/opt/deadopt/deadopt_3.cpl
    tests/dummy_data/opt/deadopt/deadopt_4.cpl
    tests/dummy_data/opt/deadopt/deadopt_5.cpl
"

# ==== Generation testing ====
TEST_SRCS[test_ir]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/ir/*.c src/ir/*/*.c std/*.c"
TEST_CODES[test_ir]="
    tests/dummy_data/ir/ir_1.cpl
"

TEST_SRCS[test_gen]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/*.c src/gen/*.c src/gen/*/*.c std/*.c"
TEST_CODES[test_gen]="
    tests/dummy_data/gen/gen_1.cpl
    tests/dummy_data/gen/gen_2.cpl
    tests/dummy_data/gen/gen_3.cpl
    tests/dummy_data/gen/gen_4.cpl
    tests/dummy_data/gen/gen_5.cpl
    tests/dummy_data/gen/gen_6.cpl
    tests/dummy_data/gen/gen_7.cpl
    tests/dummy_data/gen/gen_8.cpl
    tests/dummy_data/gen/gen_9.cpl
    tests/dummy_data/gen/gen_10.cpl
    tests/dummy_data/gen/gen_11.cpl
    tests/dummy_data/gen/gen_12.cpl
    tests/dummy_data/gen/gen_13.cpl
    tests/dummy_data/gen/gen_14.cpl
    tests/dummy_data/gen/gen_15.cpl
    tests/dummy_data/gen/gen_16.cpl
    tests/dummy_data/gen/gen_17.cpl
    tests/dummy_data/gen/gen_18.cpl
    tests/dummy_data/gen/gen_19.cpl
"

TEST_SRCS[test_build]="src/builder.c src/prep/*.c src/sem/*.c src/ast/*.c src/ast/*/*.c src/opt/*.c src/gen/*.c src/gen/*/*.c std/*.c"
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
