@[section(".text")] glob i32 a;
@[section(".data")] ro i32 b;

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | i32                  | b                    | 8     | -1    | 1     | 0     | +   |     |     |
| 0     | i32                  | a                    | 8     | -1    | 1     | 0     |     | +   |     |
| section              | align | vars                                                             |
| .text                | -1    | {0}                                                              |
| .data                | -1    | {1}                                                              |
/: