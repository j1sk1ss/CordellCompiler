@[section(".text")] glob i32 r;
glob i32 a;
@[section(".test")] glob i32 b;
@[section(".test")] ro i32 c;
glob i32 d;
@[section(".life-is-strange")] glob i32 t;
ro i32 y;

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | i32                  | a                    | 8     | -1    | 1     | 0     |     | +   |     |
| 4     | i32                  | d                    | 8     | -1    | 1     | 0     |     | +   |     |
| 3     | i32                  | c                    | 8     | -1    | 1     | 0     | +   |     |     |
| 0     | i32                  | r                    | 8     | -1    | 1     | 0     |     | +   |     |
| 5     | i32                  | t                    | 8     | -1    | 1     | 0     |     | +   |     |
| 6     | i32                  | y                    | 8     | -1    | 1     | 0     | +   |     |     |
| 2     | i32                  | b                    | 8     | -1    | 1     | 0     |     | +   |     |
| section              | align | vars                                                             |
| .text                | -1    | {0}                                                              |
| .data                | -1    | {1, 4}                                                           |
| .test                | -1    | {2, 3}                                                           |
| .life-is-strange     | -1    | {5}                                                              |
| .rodata              | -1    | {6}                                                              |
/: