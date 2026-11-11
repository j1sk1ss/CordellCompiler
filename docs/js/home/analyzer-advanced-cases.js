// More involved CSA cases, checked through the HIR test harness.
export const advancedAnalyzerCases = {
  "call-traces": {
    "label": "Across calls",
    "test": "hir/null_deref/func_deref.cpl",
    "line": 6,
    "column": 10,
    "severity": "warning",
    "phase": "HIR / CALL CONTEXTS",
    "message": "Possible NULL-dereference error (variable 'msg' is NULL)!",
    "explanation": "foo() receives only NULL. bar() has one NULL caller and one non-NULL caller, so CSA reports its shared dereference as a possible error and traces both call sites.",
    "trace": [
      {
        "line": 11,
        "kind": "NULL",
        "text": "bar(0 as ptr i8) passes NULL"
      },
      {
        "line": 12,
        "kind": "NON-NULL",
        "text": "bar(1 as ptr i8) passes a non-NULL value"
      }
    ],
    "file": "call-traces.cpl",
    "code": "function foo(ptr i8 msg) {\n    dref msg = 0 as i8;\n}\n\nfunction bar(ptr i8 msg) {\n    dref msg = 0 as i8;\n}\n\nstart() {\n    foo(0 as ptr i8);\n    bar(0 as ptr i8);\n    bar(1 as ptr i8);\n}\n"
  },
  "guarded-division": {
    "label": "Guarded calls",
    "test": "hir/div_by_zero/function.cpl",
    "line": 2,
    "column": 12,
    "severity": "warning",
    "phase": "HIR / ARGUMENT CONSTRAINTS",
    "message": "Possible division by zero here! a can be zero!",
    "explanation": "The same function is called with and without a guard. The unguarded call leaves zero possible; the guarded call restricts a to non-zero values.",
    "trace": [
      {
        "line": 6,
        "kind": "UNGUARDED",
        "text": "foo(a) accepts a value that could be zero"
      },
      {
        "line": 7,
        "kind": "GUARD",
        "text": "a != 0 restricts the next call"
      },
      {
        "line": 8,
        "kind": "NON-ZERO",
        "text": "foo(a) is reached after the check"
      }
    ],
    "file": "guarded-division.cpl",
    "code": "function foo(i32 a) -> i32 {\n    return 100 / a;\n}\n\nstart(i32 a) {\n    foo(a);\n    if a != 0; {\n        foo(a);\n    }\n}\n",
    "variant": {
      "file": "guarded-division-fixed.cpl",
      "code": "function foo(i32 a) -> i32 {\n    return 100 / a;\n}\n\nstart(i32 a) {\n    if a != 0; {\n        foo(a);\n    }\n}\n",
      "line": 2,
      "column": 12,
      "severity": "clear",
      "phase": "HIR / ARGUMENT CONSTRAINTS",
      "message": "No division-by-zero warning on the guarded path.",
      "explanation": "Only the call inside a != 0 remains. CSA no longer reports possible division by zero. It still reports the unused return value.",
      "trace": [
        {
          "line": 6,
          "kind": "GUARD",
          "text": "The remaining call requires a != 0"
        },
        {
          "line": 7,
          "kind": "NON-ZERO",
          "text": "foo(a) receives a non-zero argument"
        }
      ],
      "drawing": "guarded-division-fixed"
    }
  },
  "symbolic-bounds": {
    "label": "Unknown indices",
    "test": "hir/bad_buffer_move/complex.cpl",
    "line": 5,
    "column": 8,
    "severity": "error",
    "phase": "HIR / SYMBOLIC BOUNDS",
    "message": "Possible buffer overflow! Current buffer '_buffer' is moved by Value that can be not in [0, 10)",
    "explanation": "argc comes from outside the program. CSA checks whether its possible values fit the buffer, rather than looking for a hard-coded bad index. The later guard cannot protect an earlier access.",
    "trace": [
      {
        "line": 4,
        "kind": "INPUT",
        "text": "argc has no proven buffer bounds"
      },
      {
        "line": 5,
        "kind": "ACCESS",
        "text": "_buffer[argc] is used before a check"
      },
      {
        "line": 9,
        "kind": "LATER",
        "text": "The range check happens after the access"
      }
    ],
    "file": "symbolic-bounds.cpl",
    "code": "@[section(\".bss\")]\nglob arr _buffer[10, i8];\n\nstart(i32 argc, ptr ptr i8 argv) {\n    i8 value = _buffer[argc];\n    ptr i8 head = ref _buffer;\n    head += argc;\n\n    if argc < 0 || argc > 10; exit 0;\n    head += argc;\n}\n"
  },
  "path-exclusion": {
    "label": "Impossible paths",
    "test": "hir/dead_branch.cpl",
    "line": 7,
    "column": 8,
    "severity": "warning",
    "phase": "HIR / PATH CONDITIONS",
    "message": "Can't reach the 'then' branch! Consider to refactor the code.",
    "explanation": "Any a between 11 and 20 has already returned. Reaching the second if means a <= 10 or a > 20. Under that condition, a == 15 is impossible.",
    "trace": [
      {
        "line": 3,
        "kind": "RANGE",
        "text": "10 < a <= 20 enters the first branch"
      },
      {
        "line": 4,
        "kind": "RETURN",
        "text": "Those values leave the function here"
      },
      {
        "line": 7,
        "kind": "IMPOSSIBLE",
        "text": "a == 15 cannot hold on the surviving path"
      }
    ],
    "file": "path-exclusion.cpl",
    "code": "@[entry]\nfunction foo(i32 a) {\n    if a > 10 && a <= 20; {\n        return 1;\n    }\n\n    if a == 15; {\n        return 10;\n    }\n\n    return 20;\n}\n"
  }
};
