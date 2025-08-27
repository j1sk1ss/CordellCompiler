#!/usr/bin/env bash
set -e

INCLUDES="-Iinclude"

cd "$(dirname "$0")/.."

declare -A TEST_SRCS

# ==== Base testing ====
TEST_SRCS[test_tok]="src/prep/token.c std/*.c"
TEST_SRCS[test_mrk]="src/prep/token.c src/prep/markup.c std/*.c"
TEST_SRCS[test_sem]="src/prep/*.c src/ast/*.c std/*.c"
TEST_SRCS[test_ast]="src/prep/*.c src/ast/*.c std/*.c"

# ==== Optimization testing ====
TEST_SRCS[test_varinline]="src/prep/*.c src/ast/*.c src/opt/varinline.c std/*.c"
TEST_SRCS[test_constopt]="src/prep/*.c src/ast/*.c src/opt/varinline.c src/opt/constopt.c std/*.c"
TEST_SRCS[test_strdecl]="src/prep/*.c src/ast/*.c src/opt/strdecl.c std/*.c"
TEST_SRCS[test_deadfunc]="src/prep/*.c src/ast/*.c src/opt/deadfunc.c std/*.c"
TEST_SRCS[test_condunroll]="src/prep/*.c src/ast/*.c src/opt/condunroll.c std/*.c"
TEST_SRCS[test_deadscope]="src/prep/*.c src/ast/*.c src/opt/deadscope.c std/*.c"
TEST_SRCS[test_offsetopt]="src/prep/*.c src/ast/*.c src/opt/offsetopt.c std/*.c"

# ==== Generation testing ====
TEST_SRCS[test_gen]="src/prep/*.c src/ast/*.c src/opt/*.c src/gen/*.c src/gen/*/*.c std/*.c"

# ======================================

if [[ $# -gt 0 ]]; then
    test_names=("$@")
else
    test_names=()
    for test_file in tests/test_*.c; do
        test_names+=("$(basename "$test_file" .c)")
    done
fi

for test_name in "${test_names[@]}"; do
    test_file="tests/$test_name.c"

    if [[ ! -f "$test_file" ]]; then
        echo "Test file not found: $test_file"
        exit 1
    fi

    echo "== Compilation: $test_file =="

    if [[ -z "${TEST_SRCS[$test_name]}" ]]; then
        echo "No source files for $test_name, append it to TEST_SRCS"
        exit 1
    fi

    gcc-14 $INCLUDES ${TEST_SRCS[$test_name]} "$test_file" \
        -DWARNING_LOGS -DERROR_LOGS -g -O2 -o "tests/$test_name"

    echo "== Running: $test_name =="
    ./tests/$test_name | tee "tests/$test_name.log"

    rm "tests/$test_name"
    rm -rf "tests/$test_name.dSYM"
    echo
done

echo "Tests completed!"
