#!/usr/bin/env bash
set -e

INCLUDES="-Iinclude"

cd "$(dirname "$0")/.."

declare -A TEST_SRCS
TEST_SRCS[test_tok]="src/prep/token.c std/*.c"
TEST_SRCS[test_mrk]="src/prep/token.c src/prep/markup.c std/*.c"
TEST_SRCS[test_sem]="src/prep/*.c std/*.c"
TEST_SRCS[test_ast]="src/prep/*.c src/ast/*.c std/*.c"
TEST_SRCS[test_opt_assign]="src/prep/*.c src/ast/*.c src/opt/varinline.c std/*.c"
TEST_SRCS[test_gen]="src/prep/*.c src/ast/*.c src/opt/*.c src/generator.c std/*.c"

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
