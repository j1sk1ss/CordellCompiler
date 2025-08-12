cd ..
gcc-14 tests/test_tokenizer.c -Iinclude src/*.c src/syntax/*.c src/optimization/*.c std/*.c -o tests/test_tokenizer
cd tests && ./test_tokenizer && rm test_tokenizer