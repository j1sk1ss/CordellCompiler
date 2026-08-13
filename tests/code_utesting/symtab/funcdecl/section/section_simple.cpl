@[section(".text")] function foo() {
}
@[section(".text")] function baz() {
}
@[section(".bss")] function bar() {
}
start() {
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | baz                  | baz1                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 3     | _main                | _main                | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     | +   | +   | +   |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | foo0                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 2     | bar                  | bar2                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .text                | -1    | {}                                                               | {0, 1, 3}                                                        |
| .bss                 | -1    | {}                                                               | {2}                                                              |
/: