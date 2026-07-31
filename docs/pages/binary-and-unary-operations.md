# Binary and unary operations

CPL supports C-like arithmetic, comparison, logical, bitwise, assignment, and cast expressions.

| Operation | Description | Example |
|---|---|---|
| `+`, `-`, `*`, `/`, `%` | arithmetic | `(1 + 2) * 3 - 2` |
| `==`, `!=`, `<`, `<=`, `>`, `>=` | comparisons | `x == 1`, `x <= 20` |
| `&&`, `\|\|` | logical AND/OR with lazy evaluation by default | `a && b` |
| `not` | boolean-like negation | `not x` |
| `neg` | bitwise inversion | `neg mask` |
| `&`, `\|`, `^`, `<<`, `>>` | bitwise operations | `mask & 0xFF` |
| `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `\|=`, `^=` | assignment and update | `x += 1` |
| `as` | explicit conversion | `('0' + x) as i8` |

## Precedence example

```cpl
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 x = (1 + 2) * 3 - 2;
    put_digit(x);
    exit 0;
}
```

## Boolean-like values

CPL has no separate boolean type. For conditions:

- `0` means false;
- any non-zero primitive value means true.

`not x` returns `1` when `x` is zero and `0` otherwise. It is not a raw bitwise inversion.

## Bitwise inversion

`neg x` in CPL means bitwise inversion of the value: every bit is flipped in the result. It is useful for masks and low-level code:

```cpl
i8 config = 0x13;
config &= neg 0x10;
```

For an 8-bit value, `neg 0x10` produces `0xEF`, so the example clears bit `0x10` in `config`.

Do not confuse CPL `neg` with the x86 `neg` instruction. On x86 targets CPL `neg` is lowered to the machine `not` instruction, because the language operation is bitwise inversion, not arithmetic two's-complement negation.

