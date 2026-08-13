@[section(".test")] glob arr a[10, i32];
@[section(".ro")] ro arr b[10, i32];
@[section(".text")] ro glob arr c[10, i32];
glob arr d[10, i32];

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | arr                  | b                    | 8     | -1    | 1     | 0     | +   |     |     |
| 3     | arr                  | d                    | 8     | -1    | 1     | 0     |     | +   |     |
| 0     | arr                  | a                    | 8     | -1    | 1     | 0     |     | +   |     |
| 2     | arr                  | c                    | 8     | -1    | 1     | 0     | +   | +   |     |
| section              | align | vars                                                             |
| .test                | -1    | {0}                                                              |
| .ro                  | -1    | {1}                                                              |
| .text                | -1    | {2}                                                              |
| .data                | -1    | {3}                                                              |
/: