{
    function foo() -> i0 {
        i32 a = 0;
        a += 1;
        if 1; {
            return;
        }
        return;
        return;
    }

    start(i64 argc, ptr u64 argv) {
        i32 a = 0;
        exit 0;
        exit a;
    }
}

: OUTPUT
[WARNING] [8:16] 'Dead Code' after the termination statement!
3 | {
3 |     i32 a = 0;
4 |     a += 1;
5 |     if 1;
6 |     {
6 |         return ;
6 |     }
8 |     return ;
9 |     return ;
  |     ^^^^^^^
3 | }
[WARNING] [14:14] 'Dead Code' after the termination statement!
13 | {
13 |     i32 a = 0;
14 |     exit 0;
15 |     exit a;
   |     ^^^^^^
13 | }
: