@[section(".text")] glob i32 a;
@[section(".text")] glob i32 b;
@[section(".text")] glob i32 c;

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | i32                  | b                    | 8     | -1    | 1     | 0     |     | +   |     |
| 0     | i32                  | a                    | 8     | -1    | 1     | 0     |     | +   |     |
| 2     | i32                  | c                    | 8     | -1    | 1     | 0     |     | +   |     |
| section              | align | vars                                                             |
| .text                | -1    | {0, 1, 2}                                                        |
/: