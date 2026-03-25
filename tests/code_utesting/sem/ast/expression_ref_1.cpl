{
    function foo() -> i32;
    start() {
        ptr i32 a = ref 10;
        ptr i32 b = ref foo();
        ptr i32 c = ref (10 + 10);
        exit 1;
    }
}

: OUTPUT
[WARNING] [4:29] The reference of a temporary variable!
4 | ptr i32 a = ref 10;
  |                 ^^
[WARNING] [5:29] The reference of a temporary variable!
5 | ptr i32 b = ref foo();
  |                 ^^^^^
[WARNING] [6:29] The reference of a temporary variable!
6 | ptr i32 c = ref 10 + 10;
  |                 ^^^^^^^
: