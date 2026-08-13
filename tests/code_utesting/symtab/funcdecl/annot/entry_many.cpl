function foo() {
}
@[entry]
function bar() {
}
function baz();

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | bar                  | _main                | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | foo0                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 2     | baz                  | baz2                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .text                | -1    | {}                                                               | {0, 1, 2}                                                        |
/: