# Binary and unary operations
Obviously this language supports the certain set of binary operations from C-language, Rust-language, Python, etc.

| Operation              | Description                                 | Example    | Associativity |
|------------------------|---------------------------------------------|------------|---------------|
| `+`                    | Addition                                    | `X` + `Y`  | Left to Right |
| `-`                    | Subtraction                                 | `X` - `Y`  | Left to Right |
| `*`                    | Multiplication                              | `X` * `Y`  | Left to Right |
| `/`                    | Division                                    | `X` / `Y`  | Left to Right |
| `%`                    | Module                                      | `X` % `Y`  | Left to Right |
| `==`                   | Equality                                    | `X` == `Y` | Left to Right |
| `!=`                   | Inequality                                  | `X` != `Y` | Left to Right |
| `not`                  | Negation                                    | not `X`    | Right to Left |
| `+=` `-=` `*=` `/=` `&=` `\|=` `%=` | Update operations              | `X` += `Y` | Right to Left |
| `>` `>=` `<` `<=`      | Comparison                                  | `X` >= `Y` | Left to Right |
| `&&` `\|\|`            | Logic operations (Lazy Evaluations support) | `X` && `Y` | Left to Right |
| `>>` `<<` `&` `\|` `^` | Bit operations                              | `X` >> `Y` | Left to Right |
