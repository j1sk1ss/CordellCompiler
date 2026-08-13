@[entry("_main")]
function main_entry() -> i8 {
    exit 0;
}

@[entry("start")]
function start_entry() -> i8 {
    exit 0;
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | start_entry          | start                | 1     | ()                                   | i8               | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| 0     | main_entry           | _main                | 1     | ()                                   | i8               | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
| section              | align | vars                                                             | funcs                                                            |
| .text                | -1    | {}                                                               | {0, 1}                                                           |
/: