import { Range, Position } from "vscode-languageserver/node";

type TokenKind =
  | "eof"
  | "ident"
  | "int"
  | "str"
  | "kw"
  | "op"
  | "punc";

type Token = {
  kind: TokenKind;
  text: string;
  start: number;
  end: number;
};

type ParseIssue = { message: string; range: Range };

const KEYWORDS = new Set([
  "start","exit","exfunc","function","return",
  "if","else","while","loop","switch","case","default",
  "glob","ro","dref","ref","ptr","lis","break","extern","from","import","syscall","asm","as",
  "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","not"
]);

const TYPE_KW = new Set([
  "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","ptr"
]);

const OPERATORS = [
  "||=","&&=","<<",">>","==","!=","<=",">=","&&","||",
  "+=","-=","*=","/=","%=","|=","^=","&=","=",
  "+","-","*","/","%","|","^","&","<",">", "=>"
].sort((a,b)=>b.length-a.length);

const PUNC = new Set(["{","}","(",")","[","]",",",";"]);

function isAlpha(ch: string) { return /[A-Za-z]/.test(ch); }
function isDigit(ch: string) { return /[0-9]/.test(ch); }
function isAlnum_(ch: string){ return /[A-Za-z0-9_]/.test(ch); }

function buildLineIndex(text: string): number[] {
  const lines = [0];
  for (let i = 0; i < text.length; i++) if (text[i] === "\n") lines.push(i + 1);
  return lines;
}

function offsetToPos(lines: number[], off: number): Position {
  let lo = 0, hi = lines.length - 1;
  while (lo <= hi) {
    const mid = (lo + hi) >> 1;
    if (lines[mid] <= off) lo = mid + 1;
    else hi = mid - 1;
  }
  const line = Math.max(0, hi);
  return Position.create(line, off - lines[line]);
}

function rangeOf(lines: number[], start: number, end: number): Range {
  return Range.create(offsetToPos(lines, start), offsetToPos(lines, end));
}

function lex(text: string): Token[] {
  const tokens: Token[] = [];
  let i = 0;

  const push = (kind: TokenKind, start: number, end: number) => {
    tokens.push({ kind, text: text.slice(start, end), start, end });
  };

  while (i < text.length) {
    const ch = text[i];

    // whitespace
    if (ch === " " || ch === "\t" || ch === "\r" || ch === "\n") { i++; continue; }

    // comment : ... :
    if (ch === ":") {
      const start = i;
      i++;
      while (i < text.length && text[i] !== ":") i++;
      if (i < text.length && text[i] === ":") i++;
      // comments ignored (no tokens)
      continue;
    }

    // string
    if (ch === "\"") {
      const start = i;
      i++;
      while (i < text.length) {
        if (text[i] === "\\" && i + 1 < text.length) { i += 2; continue; }
        if (text[i] === "\"") { i++; break; }
        i++;
      }
      push("str", start, i);
      continue;
    }

    // number: 0x / 0b / decimal
    if (isDigit(ch)) {
      const start = i;
      if (text.startsWith("0x", i) || text.startsWith("0X", i)) {
        i += 2;
        while (i < text.length && /[0-9a-fA-F]/.test(text[i])) i++;
      } else if (text.startsWith("0b", i) || text.startsWith("0B", i)) {
        i += 2;
        while (i < text.length && /[01]/.test(text[i])) i++;
      } else {
        while (i < text.length && isDigit(text[i])) i++;
      }
      push("int", start, i);
      continue;
    }

    // identifier / keyword
    if (isAlpha(ch) || ch === "_") {
      const start = i;
      i++;
      while (i < text.length && isAlnum_(text[i])) i++;
      const w = text.slice(start, i);
      push(KEYWORDS.has(w) ? "kw" : "ident", start, i);
      continue;
    }

    // operators
    let matchedOp = "";
    for (const op of OPERATORS) {
      if (text.startsWith(op, i)) { matchedOp = op; break; }
    }
    if (matchedOp) {
      push("op", i, i + matchedOp.length);
      i += matchedOp.length;
      continue;
    }

    // punctuation
    if (PUNC.has(ch) || ch === "]") {
      push("punc", i, i + 1);
      i++;
      continue;
    }
    if (ch === "]") { push("punc", i, i+1); i++; continue; }

    // unknown char
    push("punc", i, i + 1);
    i++;
  }

  tokens.push({ kind: "eof", text: "<eof>", start: text.length, end: text.length });
  return tokens;
}

class Parser {
  private t: Token[];
  private i = 0;
  private issues: ParseIssue[] = [];
  private lines: number[];

  constructor(text: string) {
    this.lines = buildLineIndex(text);
    this.t = lex(text);
  }

  run(): ParseIssue[] {
    this.parseProgram();
    return this.issues;
  }

  private cur(): Token { return this.t[this.i]; }
  private prev(): Token { return this.t[Math.max(0, this.i - 1)]; }
  private at(kind: TokenKind, text?: string): boolean {
    const c = this.cur();
    if (c.kind !== kind) return false;
    if (text !== undefined) return c.text === text;
    return true;
  }
  private match(kind: TokenKind, text?: string): boolean {
    if (!this.at(kind, text)) return false;
    this.i++;
    return true;
  }
  private expect(kind: TokenKind, text?: string, msg?: string): boolean {
    if (this.match(kind, text)) return true;
    const c = this.cur();
    this.issues.push({
      message: msg ?? `Expected ${text ?? kind}, got '${c.text}'`,
      range: rangeOf(this.lines, c.start, c.end)
    });
    // advance to avoid infinite loops
    if (!this.at("eof")) this.i++;
    return false;
  }

  private syncToStatementEnd() {
    // consume until ';' or '}' or eof
    while (!this.at("eof") && !this.at("punc", ";") && !this.at("punc", "}")) this.i++;
    if (this.at("punc", ";")) this.i++;
  }

  // --- grammar ---
  private parseProgram() {
    this.expect("punc", "{", "Program must start with '{'");
    while (!this.at("eof") && !this.at("punc", "}")) {
      const before = this.i;
      this.parseTopItem();
      if (this.i === before) this.i++; // progress guard
    }
    this.expect("punc", "}", "Program must end with '}'");
  }

  private parseTopItem() {
    if (this.match("kw", "start")) {
      this.expect("punc", "(", "start: expected '('");
      this.parseParamListOpt();
      this.expect("punc", ")", "start: expected ')'");
      this.parseBlock();
      return;
    }

    if (this.match("kw", "from")) {
      this.expect("str", undefined, "from: expected string literal");
      this.expect("kw", "import", "from: expected 'import'");
      if (this.at("ident")) {
        this.i++;
        while (this.match("punc", ",")) this.expect("ident", undefined, "import: expected identifier");
      }
      return;
    }

    if (this.match("kw", "extern")) {
      if (this.match("kw", "exfunc")) {
        this.expect("ident", undefined, "extern exfunc: expected identifier");
      } else {
        this.parseType();
        this.expect("ident", undefined, "extern var: expected identifier");
      }
      this.expect("punc", ";", "extern: expected ';'");
      return;
    }

    // storage_opt
    this.match("kw", "glob");
    this.match("kw", "ro");

    if (this.match("kw", "function")) {
      this.expect("ident", undefined, "function: expected identifier");
      this.expect("punc", "(", "function: expected '('");
      this.parseParamListOpt();
      this.expect("punc", ")", "function: expected ')'");
    
      if (this.match("op", "=>")) {
        this.parseType();
      }
    
      this.parseBlock();
      return;
    }    

    // top-level var_decl
    this.parseVarOrArrDecl();
  }

  private parseBlock() {
    this.expect("punc", "{", "block: expected '{'");
    while (!this.at("eof") && !this.at("punc", "}")) {
      const before = this.i;
      this.parseStatement();
      if (this.i === before) this.i++; // progress guard
    }
    this.expect("punc", "}", "block: expected '}'");
  }

  private parseStatement() {
    if (this.at("kw", "if")) { this.i++; this.parseExpression(); this.expect("punc",";"); this.parseBlock(); if (this.match("kw","else")) this.parseBlock(); return; }
    if (this.at("kw", "while")) { this.i++; this.parseExpression(); this.expect("punc",";"); this.parseBlock(); return; }
    if (this.at("kw", "loop")) { this.i++; this.parseBlock(); return; }

    if (this.at("kw","switch")) {
      this.i++;
      this.expect("punc","(");
      this.parseExpression();
      this.expect("punc",")");
      this.expect("punc","{");
      while (this.match("kw","case")) {
        this.parseLiteral();
        this.expect("punc",";");
        this.parseBlock();
      }
      if (this.match("kw","default")) this.parseBlock();
      this.expect("punc","}");
      return;
    }

    if (this.at("kw","return")) { this.i++; if (!this.at("punc",";")) this.parseExpression(); this.expect("punc",";"); return; }
    if (this.at("kw","exit")) { this.i++; this.parseExpression(); this.expect("punc",";"); return; }
    if (this.at("kw","break")) { this.i++; this.expect("punc",";"); return; }
    if (this.at("kw","lis")) { this.i++; if (!this.at("punc",";")) this.parseExpression(); this.expect("punc",";"); return; }

    if (this.at("kw","syscall")) {
      this.i++;
      this.expect("punc","(");
      if (!this.at("punc",")")) {
        this.parseExpression();
        while (this.match("punc",",")) this.parseExpression();
      }
      this.expect("punc",")");
      this.expect("punc",";");
      return;
    }

    if (this.at("kw","asm")) {
      this.i++;
      this.expect("punc","(");
      if (!this.at("punc",")")) {
        // asm_args: ident | literal
        this.parseAsmArg();
        while (this.match("punc",",")) this.parseAsmArg();
      }
      this.expect("punc",")");
      this.expect("punc","{");
      while (!this.at("eof") && !this.at("punc","}")) {
        this.expect("str", undefined, "asm_line: expected string literal");
        this.match("punc", ",");
      }
      this.expect("punc","}");
      return;
    }

    // var_decl / arr_decl
    if (this.looksLikeDeclStart()) {
      this.parseVarOrArrDecl();
      return;
    }

    // expression_statement
    try {
      this.parseExpression();
      this.expect("punc",";");
    } catch {
      this.syncToStatementEnd();
    }
  }

  private looksLikeDeclStart(): boolean {
    const c = this.cur();
    if (c.kind === "kw" && (TYPE_KW.has(c.text))) return true;
    return false;
  }

  private parseVarOrArrDecl() {
    // arr_decl starts with: arr ident '[' ...
    if (this.at("kw","arr")) {
      const t1 = this.cur();
      const t2 = this.t[this.i+1];
      if (t2?.kind === "ident") {
        // arr_decl
        this.i++; // 'arr'
        this.expect("ident", undefined, "arr_decl: expected identifier");
        this.expect("punc","[", "arr_decl: expected '['");
        // (identifier | literal)
        if (this.at("ident")) this.i++;
        else this.parseLiteral();
        // then type
        this.parseType();
        this.expect("punc","]","arr_decl: expected ']'");
        if (this.match("op","=")) {
          // expression | arr_value
          if (this.match("punc","{")) {
            if (!this.at("punc","}")) {
              this.parseArrElem();
              while (this.match("punc",",")) this.parseArrElem();
            }
            this.expect("punc","}");
          } else {
            this.parseExpression();
          }
        }
        this.expect("punc",";","arr_decl: expected ';'");
        return;
      } else {
        // otherwise it's a type 'arr[...]' (var_decl)
      }
    }

    // var_decl: type ident [= expr] ;
    this.parseType();
    this.expect("ident", undefined, "var_decl: expected identifier");
    if (this.match("op","=")) this.parseExpression();
    this.expect("punc",";","var_decl: expected ';'");
  }

  private parseArrElem() {
    if (this.at("ident")) { this.i++; return; }
    this.parseLiteral();
  }

  private parseAsmArg() {
    if (this.at("ident")) { this.i++; return; }
    this.parseLiteral();
  }

  private parseType() {
    // ptr type
    if (this.match("kw","ptr")) {
      this.parseType();
      return;
    }

    // arr type: arr '[' int ',' type ']'
    if (this.match("kw","arr")) {
      if (this.match("punc","[")) {
        this.expect("int", undefined, "arr type: expected integer literal");
        this.expect("punc",",","arr type: expected ','");
        this.parseType();
        this.expect("punc","]","arr type: expected ']'");
        return;
      }
      
      const c = this.prev();
      this.issues.push({ message: "type 'arr' must be followed by '['", range: rangeOf(this.lines, c.start, c.end) });
      return;
    }

    // primitive
    if (this.at("kw") && TYPE_KW.has(this.cur().text) && this.cur().text !== "arr" && this.cur().text !== "ptr") {
      this.i++;
      return;
    }

    this.expect("kw", undefined, "Expected a type keyword");
  }

  private parseLiteral() {
    if (this.match("int")) return;
    if (this.match("str")) return;

    const c = this.cur();
    this.issues.push({ message: `Expected literal, got '${c.text}'`, range: rangeOf(this.lines, c.start, c.end) });
    if (!this.at("eof")) this.i++;
  }

  private parseExpression() { this.parseAssign(); }

  private parseAssign() {
    this.parseLogicalOr();
    if (this.at("op") && ["=","+=","-=","*=","/=","%=","|=","^=","&=","||=","&&="].includes(this.cur().text)) {
      this.i++;
      this.parseAssign(); // right associative
    }
  }

  private parseLogicalOr() {
    this.parseLogicalAnd();
    while (this.match("op","||")) this.parseLogicalAnd();
  }
  private parseLogicalAnd() {
    this.parseBitOr();
    while (this.match("op","&&")) this.parseBitOr();
  }
  private parseBitOr() {
    this.parseBitXor();
    while (this.match("op","|")) this.parseBitXor();
  }
  private parseBitXor() {
    this.parseBitAnd();
    while (this.match("op","^")) this.parseBitAnd();
  }
  private parseBitAnd() {
    this.parseEquality();
    while (this.match("op","&")) this.parseEquality();
  }
  private parseEquality() {
    this.parseRelational();
    while (this.at("op") && (this.cur().text === "==" || this.cur().text === "!=")) { this.i++; this.parseRelational(); }
  }
  private parseRelational() {
    this.parseShift();
    while (this.at("op") && ["<","<=",">",">="].includes(this.cur().text)) { this.i++; this.parseShift(); }
  }
  private parseShift() {
    this.parseAdd();
    while (this.at("op") && (this.cur().text === "<<" || this.cur().text === ">>")) { this.i++; this.parseAdd(); }
  }
  private parseAdd() {
    this.parseMul();
    while (this.at("op") && (this.cur().text === "+" || this.cur().text === "-")) { this.i++; this.parseMul(); }
  }
  private parseMul() {
    this.parseUnary();
    while (this.at("op") && (this.cur().text === "*" || this.cur().text === "/" || this.cur().text === "%")) { this.i++; this.parseUnary(); }
  }

  private parseUnary() {
    if (this.at("kw") && ["not","ref","dref"].includes(this.cur().text)) { this.i++; this.parseUnary(); return; }
    if (this.at("op") && (this.cur().text === "+" || this.cur().text === "-")) { this.i++; this.parseUnary(); return; }
    this.parsePostfix();
  }

  private parsePostfix() {
    this.parsePrimary();

    // calls / indexing / cast
    while (true) {
      if (this.match("punc","(")) {
        if (!this.at("punc",")")) {
          this.parseExpression();
          while (this.match("punc",",")) this.parseExpression();
        }
        this.expect("punc",")","call: expected ')'");
        continue;
      }

      if (this.match("punc","[")) {
        this.parseExpression();
        while (this.match("punc",",")) this.parseExpression();
        this.expect("punc","]","index: expected ']'");
        continue;
      }

      // postfix cast: expr as type
      if (this.match("kw","as")) {
        this.parseType();
        continue;
      }

      break;
    }
  }

  private parsePrimary() {
    if (this.at("kw", "syscall")) { 
      this.i++; 
      return; 
    }
  
    if (this.at("ident")) { this.i++; return; }
    if (this.at("int") || this.at("str")) { this.i++; return; }
    if (this.match("punc","(")) { this.parseExpression(); this.expect("punc",")","expected ')'"); return; }
  
    const c = this.cur();
    this.issues.push({
      message: `Expected primary expression, got '${c.text}'`,
      range: rangeOf(this.lines, c.start, c.end)
    });
    if (!this.at("eof")) this.i++;
  }
  
  private parseParamListOpt() {
    if (this.at("punc",")")) return;
    // param = type ident [= literal]
    this.parseType();
    this.expect("ident", undefined, "param: expected identifier");
    if (this.match("op","=")) this.parseExpression();
    while (this.match("punc",",")) {
      this.parseType();
      this.expect("ident", undefined, "param: expected identifier");
      if (this.match("op","=")) this.parseExpression();
    }
  }
}

export function parseAndDiagnose(text: string): ParseIssue[] {
  return new Parser(text).run();
}
