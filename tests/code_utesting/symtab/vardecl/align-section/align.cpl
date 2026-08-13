@[align(16)] glob i32 a;
@[align(64)] glob i32 b;
@[align(64)] glob i32 c;

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | i32                  | b                    | 64    | -1    | 1     | 0     |     | +   |     |
| 0     | i32                  | a                    | 16    | -1    | 1     | 0     |     | +   |     |
| 2     | i32                  | c                    | 64    | -1    | 1     | 0     |     | +   |     |
| section              | align | vars                                                             |
| .data                | -1    | {0, 1, 2}                                                        |
/: