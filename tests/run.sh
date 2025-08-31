#!/usr/bin/env bash
set -e

INCLUDES="-Iinclude"

cd "$(dirname "$0")/.."

declare -A TEST_SRCS
declare -A TEST_CODES

# ==== Base testing ====
TEST_SRCS[test_tok]="src/prep/token.c std/*.c"
TEST_CODES[test_tok]="tests/dummy_data/prep/token.txt"

TEST_SRCS[test_mrk]="src/prep/token.c src/prep/markup.c std/*.c"
TEST_CODES[test_mrk]="tests/dummy_data/prep/markup.cpl"

TEST_SRCS[test_sem]="src/prep/*.c src/ast/*.c src/ast/*/*.c std/*.c"
TEST_CODES[test_sem]="
    tests/dummy_data/sem/arrs.cpl
    tests/dummy_data/sem/vars.cpl
    tests/dummy_data/sem/func.cpl
    tests/dummy_data/sem/ro.cpl
"

TEST_SRCS[test_ast]="src/prep/*.c src/ast/*.c src/ast/*/*.c std/*.c"
TEST_CODES[test_ast]="
    tests/dummy_data/ast/vars.cpl
    tests/dummy_data/ast/arrs.cpl
    tests/dummy_data/ast/func.cpl
    tests/dummy_data/ast/cond.cpl
    tests/dummy_data/ast/oper.cpl
    tests/dummy_data/ast/stat.cpl
    tests/dummy_data/ast/error.cpl
"

# ==== Optimization testing ====
TEST_SRCS[test_varinline]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/varinline.c std/*.c"
TEST_CODES[test_varinline]="tests/dummy_data/opt/varinline.cpl"

TEST_SRCS[test_constopt]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/varinline.c src/opt/constopt.c std/*.c"
TEST_CODES[test_constopt]="tests/dummy_data/opt/constopt.cpl"

TEST_SRCS[test_strdecl]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/strdecl.c std/*.c"
TEST_CODES[test_strdecl]="tests/dummy_data/opt/strdecl.cpl"

TEST_SRCS[test_deadfunc]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/deadfunc.c std/*.c"
TEST_CODES[test_deadfunc]="tests/dummy_data/opt/deadfunc.cpl"

TEST_SRCS[test_condunroll]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/condunroll.c std/*.c"
TEST_CODES[test_condunroll]="tests/dummy_data/opt/condunroll.cpl"

TEST_SRCS[test_deadscope]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/deadscope.c std/*.c"
TEST_CODES[test_deadscope]="tests/dummy_data/opt/deadscope.cpl"

TEST_SRCS[test_offsetopt]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/offsetopt.c std/*.c"
TEST_CODES[test_offsetopt]="tests/dummy_data/opt/offsetopt.cpl"

# ==== Generation testing ====
TEST_SRCS[test_gen]="src/prep/*.c src/ast/*.c src/ast/*/*.c src/opt/*.c src/gen/*.c src/gen/*/*.c std/*.c"
TEST_CODES[test_gen]="
    tests/dummy_data/gen/vars.cpl
    tests/dummy_data/gen/arrs.cpl
    tests/dummy_data/gen/func.cpl
    tests/dummy_data/gen/cond.cpl
    tests/dummy_data/gen/oper.cpl
    tests/dummy_data/gen/stat.cpl
    tests/dummy_data/gen/print.cpl
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
    gcc $INCLUDES ${TEST_SRCS[$test_name]} "$test_file" \
        -DWARNING_LOGS -DERROR_LOGS -g -O0 -o "tests/$test_name"

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
