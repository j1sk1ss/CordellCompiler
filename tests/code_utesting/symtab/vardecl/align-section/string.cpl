@[section(".test")] glob arr a[0, i8] = "Hello world!";
@[section(".test2")] glob ptr i8 b = ref "Hello world!";

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | i8                   | b                    | 8     | -1    | 1     | 1     |     | +   |     |
| 0     | arr                  | a                    | 8     | -1    | 1     | 0     |     | +   |     |
| section              | align | vars                                                             |
| .test                | -1    | {0}                                                              |
| .test2               | -1    | {1}                                                              |
/: