start() {
    switch 1; {
        case 1; {}
        case 2; {}
        case 3; {}
    }

    @[straight]
    switch 1; {
        case 1; {}
        case 2; {}
    }

    switch 1; {
        case 1; {}
        case 2; {}
        case 3; {}
        case 4; {}
        case 5; {}
        case 6; {}
    }

    @[straight]
    switch 1; {
        case 1; {}
        case 2; {}
        case 3; {}
        case 4; {}
        case 5; {}
        case 6; {}
    }
}

:/ OUTPUT
[WARNING] [{X}24:12] Switch statement here has '6' cases and uses the straight search. Consider to remove @[straight].
24 | switch 1;
   | ^^^^^^^^^
24 | {
   | ^
25 |     case 1;
   | ^^^^^^^^^^^
25 |     {
   | ^^^^^
25 |     }
   | ^^^^^
26 |     case 2;
   | ^^^^^^^^^^^
26 |     {
   | ^^^^^
26 |     }
   | ^^^^^
27 |     case 3;
   | ^^^^^^^^^^^
27 |     {
   | ^^^^^
27 |     }
   | ^^^^^
28 |     case 4;
   | ^^^^^^^^^^^
28 |     {
   | ^^^^^
28 |     }
   | ^^^^^
29 |     case 5;
   | ^^^^^^^^^^^
29 |     {
   | ^^^^^
29 |     }
   | ^^^^^
30 |     case 6;
   | ^^^^^^^^^^^
30 |     {
   | ^^^^^
30 |     }
   | ^^^^^
24 | }
   | ^
   | ^
[WARNING] [{X}2:12] Switch statement here has '3' cases and uses the binary search. Consider to add @[straight].
 2 | switch 1;
   | ^^^^^^^^^
 2 | {
   | ^
 3 |     case 1;
   | ^^^^^^^^^^^
 3 |     {
   | ^^^^^
 3 |     }
   | ^^^^^
 4 |     case 2;
   | ^^^^^^^^^^^
 4 |     {
   | ^^^^^
 4 |     }
   | ^^^^^
 5 |     case 3;
   | ^^^^^^^^^^^
 5 |     {
   | ^^^^^
 5 |     }
   | ^^^^^
 2 | }
   | ^
   | ^
[WARNING] [{X}1:7] Start doesn't have the 'exit' keyword on all paths!
 1 | start ()
 2 | {
 2 |     switch 1;
 2 |     {
 3 |         case 1;
 3 |         {
 3 |         }
 4 |         case 2;
 4 |         {
 4 |         }
 5 |         case 3;
 5 |         {
 5 |         }
 2 |     }
 9 |     switch 1;
 9 |     {
10 |         case 1;
10 |         {
10 |         }
11 |         case 2;
11 |         {
11 |         }
 9 |     }
14 |     switch 1;
14 |     {
15 |         case 1;
15 |         {
15 |         }
16 |         case 2;
16 |         {
16 |         }
17 |         case 3;
17 |         {
17 |         }
18 |         case 4;
18 |         {
18 |         }
19 |         case 5;
19 |         {
19 |         }
20 |         case 6;
20 |         {
20 |         }
14 |     }
24 |     switch 1;
24 |     {
25 |         case 1;
25 |         {
25 |         }
26 |         case 2;
26 |         {
26 |         }
27 |         case 3;
27 |         {
27 |         }
28 |         case 4;
28 |         {
28 |         }
29 |         case 5;
29 |         {
29 |         }
30 |         case 6;
30 |         {
30 |         }
24 |     }
 2 | }
/: