@[section(".bss")]
function foo() {
}
@[section(".test")]
function bar() {
}
@[entry]
function baz() {
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | bar                  | bar1                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | foo0                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 2     | baz                  | _main                | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .bss                 | -1    | {}                                                               | {0}                                                              |
| .test                | -1    | {}                                                               | {1}                                                              |
| .text                | -1    | {}                                                               | {2}                                                              |
/: