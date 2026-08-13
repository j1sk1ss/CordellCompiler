@[entry]
function foo(i32 a);
function foo(i32 a) {
}

function foo();
function foo() {
}

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 1     | foo                  | foo1                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | _main                | 1     | (i32 a)                              | NULL             | {}                 | {}                 | {}                 |     |     | +   |     |     |     |     |     |     |     |     |     |     |     |
/: