function foo<T>(T a, T b) -> T {
}
function bar<T>() {
}

start() {
    foo<i32>(1, 1);
    foo<i8>(1, 1);
    bar<i0>();
    bar<f64>();
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | bar                  | bar1                 | 1     | ()                                   | NULL             | {}                 | {1}                | {3, 4}             |     |     |     |     |     |     | +   |     |     |     |     |     |     |     |
| 4     | bar                  | bar4__i0             | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 3     | bar                  | bar3__f64            | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | foo0                 | 1     | (T a, T b)                           | T                | {}                 | {0}                | {5, 6}             |     |     |     |     |     |     | +   |     |     |     |     |     |     |     |
| 5     | foo                  | foo5__i8             | 1     | (T a, T b)                           | T                | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 6     | foo                  | foo6__i32            | 1     | (T a, T b)                           | T                | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 2     | _main                | _main                | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     | +   | +   | +   |     |     |     |     |     |     |     |     |     |     |
/: