from __future__ import annotations
from dataclasses import dataclass
from typing import List, Optional
import sys

KEYWORDS = {
    "from", "import",
    "extern", "exfunc",
    "glob", "ro",
    "function", "start",
    "if", "else",
    "loop", "while",
    "switch", "case", "default",
    "return", "exit",
    "break", "lis",
    "syscall",
    "asm",
    "not", "ref", "dref",
    "f64", "i64", "u64", "f32", "i32", "u32", "i16", "u16", "i8", "u8",
    "str", "arr", "ptr",
}

MULTI = [
    "=>",
    "||", "&&",
    "==", "!=", "<=", ">=",
    "<<", ">>",
    "+=", "-=", "*=", "/=", "%=",
    "|=", "^=", "&=",
]

SINGLE = set("{}()[];,+-*/%<>=&|^:")

@dataclass(frozen=True)
class Token:
    kind: str
    value: str
    line: int
    col: int

class LexError(Exception):
    pass

def lex(src: str) -> List[Token]:
    tokens: List[Token] = []
    i = 0
    line = 1
    col = 1
    n = len(src)

    def cur() -> str:
        return src[i] if i < n else "\0"

    def peek(k: int = 1) -> str:
        j = i + k
        return src[j] if j < n else "\0"

    def advance(k: int = 1) -> None:
        nonlocal i, line, col
        for _ in range(k):
            if i >= n:
                return
            ch = src[i]
            i += 1
            if ch == "\n":
                line += 1
                col = 1
            else:
                col += 1

    def add(kind: str, value: str, l: int, c: int) -> None:
        tokens.append(Token(kind, value, l, c))

    def is_hex_digit(c: str) -> bool:
        return c.isdigit() or ("a" <= c.lower() <= "f")

    while i < n:
        ch = cur()

        if ch.isspace():
            advance()
            continue

        # comment block: ":" ... ":" (no nesting)
        if ch == ":":
            l0, c0 = line, col
            advance()
            start = i
            while i < n and cur() != ":":
                advance()
            if i >= n:
                raise LexError(f"Unterminated comment starting at {l0}:{c0}")
            text = src[start:i]
            advance()  # closing ':'
            add("COMMENT", text, l0, c0)
            continue

        # string literal
        if ch == '"':
            l0, c0 = line, col
            advance()
            buf: List[str] = []
            while i < n:
                c = cur()
                if c == '"':
                    break
                if c == "\\":
                    advance()
                    if i >= n:
                        raise LexError(f"Unterminated string at {l0}:{c0}")
                    buf.append(cur())
                    advance()
                    continue
                if c == "\n":
                    raise LexError(f"Newline in string literal at {line}:{col}")
                buf.append(c)
                advance()
            if i >= n or cur() != '"':
                raise LexError(f"Unterminated string at {l0}:{c0}")
            advance()
            add("STRING", "".join(buf), l0, c0)
            continue

        # multi-char operators
        matched = False
        for op in MULTI:
            if src.startswith(op, i):
                l0, c0 = line, col
                advance(len(op))
                add("SYM", op, l0, c0)
                matched = True
                break
        if matched:
            continue

        # single-char
        if ch in SINGLE:
            l0, c0 = line, col
            advance()
            add("SYM", ch, l0, c0)
            continue

        # integer literal: 0x... / 0b... / decimal
        if ch.isdigit():
            l0, c0 = line, col

            # hex: 0x...
            if ch == "0" and peek(1) in {"x", "X"}:
                advance(2) # 0x
                start = i
                while i < n and is_hex_digit(cur()):
                    advance()
                if i == start:
                    raise LexError(f"Invalid hex literal (need hex digits) at {l0}:{c0}")
                lexeme = src[(start - 2):i] # include '0x'
                add("INT", lexeme, l0, c0)
                continue

            # bin: 0b...
            if ch == "0" and peek(1) in {"b", "B"}:
                advance(2) # 0b
                start = i
                while i < n and cur() in {"0", "1"}:
                    advance()
                if i == start:
                    raise LexError(f"Invalid binary literal (need 0/1 digits) at {l0}:{c0}")
                lexeme = src[(start - 2):i] # include '0b'
                add("INT", lexeme, l0, c0)
                continue

            start = i
            while i < n and cur().isdigit():
                advance()
            add("INT", src[start:i], l0, c0)
            continue

        # identifier / keyword
        if ch.isalpha():
            l0, c0 = line, col
            start = i
            advance()
            while i < n and (cur().isalnum() or cur() == "_"):
                advance()
            text = src[start:i]
            if text in KEYWORDS:
                add("KW", text, l0, c0)
            else:
                add("IDENT", text, l0, c0)
            continue

        raise LexError(f"Unexpected character {ch!r} at {line}:{col}")

    tokens.append(Token("EOF", "EOF", line, col))
    return tokens

class ParseError(Exception):
    pass

class Parser:
    ASSIGN_OPS = {"=", "+=", "-=", "*=", "/=", "%=", "|=", "^=", "&="}

    def __init__(self, tokens: List[Token]):
        self.toks = tokens
        self.pos = 0

    def peek(self, k: int = 0) -> Token:
        idx = self.pos + k
        if idx >= len(self.toks):
            return self.toks[-1]
        return self.toks[idx]

    def at(self, kind: str, value: Optional[str] = None) -> bool:
        t = self.peek()
        if t.kind != kind:
            return False
        return value is None or t.value == value

    def expect(self, kind: str, value: Optional[str] = None) -> Token:
        t = self.peek()
        if t.kind != kind or (value is not None and t.value != value):
            exp = f"{kind}" + (f" {value!r}" if value is not None else "")
            got = f"{t.kind} {t.value!r}"
            raise ParseError(f"Expected {exp}, got {got} at {t.line}:{t.col}")
        self.pos += 1
        return t

    def match(self, kind: str, value: Optional[str] = None) -> bool:
        if self.at(kind, value):
            self.pos += 1
            return True
        return False

    def is_type_start(self) -> bool:
        t = self.peek()
        if t.kind != "KW":
            return False
        if t.value in {"f64", "i64", "u64", "f32", "i32", "u32", "i16", "u16", "i8", "u8", "str", "ptr"}:
            return True
        if t.value == "arr" and self.peek(1).kind == "SYM" and self.peek(1).value == "[":
            return True
        return False

    def is_literal_start(self) -> bool:
        return (
            self.at("INT")
            or self.at("STRING")
            or (self.at("KW") and self.peek().value in {"true", "false"})
        )

    def is_expression_start(self) -> bool:
        t = self.peek()
        if t.kind in {"INT", "STRING", "IDENT"}:
            return True
        if t.kind == "KW" and t.value in {"true", "false", "not", "ref", "dref"}:
            return True
        if t.kind == "SYM" and t.value in {"(", "+", "-"}:
            return True
        return False

    def parse_program(self) -> None:
        self.expect("SYM", "{")
        while not self.at("SYM", "}") and not self.at("EOF"):
            self.parse_top_item()
        self.expect("SYM", "}")
        self.expect("EOF")

    def parse_top_item(self) -> None:
        if self.at("KW", "start"):
            self.parse_start_function(); return
        if self.at("KW", "from"):
            self.parse_import_op(); return
        if self.at("KW", "extern"):
            self.parse_extern_op(); return

        self.parse_storage_opt()
        self.parse_top_decl()

    def parse_import_op(self) -> None:
        self.expect("KW", "from")
        self.expect("STRING")
        self.expect("KW", "import")
        if self.at("IDENT"):
            self.expect("IDENT")
            while self.match("SYM", ","):
                self.expect("IDENT")

    def parse_extern_op(self) -> None:
        self.expect("KW", "extern")
        if self.at("KW", "exfunc"):
            self.expect("KW", "exfunc")
            self.expect("IDENT")
        else:
            if not self.is_type_start():
                t = self.peek()
                raise ParseError(f"Expected 'exfunc' or type after extern, got {t.kind} {t.value!r} at {t.line}:{t.col}")
            self.parse_type()
            self.expect("IDENT")

    def parse_storage_opt(self) -> None:
        if self.at("KW", "glob") or self.at("KW", "ro"):
            self.pos += 1

    def parse_top_decl(self) -> None:
        if self.at("KW", "function"):
            self.parse_function_def(); return
        if self.is_type_start():
            self.parse_var_decl(); return
        t = self.peek()
        raise ParseError(f"Expected function_def or var_decl at top-level, got {t.kind} {t.value!r} at {t.line}:{t.col}")

    def parse_function_def(self) -> None:
        self.expect("KW", "function")
        self.expect("IDENT")
        self.expect("SYM", "(")
        if self.is_type_start():
            self.parse_param_list()
        self.expect("SYM", ")")
        self.expect("SYM", "=>")
        self.parse_type()
        self.parse_block()

    def parse_start_function(self) -> None:
        self.expect("KW", "start")
        self.expect("SYM", "(")
        if self.is_type_start():
            self.parse_param_list()
        self.expect("SYM", ")")
        self.parse_block()

    def parse_param_list(self) -> None:
        self.parse_param()
        while self.match("SYM", ","):
            self.parse_param()

    def parse_param(self) -> None:
        self.parse_type()
        self.expect("IDENT")
        if self.match("SYM", "="):
            self.parse_literal()

    def parse_block(self) -> None:
        self.expect("SYM", "{")
        while not self.at("SYM", "}") and not self.at("EOF"):
            self.parse_statement()
        self.expect("SYM", "}")

    def parse_statement(self) -> None:
        t = self.peek()

        if self.is_type_start():
            self.parse_var_decl(); return

        if self.at("KW", "arr") and self.peek(1).kind == "IDENT":
            self.parse_arr_decl(); return

        if self.at("KW", "if"):
            self.parse_if_statement(); return
        if self.at("KW", "loop"):
            self.expect("KW", "loop"); self.parse_block(); return
        if self.at("KW", "while"):
            self.parse_while_statement(); return
        if self.at("KW", "switch"):
            self.parse_switch_statement(); return
        if self.at("KW", "return"):
            self.parse_return_statement(); return
        if self.at("KW", "exit"):
            self.expect("KW", "exit"); self.parse_expression(); self.expect("SYM", ";"); return
        if self.at("KW", "break"):
            self.expect("KW", "break"); self.expect("SYM", ";"); return
        if self.at("KW", "lis"):
            self.expect("KW", "lis"); self.expect("SYM", ";"); return
        if self.at("KW", "syscall"):
            self.parse_syscall_statement(); return
        if self.at("KW", "asm"):
            self.parse_asm_block(); return
        if t.kind == "COMMENT":
            self.pos += 1; return
        if self.at("SYM", "{"):
            self.parse_block(); return

        if self.is_expression_start():
            self.parse_expression()
            self.expect("SYM", ";")
            return

        raise ParseError(f"Unexpected token {t.kind} {t.value!r} at {t.line}:{t.col} in statement")

    def parse_var_decl(self) -> None:
        self.parse_type()
        self.expect("IDENT")
        if self.match("SYM", "="):
            self.parse_expression()
        self.expect("SYM", ";")

    def parse_arr_decl(self) -> None:
        self.expect("KW", "arr")
        self.expect("IDENT")
        self.expect("SYM", "[")
        if self.at("IDENT"):
            self.pos += 1
        else:
            self.parse_literal()
        self.parse_type()
        self.expect("SYM", "]")
        if self.match("SYM", "="):
            if self.at("SYM", "{"):
                self.parse_arr_value()
            else:
                self.parse_expression()
        self.expect("SYM", ";")

    def parse_arr_value(self) -> None:
        self.expect("SYM", "{")
        if self.at("IDENT") or self.is_literal_start():
            self.parse_arr_elem()
            while self.match("SYM", ","):
                self.parse_arr_elem()
        self.expect("SYM", "}")

    def parse_arr_elem(self) -> None:
        if self.at("IDENT"):
            self.pos += 1
        else:
            self.parse_literal()

    def parse_if_statement(self) -> None:
        self.expect("KW", "if")
        self.parse_expression()
        self.expect("SYM", ";")
        self.parse_block()
        if self.match("KW", "else"):
            self.parse_block()

    def parse_while_statement(self) -> None:
        self.expect("KW", "while")
        self.parse_expression()
        self.expect("SYM", ";")
        self.parse_block()

    def parse_switch_statement(self) -> None:
        self.expect("KW", "switch")
        self.expect("SYM", "(")
        self.parse_expression()
        self.expect("SYM", ")")
        self.expect("SYM", "{")
        while self.at("KW", "case"):
            self.expect("KW", "case")
            self.parse_literal()
            self.expect("SYM", ";")
            self.parse_block()
        if self.at("KW", "default"):
            self.expect("KW", "default")
            self.parse_block()
        self.expect("SYM", "}")

    def parse_return_statement(self) -> None:
        self.expect("KW", "return")
        if self.is_expression_start():
            self.parse_expression()
        self.expect("SYM", ";")

    def parse_syscall_statement(self) -> None:
        # syscall_statement = "syscall" "(" [ expression_list ] ")" ";"
        self.expect("KW", "syscall")
        self.expect("SYM", "(")
        if self.is_expression_start():
            self.parse_expression_list()
        self.expect("SYM", ")")
        self.expect("SYM", ";")

    def parse_expression_list(self) -> None:
        # expression_list = expression { "," expression }
        self.parse_expression()
        while self.match("SYM", ","):
            self.parse_expression()

    def parse_syscall_arg(self) -> None:
        if self.at("IDENT"):
            self.pos += 1
        else:
            self.parse_literal()

    def parse_asm_block(self) -> None:
        self.expect("KW", "asm")
        self.expect("SYM", "(")
        if self.at("IDENT") or self.is_literal_start():
            self.parse_asm_arg()
            while self.match("SYM", ","):
                self.parse_asm_arg()
        self.expect("SYM", ")")
        self.expect("SYM", "{")
        while self.at("STRING"):
            self.expect("STRING")
            self.match("SYM", ",")
        self.expect("SYM", "}")

    def parse_asm_arg(self) -> None:
        if self.at("IDENT"):
            self.pos += 1
        else:
            self.parse_literal()

    def parse_expression(self) -> None:
        self.parse_assign()

    def parse_assign(self) -> None:
        # assign = logical_or [ assign_op assign ]
        self.parse_logical_or()
        if self.at("SYM") and self.peek().value in self.ASSIGN_OPS:
            self.pos += 1
            self.parse_assign()

    def parse_logical_or(self) -> None:
        self.parse_logical_and()
        while self.match("SYM", "||"):
            self.parse_logical_and()

    def parse_logical_and(self) -> None:
        self.parse_bit_or()
        while self.match("SYM", "&&"):
            self.parse_bit_or()

    def parse_bit_or(self) -> None:
        self.parse_bit_xor()
        while self.match("SYM", "|"):
            self.parse_bit_xor()

    def parse_bit_xor(self) -> None:
        self.parse_bit_and()
        while self.match("SYM", "^"):
            self.parse_bit_and()

    def parse_bit_and(self) -> None:
        self.parse_equality()
        while self.match("SYM", "&"):
            self.parse_equality()

    def parse_equality(self) -> None:
        self.parse_relational()
        while self.at("SYM") and self.peek().value in {"==", "!="}:
            self.pos += 1
            self.parse_relational()

    def parse_relational(self) -> None:
        self.parse_shift()
        while self.at("SYM") and self.peek().value in {"<", "<=", ">", ">="}:
            self.pos += 1
            self.parse_shift()

    def parse_shift(self) -> None:
        self.parse_add()
        while self.at("SYM") and self.peek().value in {"<<", ">>"}:
            self.pos += 1
            self.parse_add()

    def parse_add(self) -> None:
        self.parse_mul()
        while self.at("SYM") and self.peek().value in {"+", "-"}:
            self.pos += 1
            self.parse_mul()

    def parse_mul(self) -> None:
        self.parse_unary()
        while self.at("SYM") and self.peek().value in {"*", "/", "%"}:
            self.pos += 1
            self.parse_unary()

    def parse_unary(self) -> None:
        # unary_op = "not" | "+" | "-" | "ref" | "dref"
        if (self.at("KW") and self.peek().value in {"not", "ref", "dref"}) or (self.at("SYM") and self.peek().value in {"+", "-"}):
            self.pos += 1
            self.parse_unary()
            return
        self.parse_postfix()

    def parse_postfix(self) -> None:
        self.parse_primary()
        while True:
            if self.match("SYM", "("):
                if self.is_expression_start():
                    self.parse_arg_list()
                self.expect("SYM", ")")
                continue
            if self.match("SYM", "["):
                self.parse_expression()
                while self.match("SYM", ","):
                    self.parse_expression()
                self.expect("SYM", "]")
                continue
            break

    def parse_primary(self) -> None:
        if self.is_literal_start():
            self.parse_literal()
            return
        if self.at("IDENT"):
            self.pos += 1
            return
        if self.match("SYM", "("):
            self.parse_expression()
            self.expect("SYM", ")")
            return
        t = self.peek()
        raise ParseError(f"Expected primary, got {t.kind} {t.value!r} at {t.line}:{t.col}")

    def parse_arg_list(self) -> None:
        self.parse_expression()
        while self.match("SYM", ","):
            self.parse_expression()

    def parse_type(self) -> None:
        t = self.peek()
        if t.kind != "KW":
            raise ParseError(f"Expected type, got {t.kind} {t.value!r} at {t.line}:{t.col}")

        if t.value in {"f64", "i64", "u64", "f32", "i32", "u32", "i16", "u16", "i8", "u8", "str"}:
            self.pos += 1
            return

        if t.value == "ptr":
            self.pos += 1
            self.parse_type()
            return

        if t.value == "arr":
            self.pos += 1
            self.expect("SYM", "[")
            self.expect("INT")
            self.expect("SYM", ",")
            self.parse_type()
            self.expect("SYM", "]")
            return

        raise ParseError(f"Unknown type keyword {t.value!r} at {t.line}:{t.col}")

    def parse_literal(self) -> None:
        if self.at("INT") or self.at("STRING"):
            self.pos += 1
            return
        if self.at("KW") and self.peek().value in {"true", "false"}:
            self.pos += 1
            return
        t = self.peek()
        raise ParseError(f"Expected literal, got {t.kind} {t.value!r} at {t.line}:{t.col}")

def main() -> int:
    if len(sys.argv) > 2:
        print("Usage: python cpl_check.py [file]", file=sys.stderr)
        return 2

    if len(sys.argv) == 2:
        with open(sys.argv[1], "r", encoding="utf-8") as f:
            src = f.read()
    else:
        src = sys.stdin.read()

    try:
        toks = lex(src)
    except LexError as e:
        print(f"LEX ERROR: {e}", file=sys.stderr)
        return 1

    try:
        Parser(toks).parse_program()
    except ParseError as e:
        print(f"PARSE ERROR: {e}", file=sys.stderr)
        return 1

    print("OK: program accepted by grammar")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
