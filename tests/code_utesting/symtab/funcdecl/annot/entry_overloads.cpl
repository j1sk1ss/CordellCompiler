function foo(i32 a) {
}
function foo(i64 a) {
}
@[entry]
function foo(i8 a) {
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | foo                  | foo1                 | 1     | (i64 a)                              | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | foo0                 | 1     | (i32 a)                              | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 2     | foo                  | _main                | 1     | (i8 a)                               | NULL             | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .text                | -1    | {}                                                               | {0, 1, 2}                                                        |
/: