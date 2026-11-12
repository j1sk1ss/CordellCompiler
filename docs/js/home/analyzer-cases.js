// Source cases and diagnostic wording are taken from the CSA test suite.
export const analyzerCases = {
  "null-return": {
    "label": "Null pointers",
    "test": "hir/null_deref/return.cpl",
    "line": 7,
    "column": 10,
    "severity": "warning",
    "phase": "HIR / DATA FLOW",
    "message": "Possible NULL-dereference error (variable 'a' is NULL)!",
    "explanation": "The zero returned by foo() reaches a. The dereference happens after the call, but the value is still null.",
    "file": "null-return.cpl",
    "code": "function foo() -> ptr i32 {\n    return 0;\n}\n\nstart() {\n    ptr i32 a = foo();\n    exit dref a;\n}\n"
  },
  "buffer-bounds": {
    "label": "Buffer bounds",
    "test": "hir/bad_buffer_move/simple.cpl",
    "line": 3,
    "column": 8,
    "severity": "error",
    "phase": "HIR / BUFFER BOUNDS",
    "message": "Possible buffer overflow! Current buffer 'something' is moved by Value that can be not in [0, 10)",
    "explanation": "A ten-element array has indices 0 through 9. Accessing something[11] steps beyond its storage.",
    "file": "buffer-bounds.cpl",
    "code": "start() {\n    arr something[10, i8];\n    i8 a = something[11];\n    i8 b = (ref something) + 11;\n    ptr i8 head = (ref something) + 12;\n    exit 0;\n}\n"
  },
  "divide-zero": {
    "label": "Division by zero",
    "test": "hir/div_by_zero/simple.cpl",
    "line": 3,
    "column": 10,
    "severity": "warning",
    "phase": "HIR / ARITHMETIC",
    "message": "Division by zero error! (variable 'a' is '0')!",
    "explanation": "The denominator is a variable, not a literal zero. CSA follows its assigned value into the division.",
    "file": "divide-zero.cpl",
    "code": "start() {\n    i32 a = 0;\n    exit (10 / a) as u8;\n}\n"
  },
  "readonly": {
    "label": "Read-only data",
    "test": "ast/ro.cpl",
    "line": 3,
    "column": 7,
    "severity": "error",
    "phase": "AST / DECLARATIONS",
    "message": "Read-only variable 'a' is used to store a value!",
    "explanation": "The declaration makes a read-only. The later assignment attempts to overwrite it.",
    "file": "readonly.cpl",
    "code": "start() {\n    ro i8 a = 0;\n    a = 1;\n    exit 0;\n}\n"
  }
};
