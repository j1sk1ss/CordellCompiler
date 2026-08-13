container smth {
    i32 a;
    u32 b;
}

glob i32 a;
glob u32 b;

glob smth c;

:/ OUTPUT
| id    | type                 | name                 | align | par   | scope | ptr   | ro  | glb | ext |
| 1     | u32                  | b                    | 8     | -1    | 2     | 0     |     |     |     |
| 4     | smth                 | c                    | 8     | -1    | 1     | 0     |     | +   |     |
| 3     | u32                  | b                    | 8     | -1    | 1     | 0     |     | +   |     |
| 0     | i32                  | a                    | 8     | -1    | 2     | 0     |     |     |     |
| 2     | i32                  | a                    | 8     | -1    | 1     | 0     |     | +   |     |
/: