import { Range, Position } from "vscode-languageserver/node";
import { SemanticContext, MacroValue, TypeNode, MacroCondition } from "./cplSemantics";

function buildLineStarts(text: string): number[] {
  const starts = [0];
  for (let i = 0; i < text.length; i++) {
    if (text.charCodeAt(i) === 10) starts.push(i + 1);
  }
  return starts;
}

function positionAt(lineStarts: number[], offset: number): Position {
  let lo = 0, hi = lineStarts.length - 1;
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

function cloneMacroConditions(conditions: MacroCondition[]): MacroCondition[] {
  return conditions.map((c) => ({ name: c.name, isDefined: c.isDefined }));
}

function collectDefines(
  text: string,
  sem: SemanticContext,
  filePath?: string,
  initialConditions: MacroCondition[] = []
) {
  const prevFilePath = sem.setCurrentFilePath(filePath);

  try {
    const lineStarts = buildLineStarts(text);
    const conditionStack = cloneMacroConditions(initialConditions);

    const lines = text.match(/[^\r\n]*(?:\r\n|\n|\r|$)/g) ?? [];
    let offset = 0;

    for (const rawLine of lines) {
      if (rawLine.length === 0 && offset >= text.length) break;

      const line = rawLine.replace(/\r?\n$|\r$/, "");
      const directive = line.match(/^[ \t]*#[ \t]*(ifdef|ifndef|endif|define)\b(.*)$/);

      if (!directive) {
        offset += rawLine.length;
        continue;
      }

      const dir = directive[1];
      const rest = directive[2] ?? "";

      if (dir === "ifdef" || dir === "ifndef") {
        const m = rest.match(/^[ \t]+([A-Za-z_]\w*)\b/);
        if (m) conditionStack.push({ name: m[1], isDefined: dir === "ifdef" });
        offset += rawLine.length;
        continue;
      }

      if (dir === "endif") {
        if (conditionStack.length > initialConditions.length) conditionStack.pop();
        offset += rawLine.length;
        continue;
      }

      const defineMatch = line.match(/^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)(?:[ \t]+(.*?))?[ \t]*$/);
      if (!defineMatch) {
        offset += rawLine.length;
        continue;
      }

      const name = defineMatch[1];
      const valueRaw = defineMatch[2] ?? "";
      const defineKeywordAt = line.indexOf("define");
      const nameRel = line.indexOf(name, defineKeywordAt + "define".length);
      const valueRel = valueRaw ? line.lastIndexOf(valueRaw) : line.length;

      const nameStart = offset + nameRel;
      const nameEnd = nameStart + name.length;
      const valueStart = offset + valueRel;
      const valueEnd = valueStart + valueRaw.length;

      const nameRange = Range.create(
        positionAt(lineStarts, nameStart),
        positionAt(lineStarts, nameEnd)
      );
      const valueRange = Range.create(
        positionAt(lineStarts, valueStart),
        positionAt(lineStarts, valueEnd)
      );

      sem.defineMacro(name, parseMacroValue(valueRaw), nameRange, valueRange, {
        conditions: cloneMacroConditions(conditionStack)
      });

      offset += rawLine.length;
    }
  } finally {
    sem.setCurrentFilePath(prevFilePath);
  }
}

type TokenKind =
  | "eof"
  | "eol"
  | "ident"
  | "int"
  | "float"
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

type ExprInfo = {
  type: TypeNode;
  identName?: string;
  isSyscall?: boolean;
  isTypeName?: boolean;
  associatedContainerName?: string;
  associatedMethodName?: string;
  start?: number;
  end?: number;
};

type ParseIssue = { message: string; range: Range };

const KEYWORDS = new Set([
  // top-level / statements
  "start","exit","function","container","return",
  "if","else","while","loop","switch","case","default",
  "glob","ro","dref","ref","ptr","lis","break","extern","from","import","syscall","asm","as",
  "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","not","neg","poparg","sizeof",
  "section","align",
  // preprocessor
  "line","include","define","undef","ifdef","ifndef","endif"
]);

const TYPE_KW = new Set([
  "f64","f32","i64","i32","i16","i8","u64","u32","u16","u8","i0","str","arr","ptr"
]);

const OPERATORS = [
  "::",
  "||=","&&=","<<",">>","==","!=","<=",">=","&&","||",
  "+=","-=","*=","/=","%=","|=","^=","&=","=",
  "+","-","*","/","%","|","^","&","<",">", "->", "=>", "..."
].sort((a,b) => b.length-a.length);

const PUNC = new Set(["{","}","(",")","[","]",",",";","#","@","."]);

function isAlpha(ch: string) { return /[A-Za-z]/.test(ch); }
function isDigit(ch: string) { return /[0-9]/.test(ch); }
function isAlnum_(ch: string){ return /[A-Za-z0-9_]/.test(ch); }

function unwrapDocComment(tokText: string): string {
  let s = tokText;
  if (s.startsWith(":/") && s.endsWith("/:")) s = s.slice(2, -2);
  else {
    if (s.startsWith(":")) s = s.slice(1);
    if (s.endsWith(":")) s = s.slice(0, -1);
  }
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

      if (i + 1 < text.length && text[i + 1] === ":") {
        push("op", start, start + 2);
        i += 2;
        continue;
      }

      if (i + 1 < text.length && text[i + 1] === "/") {
        i += 2;
        while (i + 1 < text.length && !(text[i] === "/" && text[i + 1] === ":")) i++;
        if (i + 1 < text.length) {
          i += 2;
          push("comment", start, i);
          continue;
        }

        push("punc", start, start + 1);
        i = start + 1;
        continue;
      }

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
        if (text[i] === "\\" && i + 1 < text.length) { i += 2; continue; }
        if (text[i] === "\"") { i++; break; }
        i++;
      }
      push("str", start, i);
      continue;
    }

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

    if (isDigit(ch)) {
      const start = i;
      let kind: TokenKind = "int";

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

        if (i < text.length && text[i] === "." && i + 1 < text.length && isDigit(text[i + 1])) {
          kind = "float";
          i++;
          while (i < text.length && isDigit(text[i])) i++;
        }

        if (i < text.length && (text[i] === "e" || text[i] === "E")) {
          const expStart = i;
          let j = i + 1;
          if (j < text.length && (text[j] === "+" || text[j] === "-")) j++;
          if (j < text.length && isDigit(text[j])) {
            kind = "float";
            i = j + 1;
            while (i < text.length && isDigit(text[i])) i++;
          } else {
            i = expStart;
          }
        }
      }

      push(kind, start, i);
      continue;
    }

    if (isAlpha(ch) || ch === "_") {
      const start = i;
      i++;
      while (i < text.length && isAlnum_(text[i])) i++;
      const w = text.slice(start, i);
      push(KEYWORDS.has(w) ? "kw" : "ident", start, i);
      continue;
    }

    let matchedOp = "";
    for (const op of OPERATORS) {
      if (text.startsWith(op, i)) { matchedOp = op; break; }
    }
    if (matchedOp) {
      push("op", i, i + matchedOp.length);
      i += matchedOp.length;
      continue;
    }

    if (PUNC.has(ch)) {
      push("punc", i, i + 1);
      i++;
      continue;
    }

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
  private pendingAnnotations: string[] = [];
  private ppConditionStack: MacroCondition[];

  constructor(
    text: string,
    sem?: SemanticContext,
    private include?: IncludeResolver,
    private includeSeen: Set<string> = new Set(),
    public filePath?: string,
    initialPPConditions: MacroCondition[] = []
  ) {
    this.lines = buildLineIndex(text);
    this.t = lex(text);
    this.sem = sem;
    this.filePath = filePath;
    this.ppConditionStack = cloneMacroConditions(initialPPConditions);
  }

  run(): ParseIssue[] {
    const prevFilePath = this.sem?.setCurrentFilePath(this.filePath);
    try {
      this.parseProgram();
      return this.issues;
    } finally {
      this.sem?.setCurrentFilePath(prevFilePath);
    }
  }

  private skipEOL() {
    while (this.t[this.i]?.kind === "eol") this.i++;
  }

  private cur(): Token { this.skipEOL(); return this.t[this.i]; }
  private prev(): Token { return this.t[Math.max(0, this.i - 1)]; }

  private curRaw(): Token { return this.t[this.i]; }

  private prevNonEOL(): Token | undefined {
    for (let j = this.i - 1; j >= 0; j--) {
      if (this.t[j].kind !== "eol") return this.t[j];
    }
    return undefined;
  }

  private currentPPConditions(): MacroCondition[] {
    return cloneMacroConditions(this.ppConditionStack);
  }

  private parseConstArrayLen(msg: string): number | null {
    const tok = this.cur();
  
    if (this.match("int")) {
      return parseIntLiteral(tok.text);
    }
  
    if (this.match("ident")) {
      const macro = this.sem?.macros.get(tok.text);
      const r = rangeOf(this.lines, tok.start, tok.end);
  
      if (macro?.value.kind === "number") {
        this.sem?.useMacro(tok.text, r);
        return macro.value.value;
      }
  
      this.issues.push({
        message: `${msg}: expected integer literal or numeric macro`,
        range: r
      });
  
      return null;
    }
  
    this.issues.push({
      message: `${msg}: expected integer literal or numeric macro`,
      range: rangeOf(this.lines, tok.start, tok.end)
    });
  
    if (!this.at("eof")) this.i++;
    return null;
  }

  private lineOfOffset(off: number): number {
    let lo = 0, hi = this.lines.length - 1;
    while (lo <= hi) {
      const mid = (lo + hi) >> 1;
      if (this.lines[mid] <= off) lo = mid + 1;
      else hi = mid - 1;
    }
    return Math.max(0, hi);
  }

  private peekNonEOL(offset = 0): Token {
    let j = this.i;
    let seen = 0;
    while (j < this.t.length) {
      if (this.t[j].kind === "eol") { j++; continue; }
      if (seen === offset) return this.t[j];
      seen++;
      j++;
    }
    return this.t[this.t.length - 1];
  }

  private atNonEOL(offset: number, kind: TokenKind, text?: string): boolean {
    const c = this.peekNonEOL(offset);
    if (c.kind !== kind) return false;
    if (text !== undefined) return c.text === text;
    return true;
  }

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

  private skipInnerComments() {
    while (true) {
      while (this.t[this.i]?.kind === "eol") this.i++;
      if (this.t[this.i]?.kind !== "comment") break;
      this.i++;
    }
  }

  private clearPendingMetadata() {
    this.pendingAnnotations = [];
  }

  private parseInlineAnnotations() {
    while (this.atRaw("punc", "@")) {
      const ann = this.parseAnnotation();
      if (ann) this.pendingAnnotations.push(ann);
    }
  }

  private parseAnnotation(): string | undefined {
    const atTok = this.curRaw();
    if (!this.matchRaw("punc", "@")) {
      this.issues.push({
        message: "annotation: expected '@'",
        range: rangeOf(this.lines, atTok.start, atTok.end)
      });
      return undefined;
    }

    if (!this.matchRaw("punc", "[")) {
      const c = this.curRaw();
      this.issues.push({
        message: "annotation: expected '[' after '@'",
        range: rangeOf(this.lines, c.start, c.end)
      });
      return undefined;
    }

    let depth = 1;
    const parts: string[] = [];

    while (!this.atRaw("eof") && depth > 0) {
      const tok = this.curRaw();
      this.i++;

      if (tok.kind === "punc" && tok.text === "[") {
        depth++;
        parts.push(tok.text);
        continue;
      }

      if (tok.kind === "punc" && tok.text === "]") {
        depth--;
        if (depth > 0) parts.push(tok.text);
        continue;
      }

      parts.push(tok.text);
    }

    if (depth > 0) {
      this.issues.push({
        message: "annotation: expected ']'",
        range: rangeOf(this.lines, atTok.start, atTok.end)
      });
      return undefined;
    }

    return parts.join("").trim();
  }


  private hasPendingAnnotation(name: string): boolean {
    return this.pendingAnnotations.some((ann) => {
      const t = ann.trim();
      return t === name || t.startsWith(`${name}(`);
    });
  }

  private declareImplicitSelfIfNeeded(enabled: boolean, selfType: TypeNode, range: Range, paramsInfo: ParamInfo[]) {
    if (!enabled) return;
    if (paramsInfo[0]?.name === "self") return;
    this.sem?.declareLocalVar("self", selfType, range);
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

  private consumeStmtEnd(msg: string): boolean {
    if (this.matchRaw("punc", ";")) return true;

    if (this.matchRaw("eol")) {
      while (this.matchRaw("eol")) {}
      return true;
    }

    if (this.atRaw("eof") || this.atRaw("punc", "}")) return true;

    const prevTok = this.prevNonEOL();
    const curTok = this.curRaw();
    if (prevTok && this.lineOfOffset(prevTok.end) < this.lineOfOffset(curTok.start)) return true;

    const c = this.curRaw();
    this.issues.push({
      message: msg,
      range: rangeOf(this.lines, c.start, c.end)
    });
    return false;
  }

  private parseProgram() {
    while (!this.at("eof")) {
      const before = this.i;
      this.parseTopItem();
      if (this.i === before) this.i++;
    }
  }

  private parseTopItem() {
    if (this.atRaw("comment")) {
      const tok = this.curRaw();
      this.i++;

      const doc = unwrapDocComment(tok.text);
      if (doc) this.pendingDoc = this.pendingDoc ? (this.pendingDoc + "\n" + doc) : doc;
      return;
    }

    if (this.atRaw("punc", "@")) {
      const ann = this.parseAnnotation();
      if (ann) this.pendingAnnotations.push(ann);
      return;
    }

    if (this.atRaw("punc", "#")) {
      this.parsePPDirective();
      return;
    }

    if (this.at("punc", "{")) {
      this.parseBlock();
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    if (this.match("kw", "start")) {
      this.parseStartAfterKeyword();
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    if (this.match("kw", "from")) {
      this.expect("str", undefined, "from: expected string literal");
      this.expect("kw", "import", "from: expected 'import'");

      const importedFuncType: TypeNode = { kind: "ptr", to: { kind: "prim", name: "i0" } };
      if (this.at("ident")) {
        while (true) {
          const idTok = this.cur();
          this.expect("ident", undefined, "import: expected identifier");
          const name = this.prev().text;
          const r = rangeOf(this.lines, idTok.start, idTok.end);
          this.sem?.declareGlobalVar(name, importedFuncType, r, { readonly: true });
          if (!this.match("punc", ",")) break;
        }
      }

      this.match("punc", ";");
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    if (this.match("kw", "extern")) {
      this.parseExternOp();
      this.clearPendingMetadata();
      return;
    }

    if (this.at("kw", "section")) {
      this.parseSectionStmt();
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    if (this.at("kw", "align")) {
      this.parseAlignStmt(true);
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    {
      const modsPos = this.i;
      const mods = this.parseStorageMods();
      if (mods.isGlobal || mods.isReadonly) {
        if (this.match("kw", "function")) {
          const doc = this.pendingDoc;
          this.pendingDoc = undefined;
          this.parseFunctionAfterKeyword(doc);
          this.clearPendingMetadata();
          return;
        }
        this.i = modsPos;
      }
    }

    if (this.match("kw", "container")) {
      const doc = this.pendingDoc;
      this.pendingDoc = undefined;
      this.parseContainerAfterKeyword(doc);
      this.clearPendingMetadata();
      return;
    }

    if (this.match("kw", "function")) {
      const doc = this.pendingDoc;
      this.pendingDoc = undefined;
      this.parseFunctionAfterKeyword(doc);
      this.clearPendingMetadata();
      return;
    }

    this.parseVarOrArrDecl(true);
    this.pendingDoc = undefined;
    this.clearPendingMetadata();
  }

  private parseIncludePath(): { path: string; isSystem: boolean; range: Range } | undefined {
    const startTok = this.cur();

    if (this.match("str")) {
      const tok = this.prev();
      return {
        path: unquote(tok.text),
        isSystem: false,
        range: rangeOf(this.lines, tok.start, tok.end)
      };
    }

    if (this.match("op", "<")) {
      const open = this.prev();
      const parts: string[] = [];
      let last = open;

      while (!this.atRaw("eof") && !this.atRaw("eol")) {
        if (this.at("op", ">")) {
          const close = this.cur();
          this.i++;
          return {
            path: parts.join("").trim(),
            isSystem: true,
            range: rangeOf(this.lines, open.start, close.end)
          };
        }

        const tok = this.curRaw();
        if (tok.kind === "punc" && tok.text === ";") break;
        parts.push(tok.text);
        last = tok;
        this.i++;
      }

      this.issues.push({
        message: "#include: expected '>'",
        range: rangeOf(this.lines, open.start, last.end)
      });
      return {
        path: parts.join("").trim(),
        isSystem: true,
        range: rangeOf(this.lines, open.start, last.end)
      };
    }

    this.issues.push({
      message: "#include: expected string literal or <...>",
      range: rangeOf(this.lines, startTok.start, startTok.end)
    });
    return undefined;
  }

  private parsePPDirective() {
    const hashTok = this.curRaw();
    if (!this.matchRaw("punc", "#")) {
      this.issues.push({
        message: "pp_directive: expected '#'",
        range: rangeOf(this.lines, hashTok.start, hashTok.end)
      });
      return;
    }

    const name = this.cur();
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

    this.i++;

    switch (dir) {
      case "line":
        this.expect("int", undefined, "#line: expected integer literal");
        if (this.at("str")) this.i++;
        this.consumePPLineEnd();
        return;

      case "include": {
        const inc = this.parseIncludePath();
        this.consumePPLineEnd();

        if (!inc) return;

        const incPath = inc.path;
        if (!incPath) {
          this.issues.push({
            message: "#include: empty include path",
            range: inc.range
          });
          return;
        }

        if (this.include) {
          const res = this.include(incPath, this.filePath, inc.isSystem);
          if (!res) {
            this.issues.push({
              message: `#include not found: ${inc.isSystem ? `<${incPath}>` : incPath}`,
              range: inc.range
            });
            return;
          }

          if (this.includeSeen.has(res.filePath)) return;
          this.includeSeen.add(res.filePath);

          if (this.sem) {
            collectDefines(res.text, this.sem, res.filePath, this.currentPPConditions());
          }

          const p2 = new Parser(res.text, this.sem, this.include, this.includeSeen, res.filePath, this.currentPPConditions());
          p2.run();
        }

        return;
      }              

      case "undef":
        this.expect("ident", undefined, "#undef: expected identifier");
        this.consumePPLineEnd();
        return;

      case "ifdef": {
        const condTok = this.cur();
        if (this.expect("ident", undefined, "#ifdef: expected identifier")) {
          this.ppConditionStack.push({ name: condTok.text, isDefined: true });
        }
        this.consumePPLineEnd();
        return;
      }

      case "ifndef": {
        const condTok = this.cur();
        if (this.expect("ident", undefined, "#ifndef: expected identifier")) {
          this.ppConditionStack.push({ name: condTok.text, isDefined: false });
        }
        this.consumePPLineEnd();
        return;
      }

      case "endif":
        if (this.ppConditionStack.length > 0) this.ppConditionStack.pop();
        this.consumePPLineEnd();
        return;

      case "define":
        this.expect("ident", undefined, "#define: expected identifier");
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

  private parseGenericParamListAfterName(): string[] {
    const params: string[] = [];
    if (!this.match("op", "<")) return params;

    if (this.at("op", ">")) {
      const c = this.cur();
      this.issues.push({
        message: "generic parameter list cannot be empty",
        range: rangeOf(this.lines, c.start, c.end)
      });
      this.i++;
      return params;
    }

    while (!this.at("eof")) {
      const nameTok = this.cur();
      this.expect("ident", undefined, "generic parameter: expected identifier");
      if (nameTok.kind === "ident") params.push(nameTok.text);

      if (!this.match("punc", ",")) break;
    }

    this.expect("op", ">", "generic parameter list: expected '>'");
    return params;
  }

  private tryParseTypeArgsBeforeCall(): boolean {
    if (!this.at("op", "<")) return false;

    const saveI = this.i;
    const saveIssues = this.issues.length;

    this.i++;

    if (this.at("op", ">")) {
      this.i = saveI;
      this.issues.length = saveIssues;
      return false;
    }

    this.parseType();
    while (this.match("punc", ",")) this.parseType();

    if (!this.match("op", ">") || !this.at("punc", "(")) {
      this.i = saveI;
      this.issues.length = saveIssues;
      return false;
    }

    return true;
  }

  private parseParamListOptInfos(selfType?: TypeNode): ParamInfo[] {
    const params: ParamInfo[] = [];
    if (this.at("punc", ")")) return params;
  
    let seenVarArgs = false;
    const unknownType: TypeNode = { kind: "unknown" };
  
    while (true) {
      this.parseInlineAnnotations();
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
  
        this.parseInlineAnnotations();

        let t: TypeNode = unknownType;
        if (this.at("kw") && TYPE_KW.has(this.cur().text)) {
          t = this.parseType();
        }
  
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
  
        if (this.match("punc", ",")) {
          this.issues.push({
            message: "varargs parameter must be the last one",
            range: rangeOf(this.lines, nameTok.start, nameTok.end)
          });
        }
        break;
      }
  
      if (selfType && this.at("ident") && this.cur().text === "self") {
        const nameTok = this.cur();
        this.i++;

        params.push({
          name: "self",
          type: selfType,
          hasDefault: false,
          range: rangeOf(this.lines, nameTok.start, nameTok.end)
        });

        if (!this.match("punc", ",")) break;
        continue;
      }

      const t = this.parseType();
      this.parseInlineAnnotations();
  
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

  private parseEmbeddedStatement(allowEmptyBeforeElse = false) {
    if (this.at("eof") || this.at("punc", "}")) return;
    if (allowEmptyBeforeElse && this.at("kw", "else")) return;
    this.parseStatement();
  }

  private parseStartAfterKeyword() {
    this.expect("punc", "(");
    const paramsInfo = this.parseParamListOptInfos();
    this.expect("punc", ")");

    this.sem?.enterScope();
    for (const p of paramsInfo) this.sem?.declareLocalVar(p.name, p.type, p.range);

    this.parseBlock();

    this.sem?.exitScope();
  }

  private parseFunctionAfterKeyword(doc?: string) {
    const nameTok = this.cur();
    this.expect("ident", undefined, "function: expected identifier");
    let fnName = this.prev().text;
    let containerName: string | undefined;

    if (this.match("op", "::")) {
      containerName = fnName;
      this.sem?.useContainer(containerName, rangeOf(this.lines, nameTok.start, nameTok.end));
      this.expect("ident", undefined, "function: expected method name after '::'");
      fnName = this.prev().text;
    }

    const typeParams = this.parseGenericParamListAfterName();
    const hasSelfAnnotation = containerName ? this.hasPendingAnnotation("self") : false;

    this.expect("punc", "(");
    const selfType: TypeNode | undefined = containerName ? { kind: "container", name: containerName } : undefined;
    const paramsInfo = this.parseParamListOptInfos(selfType);
    this.expect("punc", ")");

    let ret: TypeNode = { kind: "prim", name: "i0" };
    if (this.match("op", "->")) ret = this.parseType();

    const fnRange = rangeOf(this.lines, nameTok.start, this.prev().end);

    if (this.match("punc", ";")) {
      if (containerName) this.sem?.declareContainerMethod(containerName, fnName, paramsInfo, ret, fnRange, false, doc, typeParams, { self: hasSelfAnnotation });
      else this.sem?.declareFunc(fnName, paramsInfo, ret, fnRange, false, doc, typeParams);
      return;
    }

    if (containerName) this.sem?.declareContainerMethod(containerName, fnName, paramsInfo, ret, fnRange, true, doc, typeParams, { self: hasSelfAnnotation });
    else this.sem?.declareFunc(fnName, paramsInfo, ret, fnRange, true, doc, typeParams);

    this.sem?.enterScope();
    if (containerName && selfType) this.declareImplicitSelfIfNeeded(hasSelfAnnotation, selfType, fnRange, paramsInfo);
    for (const p of paramsInfo) this.sem?.declareLocalVar(p.name, p.type, p.range);

    this.parseBlock();

    this.sem?.exitScope();
  }



  private parseContainerAfterKeyword(doc?: string) {
    const nameTok = this.cur();
    this.expect("ident", undefined, "container: expected identifier");
    const containerName = this.prev().text;
    const containerRange = rangeOf(this.lines, nameTok.start, nameTok.end);

    this.sem?.declareContainer(containerName, containerRange, doc);

    this.expect("punc", "{", "container: expected '{'");
    while (!this.at("eof") && !this.at("punc", "}")) {
      if (this.atRaw("comment")) {
        const tok = this.curRaw();
        this.i++;
        const itemDoc = unwrapDocComment(tok.text);
        if (itemDoc) this.pendingDoc = this.pendingDoc ? (this.pendingDoc + "\n" + itemDoc) : itemDoc;
        continue;
      }

      if (this.atRaw("punc", "@")) {
        const ann = this.parseAnnotation();
        if (ann) this.pendingAnnotations.push(ann);
        continue;
      }

      if (this.atRaw("punc", "#")) {
        this.parsePPDirective();
        continue;
      }

      if (this.match("kw", "function")) {
        const itemDoc = this.pendingDoc;
        this.pendingDoc = undefined;
        this.parseContainerMethodAfterKeyword(containerName, itemDoc);
        this.clearPendingMetadata();
        continue;
      }

      if (this.looksLikeDeclStart()) {
        this.parseContainerFieldDecl(containerName);
        this.pendingDoc = undefined;
        this.clearPendingMetadata();
        continue;
      }

      const c = this.cur();
      this.issues.push({
        message: "container: expected field or method",
        range: rangeOf(this.lines, c.start, c.end)
      });
      this.syncToStatementEnd();
      this.clearPendingMetadata();
    }

    this.expect("punc", "}", "container: expected '}'");
  }

  private parseContainerMethodAfterKeyword(containerName: string, doc?: string) {
    const nameTok = this.cur();
    this.expect("ident", undefined, "container method: expected identifier");
    const fnName = this.prev().text;
    const typeParams = this.parseGenericParamListAfterName();
    const hasSelfAnnotation = this.hasPendingAnnotation("self");

    this.expect("punc", "(");
    const selfType: TypeNode = { kind: "container", name: containerName };
    const paramsInfo = this.parseParamListOptInfos(selfType);
    this.expect("punc", ")");

    let ret: TypeNode = { kind: "prim", name: "i0" };
    if (this.match("op", "->")) ret = this.parseType();

    const fnRange = rangeOf(this.lines, nameTok.start, this.prev().end);

    if (this.match("punc", ";")) {
      this.sem?.declareContainerMethod(containerName, fnName, paramsInfo, ret, fnRange, false, doc, typeParams, { self: hasSelfAnnotation });
      return;
    }

    this.sem?.declareContainerMethod(containerName, fnName, paramsInfo, ret, fnRange, true, doc, typeParams, { self: hasSelfAnnotation });

    this.sem?.enterScope();
    this.declareImplicitSelfIfNeeded(hasSelfAnnotation, selfType, fnRange, paramsInfo);
    for (const p of paramsInfo) this.sem?.declareLocalVar(p.name, p.type, p.range);

    this.parseBlock();

    this.sem?.exitScope();
  }

  private parseContainerFieldDecl(containerName: string) {
    const mods = this.parseStorageMods();
    this.parseInlineAnnotations();

    if (this.looksLikeArrDeclForm()) {
      const arr = this.parseArrDeclHeader("container arr field");
      this.sem?.declareContainerField(containerName, arr.name, arr.type, arr.range, { readonly: mods.isReadonly });
      this.parseOptionalArrayInitializer();
      this.consumeStmtEnd("container arr field: expected ';' or end-of-line");
      return;
    }

    const fieldType = this.parseType();
    const nameTok = this.cur();
    this.expect("ident", undefined, "container field: expected identifier");
    const fieldName = this.prev().text;

    const declRange = rangeOf(this.lines, nameTok.start, nameTok.end);
    this.sem?.declareContainerField(containerName, fieldName, fieldType, declRange, { readonly: mods.isReadonly });

    if (this.match("op", "=")) this.parseExpression();
    this.consumeStmtEnd("container field: expected ';' or end-of-line");
  }

  private parseExternOp() {
    if (this.match("kw", "function")) {
      const nameTok = this.cur();
      this.expect("ident", undefined, "extern function: expected identifier");
      let fnName = this.prev().text;
      let containerName: string | undefined;

      if (this.match("op", "::")) {
        containerName = fnName;
        this.sem?.useContainer(containerName, rangeOf(this.lines, nameTok.start, nameTok.end));
        this.expect("ident", undefined, "extern function: expected method name after '::'");
        fnName = this.prev().text;
      }

      const typeParams = this.parseGenericParamListAfterName();
      const hasSelfAnnotation = containerName ? this.hasPendingAnnotation("self") : false;

      this.expect("punc", "(");
      const selfType: TypeNode | undefined = containerName ? { kind: "container", name: containerName } : undefined;
      const params = this.parseParamListOptInfos(selfType);
      this.expect("punc", ")");

      let ret: TypeNode = { kind: "prim", name: "i0" };
      if (this.match("op", "->")) ret = this.parseType();

      this.expect("punc", ";", "extern function: expected ';'");

      const fnRange = rangeOf(this.lines, nameTok.start, this.prev().end);
      if (containerName) this.sem?.declareContainerMethod(containerName, fnName, params, ret, fnRange, false, this.pendingDoc, typeParams, { self: hasSelfAnnotation });
      else this.sem?.declareFunc(fnName, params, ret, fnRange, false, this.pendingDoc, typeParams);
      this.pendingDoc = undefined;
      return;
    }

    const vType = this.parseType();
    const nameTok = this.cur();
    this.expect("ident", undefined, "extern var: expected identifier");
    const vName = this.prev().text;
    const vRange = rangeOf(this.lines, nameTok.start, nameTok.end);
    this.match("punc", ";");
    if (nameTok.kind === "ident") {
      this.sem?.declareExternGlobalVar(vName, vType, vRange);
    }
    this.pendingDoc = undefined;
  }

  private parseAlignStmt(isTopLevelDecls: boolean) {
    this.expect("kw", "align");
    this.expect("punc", "(", "align: expected '('");
    this.expect("int", undefined, "align: expected integer literal");
    this.expect("punc", ")", "align: expected ')'");

    if (this.match("punc", "{")) {
      while (!this.at("eof") && !this.at("punc", "}")) {
        if (this.atRaw("comment")) {
          const tok = this.curRaw();
          this.i++;
          const doc = unwrapDocComment(tok.text);
          if (doc) this.pendingDoc = this.pendingDoc ? (this.pendingDoc + "\n" + doc) : doc;
          continue;
        }
        if (this.atRaw("punc", "@")) {
          const ann = this.parseAnnotation();
          if (ann) this.pendingAnnotations.push(ann);
          continue;
        }
        if (this.atRaw("punc", "#")) { this.parsePPDirective(); continue; }

        if (!this.looksLikeDeclStart()) {
          const c = this.cur();
          this.issues.push({
            message: "align block: expected declaration",
            range: rangeOf(this.lines, c.start, c.end)
          });
          this.syncToStatementEnd();
          this.clearPendingMetadata();
          continue;
        }

        this.parseVarOrArrDecl(isTopLevelDecls);
        this.pendingDoc = undefined;
        this.clearPendingMetadata();
      }
      this.expect("punc", "}", "align block: expected '}'");
      return;
    }

    if (!this.looksLikeDeclStart()) {
      const c = this.cur();
      this.issues.push({
        message: "align: expected declaration",
        range: rangeOf(this.lines, c.start, c.end)
      });
      this.syncToStatementEnd();
      return;
    }

    this.parseVarOrArrDecl(isTopLevelDecls);
    this.clearPendingMetadata();
  }

  private parseSectionStmt() {
    this.expect("kw", "section");
    this.expect("punc", "(", "section: expected '('");
    this.expect("str", undefined, "section: expected string literal");
    this.expect("punc", ")", "section: expected ')'");
    this.expect("punc", "{", "section: expected '{'");

    while (!this.at("eof") && !this.at("punc", "}")) {
      if (this.atRaw("comment")) {
        const tok = this.curRaw();
        this.i++;
        const doc = unwrapDocComment(tok.text);
        if (doc) this.pendingDoc = this.pendingDoc ? (this.pendingDoc + "\n" + doc) : doc;
        continue;
      }

      if (this.atRaw("punc", "@")) {
        const ann = this.parseAnnotation();
        if (ann) this.pendingAnnotations.push(ann);
        continue;
      }

      if (this.atRaw("punc", "#")) {
        this.parsePPDirective();
        continue;
      }

      if (this.at("punc", "{")) {
        this.parseBlock();
        this.pendingDoc = undefined;
        this.clearPendingMetadata();
        continue;
      }

      if (this.match("kw", "start")) {
        this.parseStartAfterKeyword();
        this.pendingDoc = undefined;
        this.clearPendingMetadata();
        continue;
      }

      if (this.at("kw", "align")) {
        this.parseAlignStmt(true);
        this.pendingDoc = undefined;
        this.clearPendingMetadata();
        continue;
      }

      {
        const modsPos = this.i;
        const mods = this.parseStorageMods();
        if (mods.isGlobal || mods.isReadonly) {
          if (this.match("kw", "function")) {
            const doc = this.pendingDoc;
            this.pendingDoc = undefined;
            this.parseFunctionAfterKeyword(doc);
            this.clearPendingMetadata();
            continue;
          }
          this.i = modsPos;
        }
      }

      if (this.match("kw", "container")) {
        const doc = this.pendingDoc;
        this.pendingDoc = undefined;
        this.parseContainerAfterKeyword(doc);
        this.clearPendingMetadata();
        continue;
      }

      if (this.match("kw", "function")) {
        const doc = this.pendingDoc;
        this.pendingDoc = undefined;
        this.parseFunctionAfterKeyword(doc);
        this.clearPendingMetadata();
        continue;
      }

      if (this.looksLikeDeclStart()) {
        this.parseVarOrArrDecl(true);
        this.pendingDoc = undefined;
        this.clearPendingMetadata();
        continue;
      }

      const c = this.cur();
      this.issues.push({
        message: "section: expected declaration/function/start/scope/align/preprocessor",
        range: rangeOf(this.lines, c.start, c.end)
      });
      this.syncToStatementEnd();
      this.clearPendingMetadata();
    }

    this.expect("punc", "}", "section: expected '}'");
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
    if (this.atRaw("punc", "#")) { this.parsePPDirective(); return; }

    if (this.atRaw("comment")) {
      const tok = this.curRaw();
      this.i++;
      const doc = unwrapDocComment(tok.text);
      if (doc) this.pendingDoc = this.pendingDoc ? (this.pendingDoc + "\n" + doc) : doc;
      return;
    }

    if (this.atRaw("punc", "@")) {
      const ann = this.parseAnnotation();
      if (ann) this.pendingAnnotations.push(ann);
      return;
    }

    if (this.at("punc", ";")) {
      this.i++;
      return;
    }

    if (this.at("punc", "{")) {
      this.parseBlock();
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    if (this.at("kw", "function")) {
      this.i++;
      const doc = this.pendingDoc;
      this.pendingDoc = undefined;
      this.parseFunctionAfterKeyword(doc);
      this.clearPendingMetadata();
      return;
    }

    if (this.at("kw", "align")) {
      this.parseAlignStmt(false);
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    if (this.match("kw", "start")) {
      this.parseStartAfterKeyword();
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    if (this.at("kw", "if")) {
      this.i++;
      this.parseExpression();
      this.expect("punc", ";");

      this.parseEmbeddedStatement(true);

      if (this.match("kw", "else")) {
        this.parseEmbeddedStatement(false);
      }
      return;
    }

    if (this.at("kw", "while")) {
      this.i++;
      if (!this.at("punc", ";")) this.parseExpression();
      this.expect("punc", ";");
      this.parseEmbeddedStatement(false);
      return;
    }

    if (this.at("kw", "loop")) {
      this.i++;
      this.parseEmbeddedStatement(false);
      return;
    }

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
        this.match("punc", ";");
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
        this.parseExpression();
        while (this.match("punc", ",")) this.parseExpression();
      }
      this.expect("punc", ")");
      this.expect("punc", "{");
      this.skipInnerComments();
      while (!this.at("eof") && !this.at("punc", "}")) {
        this.expect("str", undefined, "asm_line: expected string literal");
        this.skipInnerComments();
        this.match("punc", ",");
        this.skipInnerComments();
      }
      this.expect("punc", "}");
      return;
    }

    if (this.looksLikeDeclStart()) {
      this.parseVarOrArrDecl(false);
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
      return;
    }

    try {
      this.parseExpression();
      this.expect("punc", ";");
      this.pendingDoc = undefined;
      this.clearPendingMetadata();
    } catch {
      this.syncToStatementEnd();
      this.clearPendingMetadata();
    }
  }

  private looksLikeTypeStart(): boolean {
    const c = this.cur();
    return this.atRaw("punc", "@")
      || (c.kind === "kw" && TYPE_KW.has(c.text))
      || c.kind === "ident";
  }

  private looksLikeGenericTypeDeclStart(): boolean {
    if (!this.atNonEOL(0, "ident") || !this.atNonEOL(1, "op", "<")) return false;

    let depth = 0;
    for (let off = 1; off < 64; off++) {
      const tok = this.peekNonEOL(off);
      if (tok.kind === "eof") return false;

      if (tok.kind === "op" && tok.text === "<") {
        depth++;
        continue;
      }

      if (tok.kind === "op" && tok.text === ">") {
        depth--;
        if (depth === 0) {
          return this.peekNonEOL(off + 1).kind === "ident";
        }
        continue;
      }
    }

    return false;
  }

  private looksLikeDeclStart(): boolean {
    if (this.at("kw", "glob") || this.at("kw", "ro")) return true;
    if (this.atRaw("punc", "@")) return true;
    if (this.at("kw") && TYPE_KW.has(this.cur().text)) return true;
    return (this.atNonEOL(0, "ident") && this.atNonEOL(1, "ident"))
      || this.looksLikeGenericTypeDeclStart();
  }

  private looksLikeArrDeclForm(): boolean {
    return this.at("kw", "arr")
      && this.atNonEOL(1, "ident")
      && this.atNonEOL(2, "punc", "[");
  }

  private parseArrDeclHeader(msg: string): { name: string; type: TypeNode; range: Range } {
    this.expect("kw", "arr", `${msg}: expected 'arr'`);

    const nameTok = this.cur();
    this.expect("ident", undefined, `${msg}: expected identifier`);
    const name = this.prev().text;

    this.expect("punc", "[", `${msg}: expected '['`);

    const len = this.parseConstArrayLen(msg);

    this.expect("punc", ",", `${msg}: expected ',' between size and type`);
    const elemType = this.parseType();

    this.expect("punc", "]", `${msg}: expected ']'`);

    const arrType: TypeNode = { kind: "arr", len: Number.isFinite(len) ? len : null, elem: elemType };
    return { name, type: arrType, range: rangeOf(this.lines, nameTok.start, nameTok.end) };
  }

  private parseOptionalArrayInitializer() {
    if (!this.match("op", "=")) return;

    if (this.match("punc", "{")) {
      this.skipInnerComments();
      if (!this.at("punc", "}")) {
        this.parseExpression();
        this.skipInnerComments();
        while (this.match("punc", ",")) {
          this.skipInnerComments();
          if (this.at("punc", "}")) break;
          this.parseExpression();
          this.skipInnerComments();
        }
      }
      this.expect("punc", "}", "arr_value: expected '}'");
      return;
    }

    this.parseExpression();
  }

  private parseVarOrArrDecl(isTopLevel: boolean) {
    const mods = this.parseStorageMods();
    this.parseInlineAnnotations();

    if (this.looksLikeArrDeclForm()) {
      const arr = this.parseArrDeclHeader("arr_decl");
      if (isTopLevel) this.sem?.declareGlobalVar(arr.name, arr.type, arr.range, { readonly: mods.isReadonly });
      else this.sem?.declareLocalVar(arr.name, arr.type, arr.range, { readonly: mods.isReadonly });

      this.parseOptionalArrayInitializer();
      this.consumeStmtEnd("arr_decl: expected ';' or end-of-line");
      return;
    }

    const vType = this.parseType();

    const nameTok = this.cur();
    this.expect("ident", undefined, "var_decl: expected identifier");
    const vName = this.prev().text;

    const declRange = rangeOf(this.lines, nameTok.start, nameTok.end);
    if (isTopLevel) this.sem?.declareGlobalVar(vName, vType, declRange, { readonly: mods.isReadonly });
    else this.sem?.declareLocalVar(vName, vType, declRange, { readonly: mods.isReadonly });

    if (this.match("op", "=")) this.parseExpression();
    this.consumeStmtEnd("var_decl: expected ';' or end-of-line");
  }

  private typeToName(t: TypeNode): string {
    switch (t.kind) {
      case "prim": return t.name;
      case "ptr": return `ptr ${this.typeToName(t.to)}`;
      case "arr": return `arr[${t.len ?? "?"}, ${this.typeToName(t.elem)}]`;
      case "func": return "func";
      case "container": return t.name;
      case "unknown": return "?";
    }
  }

  private parseType(): TypeNode {
    this.parseInlineAnnotations();

    if (this.match("kw", "ptr")) {
      const to = this.parseType();
      return { kind: "ptr", to };
    }

    if (this.match("kw", "arr")) {
      if (this.match("punc", "[")) {
        const len = this.parseConstArrayLen("arr type");

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
      const tok = this.cur();
      const name = tok.text;
      this.i++;
      if (this.sem?.hasContainer(name)) {
        this.sem.useContainer(name, rangeOf(this.lines, tok.start, tok.end));
      }
      return this.sem?.containerTypeForName(name) ?? { kind: "prim", name };
    }

    if (this.at("ident")) {
      const nameTok = this.cur();
      let name = nameTok.text;
      this.i++;

      if (this.match("op", "<")) {
        const args: string[] = [];
        const first = this.parseType();
        args.push(this.typeToName(first));
        while (this.match("punc", ",")) {
          const arg = this.parseType();
          args.push(this.typeToName(arg));
        }
        this.expect("op", ">", "type argument list: expected '>'");
        name += `<${args.join(", ")}>`;
      }

      if (this.sem?.hasContainer(name)) {
        this.sem.useContainer(name, rangeOf(this.lines, nameTok.start, nameTok.end));
      } else if (this.sem?.hasContainer(nameTok.text)) {
        this.sem.useContainer(nameTok.text, rangeOf(this.lines, nameTok.start, nameTok.end));
      }

      return this.sem?.containerTypeForName(name) ?? { kind: "prim", name };
    }

    this.expect("kw", undefined, "Expected a type keyword");
    return { kind: "unknown" };
  }

  private parseLiteral() {
    this.parseInlineAnnotations();
    if (this.match("int")) return;
    if (this.match("float")) return;
    if (this.match("str")) return;
    if (this.match("char")) return;

    const c = this.cur();
    this.issues.push({ message: `Expected literal, got '${c.text}'`, range: rangeOf(this.lines, c.start, c.end) });
    if (!this.at("eof")) this.i++;
  }

  private parseExpression(): ExprInfo { this.skipInnerComments(); return this.parseAssign(); }

  private parseInitializerList(): ExprInfo {
    const lbrace = this.cur();
    this.expect("punc", "{", "initializer list: expected '{'");
    this.skipInnerComments();

    if (!this.at("punc", "}")) {
      this.parseExpression();
      this.skipInnerComments();
      while (this.match("punc", ",")) {
        this.skipInnerComments();
        if (this.at("punc", "}")) break;
        this.parseExpression();
        this.skipInnerComments();
      }
    }

    this.expect("punc", "}", "initializer list: expected '}'");
    const endTok = this.prev();
    return { type: { kind: "unknown" }, start: lbrace.start, end: endTok.end };
  }

  private tryParseLambda(): ExprInfo | undefined {
    const saveI = this.i;
    const saveIssues = this.issues.length;

    if (!this.match("punc", "(")) return undefined;
    const lpar = this.prev();

    const paramsInfo = this.parseParamListOptInfos();
    if (!this.match("punc", ")") || !this.match("op", "=>")) {
      this.i = saveI;
      this.issues.length = saveIssues;
      return undefined;
    }

    let ret: TypeNode = { kind: "unknown" };

    this.sem?.enterScope();
    for (const p of paramsInfo) this.sem?.declareLocalVar(p.name, p.type, p.range);

    if (this.at("punc", "{")) {
      this.parseBlock();
    } else {
      const body = this.parseExpression();
      ret = body.type;
    }

    this.sem?.exitScope();

    const endTok = this.prev();
    return {
      type: {
        kind: "func",
        params: paramsInfo.filter((p) => !p.isVarArgs).map((p) => p.type),
        ret
      },
      start: lpar.start,
      end: endTok.end
    };
  }

  private parseAssign(): ExprInfo {
    let left = this.parseLogicalOr();
    if (this.at("op") && ["=","+=","-=","*=","/=","%=","|=","^=","&=","||=","&&="].includes(this.cur().text)) {
      this.i++;
      const right = this.parseAssign();
      left = { type: right.type, start: left.start, end: right.end };
    }
    return left;
  }

  private parseLogicalOr(): ExprInfo {
    let e = this.parseLogicalAnd();
    while (this.match("op","||")) { const r = this.parseLogicalAnd(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseLogicalAnd(): ExprInfo {
    let e = this.parseBitOr();
    while (this.match("op","&&")) { const r = this.parseBitOr(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseBitOr(): ExprInfo {
    let e = this.parseBitXor();
    while (this.match("op","|")) { const r = this.parseBitXor(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseBitXor(): ExprInfo {
    let e = this.parseBitAnd();
    while (this.match("op","^")) { const r = this.parseBitAnd(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseBitAnd(): ExprInfo {
    let e = this.parseEquality();
    while (this.match("op","&")) { const r = this.parseEquality(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseEquality(): ExprInfo {
    let e = this.parseRelational();
    while (this.at("op") && (this.cur().text === "==" || this.cur().text === "!=")) { this.i++; const r = this.parseRelational(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseRelational(): ExprInfo {
    let e = this.parseShift();
    while (this.at("op") && ["<","<=",">",">="].includes(this.cur().text)) { this.i++; const r = this.parseShift(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseShift(): ExprInfo {
    let e = this.parseAdd();
    while (this.at("op") && (this.cur().text === "<<" || this.cur().text === ">>")) { this.i++; const r = this.parseAdd(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseAdd(): ExprInfo {
    let e = this.parseMul();
    while (this.at("op") && (this.cur().text === "+" || this.cur().text === "-")) { this.i++; const r = this.parseMul(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }
  private parseMul(): ExprInfo {
    let e = this.parseUnary();
    while (this.at("op") && (this.cur().text === "*" || this.cur().text === "/" || this.cur().text === "%")) { this.i++; const r = this.parseUnary(); e = { type: { kind: "unknown" }, start: e.start, end: r.end }; }
    return e;
  }

  private parseUnary(): ExprInfo {
    if (this.at("kw") && ["not","neg","ref","dref"].includes(this.cur().text)) {
      const opTok = this.cur();
      this.i++;
      const inner = this.parseUnary();
      let t: TypeNode = { kind: "unknown" };
      if (opTok.text === "ref") t = { kind: "ptr", to: inner.type };
      else if (opTok.text === "dref" && inner.type.kind === "ptr") t = inner.type.to;
      return { type: t, start: opTok.start, end: inner.end ?? opTok.end };
    }
    if (this.at("op") && (this.cur().text === "+" || this.cur().text === "-")) {
      const opTok = this.cur();
      this.i++;
      const inner = this.parseUnary();
      return { type: inner.type, start: opTok.start, end: inner.end ?? opTok.end };
    }
    return this.parsePostfix();
  }

  private parsePostfix(): ExprInfo {
    let expr = this.parsePrimary();
    let wasCall = false;

    while (true) {
      if (this.tryParseTypeArgsBeforeCall()) {
        expr = { ...expr, end: this.prev().end };
        continue;
      }

      if (this.match("punc","(")) {
        wasCall = true;
        let argc = 0;
        if (!this.at("punc",")")) {
          this.parseExpression(); argc++;
          while (this.match("punc",",")) { this.parseExpression(); argc++; }
        }
        const endTok = this.cur();
        this.expect("punc",")");
        const callRange = rangeOf(this.lines, expr.start ?? endTok.start, endTok.end);

        if (expr.isSyscall) {
        } else if (expr.associatedContainerName && expr.associatedMethodName) {
          this.sem?.callAssociatedMethod(expr.associatedContainerName, expr.associatedMethodName, argc, callRange);
        } else if (expr.identName && !expr.isTypeName) {
          this.sem?.noteCallSite(expr.identName, callRange);
          this.sem?.callNamedOrValue(expr.identName, argc, callRange);
        } else {
          this.sem?.callIndirectExpr(expr.type, argc, callRange);
        }

        expr = { type: { kind: "unknown" }, start: expr.start, end: endTok.end };
        continue;
      }

      if (this.match("op", "::")) {
        const opTok = this.prev();
        const memberTok = this.cur();
        const ok = this.expect("ident", undefined, "scope access: expected identifier after '::'");
        const memberName = ok ? this.prev().text : "<missing>";
        const memberEnd = ok ? this.prev().end : memberTok.end;
        const memberRange = rangeOf(this.lines, expr.start ?? opTok.start, memberEnd);

        if (!expr.isTypeName || !expr.identName) {
          this.issues.push({
            message: "scope access: expected container type before '::'",
            range: memberRange
          });
          expr = { type: { kind: "unknown" }, start: expr.start, end: memberEnd };
          continue;
        }

        const memberType = this.sem?.getAssociatedMemberType(expr.identName, memberName, memberRange) ?? { kind: "unknown" };
        expr = {
          type: memberType,
          start: expr.start,
          end: memberEnd,
          associatedContainerName: expr.identName,
          associatedMethodName: memberName
        };
        continue;
      }

      if (this.match("punc", ".")) {
        const memberTok = this.cur();
        this.expect("ident", undefined, "member access: expected identifier after '.'");
        const memberName = this.prev().text;
        const memberRange = rangeOf(this.lines, memberTok.start, memberTok.end);

        if (expr.identName && !expr.isSyscall && !expr.isTypeName) {
          this.sem?.useVar(expr.identName, rangeOf(this.lines, expr.start ?? 0, expr.end ?? (expr.start ?? 0)));
        }

        const memberType = this.sem?.getContainerMemberType(expr.type, memberName, memberRange) ?? { kind: "unknown" };
        expr = { type: memberType, start: expr.start, end: memberTok.end };
        continue;
      }

      if (this.match("punc","[")) {
        if (expr.identName && !expr.isSyscall && !expr.isTypeName) {
          this.sem?.useVar(expr.identName, rangeOf(this.lines, expr.start ?? 0, expr.end ?? (expr.start ?? 0)));
        }

        this.parseExpression();
        while (this.match("punc",",")) this.parseExpression();
        this.expect("punc","]");

        let indexedType: TypeNode = { kind: "unknown" };
        if (expr.type.kind === "arr") indexedType = expr.type.elem;
        else if (expr.type.kind === "ptr") indexedType = expr.type.to;

        expr = { type: indexedType, start: expr.start, end: this.prev().end };
        continue;
      }

      if (this.match("kw","as")) {
        const t = this.parseType();
        expr = { ...expr, type: t, end: this.prev().end };
        continue;
      }

      break;
    }

    if (expr.identName && !wasCall && !expr.isSyscall && !expr.isTypeName && !expr.associatedContainerName) {
      this.sem?.useVar(expr.identName, rangeOf(this.lines, expr.start ?? 0, expr.end ?? (expr.start ?? 0)));
    }

    return expr;
  }

  private parsePrimary(): ExprInfo {
    this.skipInnerComments();
    this.parseInlineAnnotations();
    this.skipInnerComments();

    if (this.at("punc", "{")) return this.parseInitializerList();

    const lambda = this.tryParseLambda();
    if (lambda) return lambda;

    if (this.at("kw", "syscall")) {
      const tok = this.cur();
      this.i++;
      return { type: { kind: "unknown" }, isSyscall: true, start: tok.start, end: tok.end };
    }

    if (this.match("kw", "poparg")) {
      const tok = this.prev();
      return { type: { kind: "unknown" }, start: tok.start, end: tok.end };
    }

    if (this.match("kw", "sizeof")) {
      const kwTok = this.prev();
      let targetType: TypeNode = { kind: "unknown" };
      let end = kwTok.end;

      if (this.match("punc", "(")) {
        if (this.looksLikeTypeStart()) {
          targetType = this.parseType();
        } else {
          const inner = this.parseExpression();
          targetType = inner.type;
        }
        this.expect("punc", ")", "sizeof: expected ')'" );
        end = this.prev().end;
      } else if (this.looksLikeTypeStart()) {
        targetType = this.parseType();
        end = this.prev().end;
      } else {
        const inner = this.parseUnary();
        targetType = inner.type;
        end = inner.end ?? kwTok.end;
      }

      this.sem?.noteSizeof(rangeOf(this.lines, kwTok.start, end), targetType);
      return { type: { kind: "prim", name: "u64" }, start: kwTok.start, end };
    }

    if (this.at("ident")) {
      const tok = this.cur();
      const name = tok.text;
      this.i++;

      const vt = this.sem?.getVarType(name);
      if (vt) {
        return { type: vt, identName: name, start: tok.start, end: tok.end };
      }

      if (this.sem?.hasContainer(name)) {
        this.sem.useContainer(name, rangeOf(this.lines, tok.start, tok.end));
        return {
          type: { kind: "container", name },
          identName: name,
          isTypeName: true,
          start: tok.start,
          end: tok.end
        };
      }

      const t = this.sem?.getFunctionValueType(name) ?? { kind: "unknown" as const };
      return { type: t, identName: name, start: tok.start, end: tok.end };
    }

    if (this.at("int")) {
      const tok = this.cur(); this.i++;
      return { type: { kind: "prim", name: "i64" }, start: tok.start, end: tok.end };
    }

    if (this.at("float")) {
      const tok = this.cur(); this.i++;
      return { type: { kind: "prim", name: "f64" }, start: tok.start, end: tok.end };
    }

    if (this.at("str")) {
      const tok = this.cur(); this.i++;
      const value = unquote(tok.text);
      return {
        type: { kind: "arr", len: value.length + 1, elem: { kind: "prim", name: "u8" } },
        start: tok.start,
        end: tok.end
      };
    }

    if (this.at("char")) {
      const tok = this.cur(); this.i++;
      return { type: { kind: "prim", name: "i8" }, start: tok.start, end: tok.end };
    }

    if (this.match("punc","(")) {
      const lpar = this.prev();
      const inner = this.parseExpression();
      this.expect("punc",")","expected ')'");
      const rpar = this.prev();
      return { type: inner.type, start: lpar.start, end: rpar.end };
    }

    const c = this.cur();
    this.issues.push({
      message: `Expected primary expression, got '${c.text}'`,
      range: rangeOf(this.lines, c.start, c.end)
    });
    if (!this.at("eof")) this.i++;
    return { type: { kind: "unknown" }, start: c.start, end: c.end };
  }
}

export type IncludeResolverResult = { text: string; filePath: string };
export type IncludeResolver = (includePath: string, fromFilePath?: string, isSystemInclude?: boolean) => IncludeResolverResult | undefined;

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
  collectDefines(text, sem, filePath);
  const p = new Parser(text, sem, include, new Set<string>(), filePath);
  const syntax = p.run();
  sem.finish();
  return { issues: [...syntax, ...sem.issues], sem };
}