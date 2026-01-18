import { Range, Position } from "vscode-languageserver/node";
import { SemanticContext, MacroValue, TypeNode } from "./cplSemantics";

function buildLineStarts(text: string): number[] {
  const starts = [0];
  for (let i = 0; i < text.length; i++) {
    if (text.charCodeAt(i) === 10) starts.push(i + 1);
  }
  return starts;
}

function positionAt(lineStarts: number[], offset: number): Position {
  let lo = 0,
    hi = lineStarts.length - 1;
  while (lo <= hi) {
    const mid = (lo + hi) >> 1;
    const s = lineStarts[mid];
    const next = mid + 1 < lineStarts.length ? lineStarts[mid + 1] : Number.POSITIVE_INFINITY;

    if (offset < s) hi = mid - 1;
    else if (offset >= next) lo = mid + 1;
    else return Position.create(mid, offset - s);
  }
  return Position.create(0, 0);
}

function unescapeCStyle(s: string): string {
  return s
    .replace(/\\n/g, "\n")
    .replace(/\\t/g, "\t")
    .replace(/\\r/g, "\r")
    .replace(/\\0/g, "\0")
    .replace(/\\"/g, "\"")
    .replace(/\\\\/g, "\\");
}

function parseMacroValue(raw: string): MacroValue {
  const t = raw.trim();

  if (t.length >= 2 && t.startsWith('"') && t.endsWith('"')) {
    return { kind: "string", value: unescapeCStyle(t.slice(1, -1)) };
  }
  if (/^-?0x[0-9a-fA-F]+$/.test(t)) return { kind: "number", value: Number.parseInt(t, 16) };
  if (/^-?\d+$/.test(t)) return { kind: "number", value: Number.parseInt(t, 10) };

  return { kind: "raw", text: t };
}

function collectDefines(text: string, sem: SemanticContext) {
  const lineStarts = buildLineStarts(text);

  const re = /^[ \t]*#define[ \t]+([A-Za-z_]\w*)[ \t]+(.+?)[ \t]*$/gm;

  let m: RegExpExecArray | null;
  while ((m = re.exec(text))) {
    const fullStart = m.index;
    const full = m[0];
    const name = m[1];
    const valueRaw = m[2];

    const nameRel = full.indexOf(name);
    const valueRel = full.lastIndexOf(valueRaw);

    const nameStart = fullStart + nameRel;
    const nameEnd = nameStart + name.length;

    const valueStart = fullStart + valueRel;
    const valueEnd = valueStart + valueRaw.length;

    const nameRange = Range.create(
      positionAt(lineStarts, nameStart),
      positionAt(lineStarts, nameEnd)
    );
    const valueRange = Range.create(
      positionAt(lineStarts, valueStart),
      positionAt(lineStarts, valueEnd)
    );

    sem.defineMacro(name, parseMacroValue(valueRaw), nameRange, valueRange);
  }
}

type TokenKind =
  | "eof"
  | "eol"
  | "ident"
  | "int"
  | "str"
  | "char"
  | "comment"
  | "kw"
  | "op"
  | "punc";

type Token = {
  kind: TokenKind;
  text: string;
  start: number;
  end: number;
};

type StorageMods = {
  isGlobal: boolean;
  isReadonly: boolean;
};

type ParamInfo = {
  name: string;
  type: TypeNode;
  hasDefault: boolean;
  isVarArgs?: boolean;
  range: Range;
};

type ParseIssue = { message: string; range: Range };

const KEYWORDS = new Set([
  // top-level / statements
  "start","exit","exfunc","function","return",
  "if","else","while","loop","switch","case","default",
  "glob","ro","dref","ref","ptr","lis","break","extern","from","import","syscall","asm","as",
  "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","not",
  // preprocessor
  "line","include","define","undef","ifdef","ifndef","endif"
]);

const TYPE_KW = new Set([
  "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","ptr"
]);

const OPERATORS = [
  "||=","&&=","<<",">>","==","!=","<=",">=","&&","||",
  "+=","-=","*=","/=","%=","|=","^=","&=","=",
  "+","-","*","/","%","|","^","&","<",">", "=>", "..."
].sort((a,b)=>b.length-a.length);

const PUNC = new Set(["{","}","(",")","[","]",",",";","#"]);

function isAlpha(ch: string) { return /[A-Za-z]/.test(ch); }
function isDigit(ch: string) { return /[0-9]/.test(ch); }
function isAlnum_(ch: string){ return /[A-Za-z0-9_]/.test(ch); }

function unwrapDocComment(tokText: string): string {
  let s = tokText;
  if (s.startsWith(":")) s = s.slice(1);
  if (s.endsWith(":")) s = s.slice(0, -1);
  s = s.replace(/\r\n/g, "\n");

  const lines = s.split("\n");
  while (lines.length && lines[0].trim() === "") lines.shift();
  while (lines.length && lines[lines.length - 1].trim() === "") lines.pop();

  const indents = lines
    .filter(l => l.trim() !== "")
    .map(l => (l.match(/^\s*/)?.[0].length ?? 0));
  const min = indents.length ? Math.min(...indents) : 0;

  return lines.map(l => l.slice(min)).join("\n").trimEnd();
}

function unquote(s: string): string {
  if (s.length >= 2 && s[0] === '"' && s[s.length - 1] === '"') s = s.slice(1, -1);
  return s.replace(/\\(["\\ntr0])/g, (_, c) => {
    switch (c) {
      case '"': return '"';
      case "\\": return "\\";
      case "n": return "\n";
      case "t": return "\t";
      case "r": return "\r";
      case "0": return "\0";
      default: return c;
    }
  });
}

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

function parseIntLiteral(s: string): number {
  if (s.startsWith("0x") || s.startsWith("0X")) return parseInt(s.slice(2), 16);
  if (s.startsWith("0b") || s.startsWith("0B")) return parseInt(s.slice(2), 2);
  return parseInt(s, 10);
}

function lex(text: string): Token[] {
  const tokens: Token[] = [];
  let i = 0;

  const push = (kind: TokenKind, start: number, end: number) => {
    tokens.push({ kind, text: text.slice(start, end), start, end });
  };

  while (i < text.length) {
    const ch = text[i];

    if (ch === " " || ch === "\t") { i++; continue; }
    if (ch === "\n") {
      push("eol", i, i + 1);
      i++;
      continue;
    }
    if (ch === "\r") {
      const start = i;
      if (i + 1 < text.length && text[i + 1] === "\n") i += 2;
      else i += 1;
      push("eol", start, i);
      continue;
    }

    if (ch === ":") {
      const start = i;
      i++;
    
      while (i < text.length && text[i] !== ":") i++;
      if (i < text.length && text[i] === ":") {
        i++;
        push("comment", start, i);
        continue;
      }
    
      push("punc", start, start + 1);
      i = start + 1;
      continue;
    }    

    if (ch === "\"") {
      const start = i;
      i++;
      while (i < text.length) {
        if (text[i] === "\n" || text[i] === "\r") break;
        if (text[i] === "\\" && i + 1 < text.length) { i += 2; continue; }
        if (text[i] === "\"") { i++; break; }
        i++;
      }
      push("str", start, i);
      continue;
    }

    // char literal
    if (ch === "'") {
      const start = i;
      i++;
      while (i < text.length) {
        if (text[i] === "\n" || text[i] === "\r") break;
        if (text[i] === "\\" && i + 1 < text.length) { i += 2; continue; }
        if (text[i] === "'") { i++; break; }
        i++;
      }
      push("char", start, i);
      continue;
    }

    // integer literal
    if (isDigit(ch)) {
      const start = i;
      if (text.startsWith("0x", i) || text.startsWith("0X", i)) {
        i += 2;
        while (i < text.length && /[0-9a-fA-F]/.test(text[i])) i++;
      }
      else if (text.startsWith("0b", i) || text.startsWith("0B", i)) {
        i += 2;
        while (i < text.length && /[01]/.test(text[i])) i++;
      }
      else {
        while (i < text.length && isDigit(text[i])) i++;
      }
      push("int", start, i);
      continue;
    }

    // identifiers / keywords
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
    if (PUNC.has(ch)) {
      push("punc", i, i + 1);
      i++;
      continue;
    }

    // fallback: unknown char as punctuation
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
  private sem?: SemanticContext;
  private pendingDoc: string | undefined;

  constructor(
    text: string,
    sem?: SemanticContext,
    private include?: IncludeResolver,
    private includeSeen: Set<string> = new Set(),
    public filePath?: string
  ) {
    this.lines = buildLineIndex(text);
    this.t = lex(text);
    this.sem = sem;
    this.filePath = filePath;
  }

  run(): ParseIssue[] {
    this.parseProgram();
    return this.issues;
  }

  // ---- trivia handling ----
  private skipEOL() {
    while (this.t[this.i]?.kind === "eol") this.i++;
  }

  // default view (EOL skipped)
  private cur(): Token { this.skipEOL(); return this.t[this.i]; }
  private prev(): Token { return this.t[Math.max(0, this.i - 1)]; }

  // raw view (EOL NOT skipped) — needed for pp_directive terminators
  private curRaw(): Token { return this.t[this.i]; }

  private at(kind: TokenKind, text?: string): boolean {
    const c = this.cur();
    if (c.kind !== kind) return false;
    if (text !== undefined) return c.text === text;
    return true;
  }

  private atRaw(kind: TokenKind, text?: string): boolean {
    const c = this.curRaw();
    if (c.kind !== kind) return false;
    if (text !== undefined) return c.text === text;
    return true;
  }

  private match(kind: TokenKind, text?: string): boolean {
    if (!this.at(kind, text)) return false;
    this.i++;
    return true;
  }

  private matchRaw(kind: TokenKind, text?: string): boolean {
    if (!this.atRaw(kind, text)) return false;
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
    if (!this.at("eof")) this.i++;
    return false;
  }

  private syncToStatementEnd() {
    while (
      !this.atRaw("eof") &&
      !this.atRaw("punc", ";") &&
      !this.atRaw("punc", "}") &&
      !this.atRaw("eol")
    ) this.i++;

    if (this.atRaw("punc", ";") || this.atRaw("eol")) this.i++;
  }

  // ---- program / top level ----

  private parseProgram() {
    this.expect("punc", "{", "Program must start with '{'");
    while (!this.at("eof") && !this.at("punc", "}")) {
      const before = this.i;
      this.parseTopItem();
      if (this.i === before) this.i++;
    }
    this.expect("punc", "}", "Program must end with '}'");
  }

  private parseTopItem() {
    if (this.atRaw("comment")) {
      const tok = this.curRaw();
      this.i++;
    
      const doc = unwrapDocComment(tok.text);
      if (doc) this.pendingDoc = this.pendingDoc ? (this.pendingDoc + "\n" + doc) : doc;
      return;
    }

    if (this.atRaw("punc", "#")) {
      this.parsePPDirective();
      return;
    }

    if (this.match("kw", "start")) {
      this.expect("punc", "(");
      const paramsInfo = this.parseParamListOptInfos();
      this.expect("punc", ")");

      this.sem?.enterScope();
      for (const p of paramsInfo) this.sem?.declareLocalVar(p.name, p.type, p.range);

      this.parseBlock();

      this.sem?.exitScope();
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
      if (this.match("kw", "function")) {
        const nameTok = this.cur();
        this.expect("ident", undefined, "extern function: expected identifier");
        const fnName = this.prev().text;
    
        this.expect("punc", "(");
        const params = this.parseParamListOptInfos();
        this.expect("punc", ")");
    
        let ret: TypeNode = { kind: "prim", name: "i0" };
        if (this.match("op", "=>")) {
          ret = this.parseType();
        }
    
        this.expect("punc", ";", "extern function: expected ';'");
    
        const fnRange = rangeOf(this.lines, nameTok.start, this.prev().end);
        this.sem?.declareFunc(fnName, params, ret, fnRange, false, this.pendingDoc);
        this.pendingDoc = undefined;
        return;
      }
    
      // old extern exfunc / extern var
      if (this.match("kw", "exfunc")) {
        this.expect("ident", undefined, "extern exfunc: expected identifier");
      } else {
        this.parseType();
        this.expect("ident", undefined, "extern var: expected identifier");
      }
    
      this.match("punc", ";");
      return;
    }    

    const mods = this.parseStorageMods();

    // top_decl = var_decl | function_def | function_proto
    if (this.match("kw", "function")) {
      const doc = this.pendingDoc;
      this.pendingDoc = undefined;

      const nameTok = this.cur();
      this.expect("ident", undefined, "function: expected identifier");
      const fnName = this.prev().text;

      this.expect("punc", "(");
      const paramsInfo = this.parseParamListOptInfos();
      this.expect("punc", ")");

      let ret: TypeNode = { kind: "prim", name: "i0" };
      if (this.match("op", "=>")) ret = this.parseType();

      const fnRange = rangeOf(this.lines, nameTok.start, this.prev().end);

      if (this.match("punc", ";")) {
        this.sem?.declareFunc(fnName, paramsInfo, ret, fnRange, false, doc);
        return;
      }

      this.sem?.declareFunc(fnName, paramsInfo, ret, fnRange, true, doc);

      this.sem?.enterScope();
      for (const p of paramsInfo) this.sem?.declareLocalVar(p.name, p.type, p.range);

      this.parseBlock();

      this.sem?.exitScope();
      return;
    }

    // var_decl / arr_decl at top level
    this.parseVarOrArrDecl(true);
  }

  // ---- pp_directive ----
  // pp_directive = "#" , pp_body , (EOL | ";")
  private parsePPDirective() {
    const hashTok = this.curRaw();
    if (!this.matchRaw("punc", "#")) {
      this.issues.push({
        message: "pp_directive: expected '#'",
        range: rangeOf(this.lines, hashTok.start, hashTok.end)
      });
      return;
    }

    // directive name should be a keyword token in our lexer
    const name = this.cur(); // (EOL skipped, OK)
    const isKw = name.kind === "kw";
    const dir = isKw ? name.text : "";

    if (!isKw) {
      this.issues.push({
        message: "pp_directive: expected directive name",
        range: rangeOf(this.lines, name.start, name.end)
      });
      this.consumePPLineEnd();
      return;
    }

    // consume name
    this.i++;

    switch (dir) {
      case "line":
        this.expect("int", undefined, "#line: expected integer literal");
        if (this.at("str")) this.i++;
        this.consumePPLineEnd();
        return;

      case "include": {
        this.expect("str", undefined, "#include: expected string literal");
        const lit = this.prev().text;
        const incPath = unquote(lit);
      
        this.consumePPLineEnd();
      
        if (this.include) {
          const res = this.include(incPath, this.filePath);
          if (!res) {
            this.issues.push({
              message: `#include not found: ${incPath}`,
              range: rangeOf(this.lines, name.start, name.end)
            });
          
            return;
          }    

          if (this.includeSeen.has(res.filePath)) return;
          this.includeSeen.add(res.filePath);
          const p2 = new Parser(res.text, this.sem, this.include, this.includeSeen, res.filePath);
          p2.run();
        }

        return;
      }              

      case "undef":
        this.expect("ident", undefined, "#undef: expected identifier");
        this.consumePPLineEnd();
        return;

      case "ifdef":
        this.expect("ident", undefined, "#ifdef: expected identifier");
        this.consumePPLineEnd();
        return;

      case "ifndef":
        this.expect("ident", undefined, "#ifndef: expected identifier");
        this.consumePPLineEnd();
        return;

      case "endif":
        this.consumePPLineEnd();
        return;

      case "define":
        this.expect("ident", undefined, "#define: expected identifier");
        // replacement-list: consume tokens until ';' or EOL (raw)
        while (!this.atRaw("eof") && !this.atRaw("eol") && !this.atRaw("punc", ";")) {
          this.i++;
        }
        this.consumePPLineEnd();
        return;

      default:
        this.issues.push({
          message: `Unknown preprocessor directive '${dir}'`,
          range: rangeOf(this.lines, name.start, name.end)
        });
        // consume rest of line
        while (!this.atRaw("eof") && !this.atRaw("eol") && !this.atRaw("punc", ";")) this.i++;
        this.consumePPLineEnd();
        return;
    }
  }

  private consumePPLineEnd() {
    if (this.matchRaw("punc", ";")) return;
    if (this.matchRaw("eol")) return;
    if (this.atRaw("eof")) return;

    const c = this.curRaw();
    this.issues.push({
      message: "pp_directive: expected end-of-line or ';'",
      range: rangeOf(this.lines, c.start, c.end)
    });

    while (!this.atRaw("eof") && !this.atRaw("eol") && !this.atRaw("punc", ";")) this.i++;
    if (this.atRaw("punc", ";") || this.atRaw("eol")) this.i++;
  }

  private parseParamListOptInfos(): ParamInfo[] {
    const params: ParamInfo[] = [];
    if (this.at("punc", ")")) return params;
  
    let seenVarArgs = false;
    const unknownType: TypeNode = { kind: "unknown" };
  
    while (true) {
      let isVarArgs = false;
  
      if (this.match("op", "...")) {
        if (seenVarArgs) {
          const c = this.prev();
          this.issues.push({
            message: "multiple varargs parameters are not allowed",
            range: rangeOf(this.lines, c.start, c.end)
          });
        }
        seenVarArgs = true;
        isVarArgs = true;
  
        // optional type
        let t: TypeNode = unknownType;
        if (this.at("kw") && TYPE_KW.has(this.cur().text)) {
          t = this.parseType();
        }
  
        // optional name
        let name = "__varargs";
        let nameTok = this.prev();
        if (this.at("ident")) {
          nameTok = this.cur();
          this.i++;
          name = nameTok.text;
        }
  
        params.push({
          name,
          type: t,
          hasDefault: false,
          isVarArgs: true,
          range: rangeOf(this.lines, nameTok.start, nameTok.end)
        });
  
        // varargs MUST be last
        if (this.match("punc", ",")) {
          this.issues.push({
            message: "varargs parameter must be the last one",
            range: rangeOf(this.lines, nameTok.start, nameTok.end)
          });
        }
        break;
      }
  
      // ---- normal parameter ----
      const t = this.parseType();
  
      const nameTok = this.cur();
      this.expect("ident", undefined, "param: expected identifier");
      const name = this.prev().text;
  
      let hasDefault = false;
      if (this.match("op", "=")) {
        hasDefault = true;
        this.parseExpression();
      }
  
      params.push({
        name,
        type: t,
        hasDefault,
        range: rangeOf(this.lines, nameTok.start, nameTok.end)
      });
  
      if (!this.match("punc", ",")) break;
    }
  
    return params;
  }  

  private parseBlock() {
    this.expect("punc", "{", "block: expected '{'");
    this.sem?.enterScope();
    while (!this.at("eof") && !this.at("punc", "}")) {
      const before = this.i;
      this.parseStatement();
      if (this.i === before) this.i++;
    }
    this.sem?.exitScope();
    this.expect("punc", "}", "block: expected '}'");
  }
  
  private parseStorageMods(): StorageMods {
    let isGlobal = false;
    let isReadonly = false;
  
    let progressed = true;
    while (progressed) {
      progressed = false;
      if (this.match("kw", "glob")) {
        isGlobal = true;
        progressed = true;
      }
      if (this.match("kw", "ro")) {
        isReadonly = true;
        progressed = true;
      }
    }
  
    return { isGlobal, isReadonly };
  }

  private parseStatement() {
    // pp_directive
    if (this.atRaw("punc", "#")) { this.parsePPDirective(); return; }

    // comment statement
    if (this.match("comment")) return;

    if (this.at("kw", "if")) {
      this.i++;
      this.parseExpression();
      this.expect("punc", ";");
      this.parseBlock();
      if (this.match("kw", "else")) this.parseBlock();
      return;
    }

    if (this.at("kw", "while")) {
      this.i++;
      this.parseExpression();
      this.expect("punc", ";");
      this.parseBlock();
      return;
    }

    if (this.at("kw", "loop")) {
      this.i++;
      this.parseBlock();
      return;
    }

    // switch expression; { case literal; block ... [default [;] block] }
    if (this.at("kw", "switch")) {
      this.i++;
      this.parseExpression();
      this.expect("punc", ";", "switch: expected ';' after expression");
      this.expect("punc", "{", "switch: expected '{'");

      while (this.match("kw", "case")) {
        this.parseLiteral();
        this.expect("punc", ";", "case: expected ';'");
        this.parseBlock();
      }

      if (this.match("kw", "default")) {
        this.match("punc", ";"); // optional
        this.parseBlock();
      }

      this.expect("punc", "}", "switch: expected '}'");
      return;
    }

    if (this.at("kw", "return")) {
      this.i++;
      if (!this.at("punc", ";")) this.parseExpression();
      this.expect("punc", ";");
      return;
    }

    if (this.at("kw", "exit")) {
      this.i++;
      this.parseExpression();
      this.expect("punc", ";");
      return;
    }

    if (this.at("kw", "break")) {
      this.i++;
      this.expect("punc", ";");
      return;
    }

    // lis [string_literal] ;
    if (this.at("kw", "lis")) {
      this.i++;
      if (!this.at("punc", ";")) this.expect("str", undefined, "lis: expected string literal");
      this.expect("punc", ";");
      return;
    }

    if (this.at("kw", "syscall")) {
      this.i++;
      this.expect("punc", "(");
      if (!this.at("punc", ")")) {
        this.parseExpression();
        while (this.match("punc", ",")) this.parseExpression();
      }
      this.expect("punc", ")");
      this.expect("punc", ";");
      return;
    }

    if (this.at("kw", "asm")) {
      this.i++;
      this.expect("punc", "(");
      if (!this.at("punc", ")")) {
        this.parseAsmArg();
        while (this.match("punc", ",")) this.parseAsmArg();
      }
      this.expect("punc", ")");
      this.expect("punc", "{");
      while (!this.at("eof") && !this.at("punc", "}")) {
        this.expect("str", undefined, "asm_line: expected string literal");
        this.match("punc", ",");
      }
      this.expect("punc", "}");
      return;
    }

    if (this.looksLikeDeclStart()) {
      this.parseVarOrArrDecl(false);
      return;
    }

    try {
      this.parseExpression();
      this.expect("punc", ";");
    } catch {
      this.syncToStatementEnd();
    }
  }

  private looksLikeDeclStart(): boolean {
    const c = this.cur();
    return (
      (c.kind === "kw" && TYPE_KW.has(c.text)) ||
      this.at("kw", "glob") ||
      this.at("kw", "ro")
    );
  }  

  private parseVarOrArrDecl(isTopLevel: boolean) {
    const mods = this.parseStorageMods();
    // arr_decl: "arr" ident "[" integer_literal "," type "]" [ "=" (expression | arr_value) ] ";"
    if (this.at("kw", "arr")) {
      const t2 = this.t[this.i + 1];
      const t3 = this.t[this.i + 2];
      if (t2?.kind === "ident" && t3?.kind === "punc" && t3.text === "[") {
        this.i++; // consume 'arr'

        const nameTok = this.cur();
        this.expect("ident", undefined, "arr_decl: expected identifier");
        const arrName = this.prev().text;

        this.expect("punc", "[", "arr_decl: expected '['");

        const lenTok = this.cur();
        this.expect("int", undefined, "arr_decl: expected integer literal length");
        const len = parseIntLiteral(lenTok.text);

        this.expect("punc", ",", "arr_decl: expected ',' between size and type");
        const elemType = this.parseType();

        const arrType: TypeNode = { kind: "arr", len: Number.isFinite(len) ? len : null, elem: elemType };
        const declRange = rangeOf(this.lines, nameTok.start, nameTok.end);
        if (isTopLevel) this.sem?.declareGlobalVar(arrName, arrType, declRange);
        else this.sem?.declareLocalVar(arrName, arrType, declRange);

        this.expect("punc", "]", "arr_decl: expected ']'");

        if (this.match("op", "=")) {
          if (this.match("punc", "{")) {
            if (!this.at("punc", "}")) {
              this.parseExpression();
              while (this.match("punc", ",")) this.parseExpression();
            }
            this.expect("punc", "}", "arr_value: expected '}'");
          } else {
            this.parseExpression();
          }
        }

        this.expect("punc", ";", "arr_decl: expected ';'");
        return;
      }
    }

    // var_decl: type ident [ "=" expression ] ";"
    const vType = this.parseType();

    const nameTok = this.cur();
    this.expect("ident", undefined, "var_decl: expected identifier");
    const vName = this.prev().text;

    const declRange = rangeOf(this.lines, nameTok.start, nameTok.end);
    if (isTopLevel) this.sem?.declareGlobalVar(vName, vType, declRange);
    else this.sem?.declareLocalVar(vName, vType, declRange);

    if (this.match("op", "=")) this.parseExpression();
    this.expect("punc", ";", "var_decl: expected ';'");
  }

  private parseAsmArg() {
    if (this.at("ident")) { this.i++; return; }
    this.parseLiteral();
  }

  private parseType(): TypeNode {
    if (this.match("kw", "ptr")) {
      const to = this.parseType();
      return { kind: "ptr", to };
    }

    if (this.match("kw", "arr")) {
      if (this.match("punc", "[")) {
        const lenTok = this.cur();
        this.expect("int", undefined, "arr type: expected integer literal");
        const len = parseIntLiteral(lenTok.text);

        this.expect("punc", ",", "arr type: expected ','");
        const elem = this.parseType();

        this.expect("punc", "]", "arr type: expected ']'");
        return { kind: "arr", len: Number.isFinite(len) ? len : null, elem };
      }

      const c = this.prev();
      this.issues.push({ message: "type 'arr' must be followed by '['", range: rangeOf(this.lines, c.start, c.end) });
      return { kind: "unknown" };
    }

    if (this.at("kw") && TYPE_KW.has(this.cur().text) && this.cur().text !== "arr" && this.cur().text !== "ptr") {
      const name = this.cur().text;
      this.i++;
      return { kind: "prim", name };
    }

    this.expect("kw", undefined, "Expected a type keyword");
    return { kind: "unknown" };
  }

  private parseLiteral() {
    if (this.match("int")) return;
    if (this.match("str")) return;
    if (this.match("char")) return;

    const c = this.cur();
    this.issues.push({ message: `Expected literal, got '${c.text}'`, range: rangeOf(this.lines, c.start, c.end) });
    if (!this.at("eof")) this.i++;
  }

  // ---- expressions ----

  private parseExpression() { this.parseAssign(); }

  private parseAssign() {
    this.parseLogicalOr();
    if (this.at("op") && ["=","+=","-=","*=","/=","%=","|=","^=","&=","||=","&&="].includes(this.cur().text)) {
      this.i++;
      this.parseAssign();
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
    let atomIdent: { name: string; start: number; end: number } | null = null;

    if (this.at("ident")) {
      const tok = this.cur();
      atomIdent = { name: tok.text, start: tok.start, end: tok.end };
      this.i++;
    }
    else if (this.at("kw","syscall")) {
      const tok = this.cur();
      atomIdent = { name: "syscall", start: tok.start, end: tok.end };
      this.i++;
    }
    else {
      this.parsePrimary();
    }

    let wasCall = false;
    while (true) {
      if (this.match("punc","(")) {
        wasCall = true;
        let argc = 0;
        if (!this.at("punc",")")) {
          this.parseExpression(); argc++;
          while (this.match("punc",",")) { this.parseExpression(); argc++; }
        }
        const endTok = this.cur();
        this.expect("punc",")");
        if (atomIdent) {
          const callRange = rangeOf(this.lines, atomIdent.start, endTok.end);
          this.sem?.noteCallSite(atomIdent.name, callRange);
          this.sem?.callFunc(atomIdent.name, argc, callRange);
        }
        continue;
      }

      if (this.match("punc","[")) {
        this.parseExpression();
        while (this.match("punc",",")) this.parseExpression();
        this.expect("punc","]");
        continue;
      }

      if (this.match("kw","as")) { this.parseType(); continue; }
      break;
    }

    if (atomIdent && !wasCall && atomIdent.name !== "syscall") {
      this.sem?.useVar(atomIdent.name, rangeOf(this.lines, atomIdent.start, atomIdent.end));
    }
  }

  private parsePrimary() {
    if (this.at("kw", "syscall")) {
      this.i++;
      return;
    }

    if (this.at("ident")) { this.i++; return; }
    if (this.at("int") || this.at("str") || this.at("char")) { this.i++; return; }
    if (this.match("punc","(")) { this.parseExpression(); this.expect("punc",")","expected ')'"); return; }

    const c = this.cur();
    this.issues.push({
      message: `Expected primary expression, got '${c.text}'`,
      range: rangeOf(this.lines, c.start, c.end)
    });
    if (!this.at("eof")) this.i++;
  }
}

export type IncludeResolverResult = { text: string; filePath: string };
export type IncludeResolver = (includePath: string, fromFilePath?: string) => IncludeResolverResult | undefined;

export function parseAndDiagnose(text: string): ParseIssue[] {
  const sem = new SemanticContext();
  const p = new Parser(text, sem);
  const syntax = p.run();
  sem.finish();
  const semantic = sem.issues;
  return [...syntax, ...semantic];
}

export function analyze(text: string, include?: IncludeResolver, filePath?: string) {
  const sem = new SemanticContext();
  collectDefines(text, sem);
  const p = new Parser(text, sem, include, new Set<string>(), filePath);
  const syntax = p.run();
  sem.finish();
  return { issues: [...syntax, ...sem.issues], sem };
}
