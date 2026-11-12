# Semantic static checker

The compiler has two optional analysis stages:

- AST analysis, enabled with `--ast-analysis`;
- HIR/IR analysis, enabled with `--ir-analysis`.

```bash
./builds/<platform>/cplc --ast-analysis --ir-analysis main.cpl
```

Use `--CSA` to run Cordell Static Analyzer only:

```bash
./builds/<platform>/cplc --CSA main.cpl
```

These checks are intentionally lightweight. They help catch mistakes before or during lowering, but CPL is still an unsafe low-level language.

## AST analysis

AST analysis works on the parsed source structure and symbol tables. It is meant to catch source-level mistakes such as:

- updating a `ro` variable;
- using a return value with an incompatible type;
- declaring a variable without initialization and then using it unsafely;
- returning a value that does not match the function return type;
- missing `return` in a non-`i0` function;
- missing `exit` in an entry point;
- wrong function argument count or type;
- unused non-`i0` function return value;
- invalid array access;
- duplicated branches;
- dead code;
- suspicious implicit conversions;
- `break` outside a loop or switch;
- using an `i0` result as a value;
- odd alignment in `@[align(N)]`.

## HIR/IR analysis

IR analysis runs after HIR and CFG construction. It can reason about lower-level program behavior and target-sensitive operations.

Typical checks include:

- null dereference;
- possible null dereference through function arguments;
- constant `if` conditions and dead branches;
- function call type checks after lowering;
- possible division by zero;
- possible division by one;
- syscall validation for the selected target platform.
