@[entry]
@[section(".test")]
function foo() {
    @[section(".local")]
    function bar() {

    }
    bar();
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | bar                  | bar1                 | 3     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     | +   |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | _main                | 1     | ()                                   | NULL             | {1}                | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .test                | -1    | {}                                                               | {0}                                                              |
/: