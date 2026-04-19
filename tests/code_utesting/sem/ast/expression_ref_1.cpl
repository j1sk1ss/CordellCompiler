function foo() -> i32;
start() {
    ptr i32 a = ref 10;
    ptr i32 b = ref foo();
    ptr i32 c = ref (10 + 10);
    exit 1;
}

:/ OUTPUT
[WARNING] [3:25] The reference of a temporary variable! May lead to UB, consider refactoring!
3 | ptr i32 a = ref 10;
  |                 ^^
[WARNING] [4:25] The reference of a temporary variable! May lead to UB, consider refactoring!
4 | ptr i32 b = ref foo();
  |                 ^^^^^
[WARNING] [5:25] The reference of a temporary variable! May lead to UB, consider refactoring!
5 | ptr i32 c = ref 10 + 10;
  |                 ^^^^^^^
/: