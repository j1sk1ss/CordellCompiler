function foo(@[align(16)] i32 a, @[align(8)] i32 b) {
}

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | i32                  | b                    | 8     | -1    | 2     | 0     |     |     |     |
| 0     | i32                  | a                    | 16    | -1    | 2     | 0     |     |     |     |
| section              | align | vars                                                             |
| .text                | -1    | {}                                                               |
/: