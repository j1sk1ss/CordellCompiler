@[entry]
function foo(i32 a) {
    foo(a + 1);
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 0     | foo                  | _main                | 1     | (i32 a)                              | NULL             | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .text                | -1    | {}                                                               | {0}                                                              |
/: