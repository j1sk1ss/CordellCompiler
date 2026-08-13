function foo();
function foo(i32 a);
function foo(u32 a);
function foo(i16 a);
function foo(u16 a);
function foo(i8 a);
function foo(i8 a, i8 b);
function foo(u8 a);
function foo(u8 a, u8 b);
function foo(i8 a, u8 b);
function foo(u8 a, i8 b);

:/ OUTPUT
| id    | name                 | virt                 | scope | args                                 | ret              | locals             | generic types      | resolutions        | ext | glb | ent | use | loc | var | gen | abi | wek | slf | nkd | inl | bod | vnm |
| 2     | foo                  | foo2                 | 1     | (u32 a)                              | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 9     | foo                  | foo9                 | 1     | (i8 a, u8 b)                         | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 1     | foo                  | foo1                 | 1     | (i32 a)                              | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 4     | foo                  | foo4                 | 1     | (u16 a)                              | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 3     | foo                  | foo3                 | 1     | (i16 a)                              | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 7     | foo                  | foo7                 | 1     | (u8 a)                               | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 10    | foo                  | foo10                | 1     | (u8 a, i8 b)                         | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 0     | foo                  | foo0                 | 1     | ()                                   | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 5     | foo                  | foo5                 | 1     | (i8 a)                               | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 6     | foo                  | foo6                 | 1     | (i8 a, i8 b)                         | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
| 8     | foo                  | foo8                 | 1     | (u8 a, u8 b)                         | NULL             | {}                 | {}                 | {}                 | +   |     |     |     |     |     |     |     |     |     |     |     |     |     |
/: