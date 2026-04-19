function foo() -> i32;
start() {
    foo();
    exit 1;
}

:/ OUTPUT
[WARNING] [3:9] Unused the function 'foo' result! If its result isn't used elsewhere, consider to change its return type to the 'i0' type.
3 | {
3 |     foo();
  |     ^^^^^
4 |     exit 1;
3 | }
/: