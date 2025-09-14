# Semantic check
- [bitness](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/sem/bitness.c)
- [rettype](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/sem/rettype.c)
- [ro](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/sem/ro.c)
- [semantic](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/sem/semantic.c)
- [size](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/sem/size.c)
- [ownership](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/sem/ownership.c)

# Examples

- size

Source code:
```CPL
0  {
1      start() {
2          arr fla[5, i8]  = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
3          arr sla[5, i8]  = { 1, 2, 3, 4, 256 };
4          arr tla[5, i16] = { 1, 2, 3, 4, 256 };
5          
6          
7          arr unkla[10, i16] =;
8          exit 0;
9      }
10 }
```

Produced errors:
```
[WARN] (src/sem/size.c:59) Array [fla] larger than expected size 10 > 5!
[WARN] (src/sem/size.c:49) Value 256 at line=3 too large for array i8 (256 >= 127)!
```

------------------------------

- rettype

Source code:
```CPL
1  {
2      function foo(i8 a) {
3          return 1;
4      }
5  
6      function bar(i64 d) => i32 {
7          return d;
8      }
9  
10     function bar1(i64 d) => ptr i64 {
11         return d;
12     }
13 
14     function bar2() => u8 {
15         return 250;
16     }
17 
18     function bar3() => u8 {
19         return 350;
20     }
21 
22     start() {
23         foo('A');
24         bar(100);
25         exit 0;
26     }
27 }
```

Produced errors:
```
[WARN] (src/sem/rettype.c:37) Unmatched return type in line=6. Should return bitness=32, but provide bitness=64
[WARN] (src/sem/rettype.c:47) Unmatched pointer status at line=10. Should be ptr, but stack!
[WARN] (src/sem/rettype.c:37) Unmatched return type in line=18. Should return bitness=8, but provide bitness=16
```

------------------------------

- ro

Source code:
```CPL
1  {
2      start() {
3          ro i32 a = 10;
4          i32 b = a;
5          a = a + 1;
6          exit 0;
7      }
8  }
```

Produced errors:
```
[ERROR] (src/sem/ro.c:12) Read only variable [a] assigned here! line=4
```

------------------------------

- bitness / size

Source code:
```CPL
0  {
1      start(i64 argc, ptr u64 argv) {
2          i8 a  = 126;
3          i8 a1 = 128;
4          u8 a2 = 128;
5          u8 a3 = 256;
6  
7          arr a4[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
8          arr a5[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 128 };
9          arr a6[10, u8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 128 };
10         arr a7[5, u8]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
11         exit 0;
12     }
13 }
```

Produced errors:
```
[WARN] (src/sem/size.c:25) Value 128 at line=3 too large for type i8 (128 >= 127)!
[WARN] (src/sem/size.c:25) Value 256 at line=5 too large for type u8 (256 >= 255)!
[WARN] (src/sem/size.c:49) Value 128 at line=8 too large for array i8 (128 >= 127)!
[WARN] (src/sem/size.c:59) Array [a7] larger than expected size 10 > 5!
[WARN] (src/sem/bitness.c:11) Danger shadow type cast at line 5. Different size [8] (a3) and [16] (256). Did you expect this?
```

------------------------------

- bitness / size

Source code:
```CPL
0  {
1      start() {
2          i8 a  = 129;
3          u8 a1 = 129;
4          u8 a2 = 256;
5          i16 b = 100000;
6          i32 c = 100000;
7          i64 d = 100000;
8          exit 0;
9      }
10 }
```

Produced errors:
```
[WARN] (src/sem/size.c:32) Value 129 at line=2 too large for type i8 (129 >= 127)!
[WARN] (src/sem/size.c:32) Value 256 at line=4 too large for type u8 (256 >= 255)!
[WARN] (src/sem/size.c:32) Value 100000 at line=5 too large for type i16 (100000 >= 32767)!
[WARN] (src/sem/bitness.c:11) Danger shadow type cast at line 4. Different size [8] (a2) and [16] (256). Did you expect this?
[WARN] (src/sem/bitness.c:11) Danger shadow type cast at line 5. Different size [16] (b) and [32] (100000). Did you expect this?
```

------------------------------

- size

Source code:
```CPL
0  {
1      start() {
2          u8 a  = -1;
3          u8 b  = 240;
4          i8 a1 = -1;
5          i8 b1 = 240;
6          exit 0;
7      }
8  }
```

Produced errors:
```
[WARN] (src/sem/size.c:25) Value -1 at line=2 lower then 0 for unsigned type u8, -1 < 0!
[WARN] (src/sem/size.c:32) Value 240 at line=5 too large for type i8 (240 >= 127)!
```

------------------------------

- rettype

Source code:
```CPL
1  {
2      function foo() => i32 {
3          return 0;
4      }
5  
6      function bar() => i64 {
7          return 0;
8      }
9  
10     start(i64 argc, ptr u64 argv) {
11         i32 a = foo();
12         i32 b = bar();
13         exit 0;
14     }
15 }
```

Produced errors:
```
[WARN] (src/sem/bitness.c:11) Danger shadow type cast at line 11. Different size [32] (b) and [64] (bar). Did you expect this?
[WARN] (src/sem/rettype.c:37) Unmatched return type in line=2. Should return bitness=32, but provide bitness=8
[WARN] (src/sem/rettype.c:37) Unmatched return type in line=6. Should return bitness=64, but provide bitness=8
```

------------------------------

- ownership

Source code:
```CPL
1  {
2      start(i64 argc, ptr u64 argv) {
3          i32 a = 10;
4          ptr u32 a_owner = ref a;
5  
6          i32 b = 10;
7          ptr u32 b_owner = ref b;
8          ptr u32 b_owner_owner = ref b_owner;
9          exit 0;
10     }
11 }
```

Produced errors:
```
[WARN] (src/sem/ownership.c:204) Owner own another owner! b -> b_owner -> b_owner_owner
[WARN] (src/sem/ownership.c:209) Variable b owned multiple times! Did you expect this?
```


