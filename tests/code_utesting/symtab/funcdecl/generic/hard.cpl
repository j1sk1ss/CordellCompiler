: BUG :
function foo<T>(T a, i32 b) -> T {
    10;
}

function foo<T>(T a, i64 b) -> T {
    20;
}

start() {
    exit (foo<i32>(10, 10 as i32) + foo<i32>(10, 10 as i64)) as u8;
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | foo                  | foo1                 | 1     | (T a, i64 b)                         | T                | {}                 | {3}                | {3}                |     |     |     |     |     |     | +   |     |     |     |     |     |     |     |
| 3     | foo                  | foo3__i32            | 1     | (T a, i64 b)                         | T                | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | foo0                 | 1     | (T a, i32 b)                         | T                | {}                 | {0}                | {}                 |     |     |     |     |     |     | +   |     |     |     |     |     |     |     |
| 2     | _main                | _main                | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     | +   | +   | +   |     |     |     |     |     |     |     |     |     |     |
/: