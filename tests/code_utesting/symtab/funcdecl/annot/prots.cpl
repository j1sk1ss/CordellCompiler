@[entry]
function bar(i32 a);
function bar(i32 a) {}
function foo() -> i32 {
    return 0;
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | foo                  | foo1                 | 1     | ()                                   | i32              | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 0     | bar                  | _main                | 1     | (i32 a)                              | NULL             | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .text                | -1    | {}                                                               | {0, 1}                                                           |
/: