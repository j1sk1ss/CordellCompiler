@[align(7)] glob i32 a = 1;
start() {
    @[align(1)] i32 b = 1;
    exit 1;
}

:/ OUTPUT
[WARNING] b The '[3:28]' variable has align that isn't even! '1' % 2 != 0.
3 | i32 b = 1;
  | ^^^^^^^^^
[WARNING] a The '[1:30]' variable has align that isn't even! '7' % 2 != 0.
1 | glob i32 a = 1;
  | ^^^^^^^^^^^^^^
/: