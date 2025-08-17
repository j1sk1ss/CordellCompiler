#!/bin/bash
set -e

INCLUDES="-Iinclude"
SRC="src/*.c src/syntax/*.c src/optimization/*.c std/*.c"

cd "$(dirname "$0")/.."

for test_file in tests/test_*.c; do
    test_name=$(basename "$test_file" .c)
    echo "== Compilation: $test_file =="
    gcc-14 $INCLUDES $SRC "$test_file" -DWARNING_LOGS -DERROR_LOGS -g -O2 -o "tests/$test_name"
    
    echo "== Running: $test_name =="
    ./tests/$test_name | tee "tests/$test_name.log"

    rm "tests/$test_name"
    rm -rf "tests/$test_name.dSYM"
    echo
done

echo "Tests completed!"
