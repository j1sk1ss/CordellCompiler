{
    start(i64 argc, ptr u64 argv) {
        if 1; {
            i32 a = 1;
            a -= 100;
            if 1; {
                exit 1;
            }
            exit 0;
        }
        else {
            i32 a = 1;
            a -= 100;
            if 1; {
                exit 1;
            }
            exit 0;
        }
        exit 0;
    }
}

: OUTPUT
[WARNING] Possible branch redundancy! The branch at [{X}] is similar to the branch at [{X}]!
 3 | if 1;
 4 | {
   | ^
 4 |     i32 a = 1;
   | ^^^^^^^^^^^^^^
 5 |     a -= 100;
   | ^^^^^^^^^^^^^
 6 |     if 1;
   | ^^^^^^^^^
 7 |     {
   | ^^^^^
 7 |         exit 1;
   | ^^^^^^^^^^^^^^^
 7 |     }
   | ^^^^^
 9 |     exit 0;
   | ^^^^^^^^^^^
 4 | }
   | ^
12 | else {
   | ^    ^
12 |     i32 a = 1;
   | ^^^^^^^^^^^^^^
13 |     a -= 100;
   | ^^^^^^^^^^^^^
14 |     if 1;
   | ^^^^^^^^^
15 |     {
   | ^^^^^
15 |         exit 1;
   | ^^^^^^^^^^^^^^^
15 |     }
   | ^^^^^
17 |     exit 0;
   | ^^^^^^^^^^^
12 | }
   | ^
   | ^
: