import {
  createConnection,
  TextDocuments,
  Diagnostic,
  DiagnosticSeverity,
  InitializeParams,
  InitializeResult,
  ProposedFeatures,
  TextDocumentSyncKind,
  Position,
  MarkupKind,
  Location,
  SemanticTokensBuilder
} from "vscode-languageserver/node";

import * as fs from "fs";
import * as path from "path";
import { fileURLToPath, pathToFileURL } from "url";

import { TextDocument } from "vscode-languageserver-textdocument";
import { analyze } from "./cplParser";
import {
  SemanticContext,
  formatType,
  formatFunctionSignature,
  FuncOverloadSym,
  ContainerSym,
  sizeofType
} from "./cplSemantics";

const connection = createConnection(ProposedFeatures.all);
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

const semByUri = new Map<string, SemanticContext>();
let workspaceRoots: string[] = [];

const semanticTokenTypes = ["macro"];
const semanticTokenModifiers = ["declaration"];

connection.onInitialize((params: InitializeParams): InitializeResult => {
  workspaceRoots = [];

  for (const folder of params.workspaceFolders ?? []) {
    const fsPath = uriToFsPath(folder.uri);
    if (fsPath) workspaceRoots.push(path.normalize(fsPath));
  }

  const rootUriPath = params.rootUri ? uriToFsPath(params.rootUri) : undefined;
  if (rootUriPath) workspaceRoots.push(path.normalize(rootUriPath));
  if (params.rootPath) workspaceRoots.push(path.normalize(params.rootPath));

  workspaceRoots = [...new Set(workspaceRoots)];

  return {
    capabilities: {
      textDocumentSync: TextDocumentSyncKind.Incremental,
      hoverProvider: true,
      definitionProvider: true,
      semanticTokensProvider: {
        legend: {
          tokenTypes: semanticTokenTypes,
          tokenModifiers: semanticTokenModifiers
        },
        full: true
      }
    }
  };
});

type IncludeResolverResult = { text: string; filePath: string };
type IncludeResolver = (includePath: string, fromFilePath?: string, isSystemInclude?: boolean) => IncludeResolverResult | undefined;

function uriToFsPath(uri: string): string | undefined {
  try {
    if (uri.startsWith("file://")) return fileURLToPath(uri);
  } catch {}
  return undefined;
}

function uniquePaths(paths: string[]): string[] {
  const seen = new Set<string>();
  const out: string[] = [];

  for (const p of paths) {
    const normalized = path.normalize(p);
    if (seen.has(normalized)) continue;
    seen.add(normalized);
    out.push(normalized);
  }

  return out;
}

function findMakefileUpwards(startPath?: string): string | undefined {
  const startDirs = uniquePaths([
    startPath ? (fs.existsSync(startPath) && fs.statSync(startPath).isDirectory() ? startPath : path.dirname(startPath)) : "",
    ...workspaceRoots
  ].filter(Boolean));

  for (const startDir of startDirs) {
    let dir = path.normalize(startDir);

    while (true) {
      const candidate = path.join(dir, "Makefile");
      if (fs.existsSync(candidate)) return candidate;

      const parent = path.dirname(dir);
      if (parent === dir) break;
      dir = parent;
    }
  }

  return undefined;
}

function parseMakefileVars(makefilePath: string): Map<string, string> {
  const vars = new Map<string, string>();

  try {
    const text = fs.readFileSync(makefilePath, "utf8");
    for (const line of text.split(/\r?\n/)) {
      const m = line.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*(?:\?=|:=|=)\s*(.*)$/);
      if (!m) continue;
      vars.set(m[1], m[2].replace(/\s+#.*$/, "").trim());
    }
  } catch {}

  return vars;
}

function expandMakeValue(value: string, vars: Map<string, string>, depth = 0): string {
  if (depth > 20) return value;
  return value.replace(/\$\(([^)]+)\)/g, (_full, name: string) => {
    const replacement = vars.get(name.trim());
    return replacement == null ? "" : expandMakeValue(replacement, vars, depth + 1);
  });
}

function makeIncludeDirs(fromFilePath?: string): string[] {
  const dirs: string[] = [];
  const makefilePath = findMakefileUpwards(fromFilePath);

  if (makefilePath) {
    const makefileDir = path.dirname(makefilePath);
    const vars = parseMakefileVars(makefilePath);

    dirs.push(path.join(makefileDir, "cpllib"));
    dirs.push(path.join(makefileDir, "include"));

    const cplLibDirRaw = vars.get("CPLLIBDIR");
    if (cplLibDirRaw) {
      const expanded = expandMakeValue(cplLibDirRaw, vars);
      dirs.push(path.isAbsolute(expanded) ? expanded : path.resolve(makefileDir, expanded));
    }
  }

  dirs.push("/usr/local/share/cpl/include");
  dirs.push("/usr/share/cpl/include");

  return uniquePaths(dirs).filter((d) => fs.existsSync(d));
}

function readCplFile(filePath: string): IncludeResolverResult | undefined {
  const normalized = path.normalize(filePath);

  try {
    const uri = pathToFileURL(normalized).toString();
    const openDoc = documents.get(uri);
    if (openDoc) return { text: openDoc.getText(), filePath: normalized };
  } catch {}

  try {
    if (fs.existsSync(normalized)) {
      const text = fs.readFileSync(normalized, "utf8");
      return { text, filePath: normalized };
    }
  } catch {}

  return undefined;
}

function makeIncludeResolver(
  documents: TextDocuments<TextDocument>,
  rootDocFsPath?: string
): IncludeResolver {
  return (includePath, fromFilePath, isSystemInclude) => {
    const inc = includePath.replace(/\\/g, "/");

    const baseDir =
      fromFilePath ? path.dirname(fromFilePath)
      : rootDocFsPath ? path.dirname(rootDocFsPath)
      : workspaceRoots[0] ?? process.cwd();

    const localCandidate = path.isAbsolute(inc) ? inc : path.resolve(baseDir, inc);
    const includeCandidates = makeIncludeDirs(fromFilePath ?? rootDocFsPath)
      .map((dir) => path.resolve(dir, inc));

    const candidates = isSystemInclude
      ? uniquePaths([...includeCandidates, localCandidate])
      : uniquePaths([localCandidate, ...includeCandidates]);

    for (const candidate of candidates) {
      const res = readCplFile(candidate);
      if (res) return res;
    }

    return undefined;
  };
}

async function validateTextDocument(doc: TextDocument) {
  const text = doc.getText();

  const docFsPath = uriToFsPath(doc.uri);
  const include = makeIncludeResolver(documents, docFsPath);

  const { issues, sem } = analyze(text, include, docFsPath);
  semByUri.set(doc.uri, sem);

  const diags: Diagnostic[] = issues.map((e) => ({
    severity: DiagnosticSeverity.Error,
    range: e.range,
    message: e.message,
    source: "cpl-ls"
  }));

  connection.sendDiagnostics({ uri: doc.uri, diagnostics: diags });
}

function inRange(pos: Position, r: { start: Position; end: Position }) {
  if (pos.line < r.start.line || pos.line > r.end.line) return false;
  if (pos.line === r.start.line && pos.character < r.start.character) return false;
  if (pos.line === r.end.line && pos.character > r.end.character) return false;
  return true;
}

function sameFsPath(a?: string, b?: string): boolean {
  if (!a || !b) return false;
  return path.normalize(a) === path.normalize(b);
}

function belongsToFile(symbolFilePath: string | undefined, currentFilePath: string | undefined): boolean {
  return !symbolFilePath || !currentFilePath || sameFsPath(symbolFilePath, currentFilePath);
}

function locationFor(filePath: string | undefined, range: { start: Position; end: Position }, fallbackUri: string): Location {
  const uri = filePath ? pathToFileURL(path.normalize(filePath)).toString() : fallbackUri;
  return Location.create(uri, range);
}

function firstDeclarationLocation(
  fn: FuncOverloadSym,
  currentFilePath: string | undefined,
  fallbackUri: string
): Location | undefined {
  if (!fn.decls.length) return undefined;

  const externalIndex = fn.decls.findIndex((_, i) => {
    const file = fn.declFiles?.[i];
    return file && currentFilePath && !sameFsPath(file, currentFilePath);
  });

  const idx = externalIndex >= 0 ? externalIndex : 0;
  return locationFor(fn.declFiles?.[idx] ?? fn.primaryFilePath, fn.decls[idx], fallbackUri);
}

function implementationLocation(fn: FuncOverloadSym, fallbackUri: string): Location | undefined {
  if (fn.def) return locationFor(fn.defFilePath ?? fn.primaryFilePath, fn.def, fallbackUri);
  return locationFor(fn.primaryFilePath, fn.primaryRange, fallbackUri);
}

function targetFuncLocation(
  fn: FuncOverloadSym,
  sourceKind: "call" | "value" | "declaration" | "definition",
  currentFilePath: string | undefined,
  fallbackUri: string
): Location | undefined {
  if (sourceKind === "definition") {
    return firstDeclarationLocation(fn, currentFilePath, fallbackUri) ?? implementationLocation(fn, fallbackUri);
  }

  if (sourceKind === "declaration") {
    return implementationLocation(fn, fallbackUri);
  }

  return implementationLocation(fn, fallbackUri);
}

function containerLocation(container: ContainerSym, fallbackUri: string): Location {
  return locationFor(container.filePath, container.range, fallbackUri);
}

function findContainerAtDeclaration(
  sem: SemanticContext,
  pos: Position,
  currentFilePath: string | undefined
): ContainerSym | undefined {
  for (const c of sem.containerDecls) {
    if (belongsToFile(c.filePath, currentFilePath) && inRange(pos, c.range)) return c;
  }
  return undefined;
}

function findFuncAtDeclaration(
  sem: SemanticContext,
  pos: Position,
  currentFilePath: string | undefined
): { fn: FuncOverloadSym; sourceKind: "declaration" | "definition" } | undefined {
  for (const list of sem.funcs.values()) {
    for (const fn of list) {
      if (fn.def && belongsToFile(fn.defFilePath ?? fn.primaryFilePath, currentFilePath) && inRange(pos, fn.def)) {
        return { fn, sourceKind: "definition" };
      }

      for (let i = 0; i < fn.decls.length; i++) {
        if (belongsToFile(fn.declFiles?.[i] ?? fn.primaryFilePath, currentFilePath) && inRange(pos, fn.decls[i])) {
          return { fn, sourceKind: "declaration" };
        }
      }
    }
  }

  return undefined;
}

function rangeLen(r: { start: Position; end: Position }): number {
  if (r.start.line !== r.end.line) return 0;
  return Math.max(0, r.end.character - r.start.character);
}

function renderOverloads(
  overloads: FuncOverloadSym[],
  opts?: { selected?: FuncOverloadSym; title?: string; extra?: string }
): string {
  const lines: string[] = [];

  if (opts?.title) lines.push(`**${opts.title}**`, "");

  const ordered = [...overloads].sort((a, b) => {
    const ad = a.params.length;
    const bd = b.params.length;
    if (ad !== bd) return ad - bd;
    return formatFunctionSignature(a).localeCompare(formatFunctionSignature(b));
  });

  if (ordered.length === 1) {
    lines.push("```cpl", formatFunctionSignature(ordered[0]), "```");
    if (ordered[0].doc?.trim()) {
      lines.push("", ordered[0].doc);
    }
    if (opts?.extra) lines.push("", opts.extra);
    return lines.join("\n");
  }

  const selected = opts?.selected;
  if (selected) {
    lines.push("```cpl", formatFunctionSignature(selected), "```", "");
    lines.push(`_Overloads: ${ordered.length}_`, "");
  } else {
    lines.push(`_Found overloads: ${ordered.length}_`, "");
  }

  for (const fn of ordered) {
    const sig = formatFunctionSignature(fn);
    const marker = selected === fn ? "- **(selected)** " : "- ";
    lines.push(`${marker}\`${sig}\``);
  }

  if (selected?.doc?.trim()) {
    lines.push("", selected.doc);
  } else {
    const docCarrier = ordered.find((o) => o.doc?.trim());
    if (docCarrier?.doc?.trim()) lines.push("", docCarrier.doc);
  }

  if (opts?.extra) lines.push("", opts.extra);

  return lines.join("\n");
}

function findFuncDeclHover(sem: SemanticContext, pos: Position, currentFilePath?: string): FuncOverloadSym | undefined {
  for (const list of sem.funcs.values()) {
    for (const fn of list) {
      if (fn.def && belongsToFile(fn.defFilePath ?? fn.primaryFilePath, currentFilePath) && inRange(pos, fn.def)) return fn;

      for (let i = 0; i < fn.decls.length; i++) {
        if (belongsToFile(fn.declFiles?.[i] ?? fn.primaryFilePath, currentFilePath) && inRange(pos, fn.decls[i])) return fn;
      }

      if (belongsToFile(fn.primaryFilePath, currentFilePath) && inRange(pos, fn.primaryRange)) return fn;
    }
  }
  return undefined;
}

connection.onHover((params) => {
  const sem = semByUri.get(params.textDocument.uri);
  if (!sem) return null;

  const docFsPath = uriToFsPath(params.textDocument.uri);

  const sz = sem.sizeofSites.find((s) => belongsToFile(s.filePath, docFsPath) && inRange(params.position, s.range));
  if (sz) {
    const computed = sz.size ?? sizeofType(sz.targetType) ?? null;
    const value = computed == null
      ? `\`\`\`cpl
sizeof(${formatType(sz.targetType)}) = ?
\`\`\`

Size can't be resolved statically.`
      : `\`\`\`cpl
sizeof(${formatType(sz.targetType)}) = ${computed}
\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const vu = sem.varUses.find((v) => belongsToFile(v.filePath, docFsPath) && inRange(params.position, v.range));
  if (vu) {
    const value = `\`\`\`cpl\n${formatType(vu.type)} ${vu.name}\n\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const mu = sem.macroUses.find((m) => belongsToFile(m.filePath, docFsPath) && inRange(params.position, m.range));
  if (mu) {
    const ms = sem.macros.get(mu.name);
    if (ms) {
      const v =
        ms.value.kind === "string" ? JSON.stringify(ms.value.value) :
        ms.value.kind === "number" ? String(ms.value.value) :
        ms.value.text;

      const value = `\`\`\`cpl\n#define ${ms.name} ${v}\n\`\`\``;
      return { contents: { kind: MarkupKind.Markdown, value } };
    }
  }

  const cs = sem.callSites.find((c) => belongsToFile(c.filePath, docFsPath) && inRange(params.position, c.range));
  if (cs) {
    const all = sem.funcs.get(cs.name) ?? [];
    if (all.length > 0) {
      const res = cs.resolution;
      let extra = "";
      if (res?.status === "unknown") extra = `Call: unknown function \`${cs.name}\``;
      else if (res?.status === "no_match") extra = `There is no overload for \`${cs.argc}\` arguments`;
      else if (res?.status === "ambiguous") extra = `Can't select the function by arguments`;

      const value = renderOverloads(
        res?.candidates?.length ? res.candidates : all,
        {
          selected: res?.selected,
          title: `Call ${cs.name}(...)`,
          extra: extra || undefined
        }
      );

      return { contents: { kind: MarkupKind.Markdown, value } };
    }
  }

  const fu = sem.funcValueUses.find((f) => belongsToFile(f.filePath, docFsPath) && inRange(params.position, f.range));
  if (fu) {
    const overloads = sem.funcs.get(fu.name) ?? [];
    if (overloads.length > 0) {
      const value = renderOverloads(overloads, { title: `Function ${fu.name}` });
      return { contents: { kind: MarkupKind.Markdown, value } };
    }
  }

  const ics = sem.indirectCallSites.find((c) => belongsToFile(c.filePath, docFsPath) && inRange(params.position, c.range));
  if (ics) {
    const value = `\`\`\`cpl
callable ${formatType(ics.calleeType)}
\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const cu = sem.containerUses.find((c) => belongsToFile(c.filePath, docFsPath) && inRange(params.position, c.range));
  if (cu) {
    const c = sem.containers.get(cu.name);
    if (c) {
      const value = `\`\`\`cpl
container ${c.name}
\`\`\`` + (c.doc?.trim() ? `

${c.doc}` : "");
      return { contents: { kind: MarkupKind.Markdown, value } };
    }
  }

  const cd = findContainerAtDeclaration(sem, params.position, docFsPath);
  if (cd) {
    const value = `\`\`\`cpl
container ${cd.name}
\`\`\`` + (cd.doc?.trim() ? `

${cd.doc}` : "");
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const fnDecl = findFuncDeclHover(sem, params.position, docFsPath);
  if (fnDecl) {
    const displayName = fnDecl.containerName ? `${fnDecl.containerName}::${fnDecl.name}` : fnDecl.name;
    const list = sem.funcs.get(displayName) ?? sem.funcs.get(fnDecl.name) ?? [fnDecl];
    const value = renderOverloads(list, { selected: fnDecl, title: `Function ${displayName}` });
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const vd = sem.varDecls.find((v) => belongsToFile(v.filePath, docFsPath) && inRange(params.position, v.range));
  if (vd) {
    const value = `\`\`\`cpl\n${formatType(vd.type)} ${vd.name}\n\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  return null;
});


function uniqueLocations(locations: Location[]): Location[] {
  const seen = new Set<string>();
  const out: Location[] = [];

  for (const loc of locations) {
    const key = `${loc.uri}:${loc.range.start.line}:${loc.range.start.character}:${loc.range.end.line}:${loc.range.end.character}`;
    if (seen.has(key)) continue;
    seen.add(key);
    out.push(loc);
  }

  return out;
}

function locationsForFunctions(
  fns: FuncOverloadSym[],
  sourceKind: "call" | "value" | "declaration" | "definition",
  currentFilePath: string | undefined,
  fallbackUri: string
): Location[] {
  return uniqueLocations(
    fns
      .map((fn) => targetFuncLocation(fn, sourceKind, currentFilePath, fallbackUri))
      .filter((loc): loc is Location => !!loc)
  );
}

connection.onDefinition((params) => {
  const sem = semByUri.get(params.textDocument.uri);
  if (!sem) return null;

  const docFsPath = uriToFsPath(params.textDocument.uri);
  const pos = params.position;

  const mu = sem.macroUses.find((m) => belongsToFile(m.filePath, docFsPath) && inRange(pos, m.range));
  if (mu) {
    const ms = sem.macros.get(mu.name);
    if (ms) return locationFor(ms.filePath, ms.range, params.textDocument.uri);
  }

  const vu = sem.varUses.find((v) => belongsToFile(v.filePath, docFsPath) && inRange(pos, v.range));
  if (vu?.targetRange) {
    return locationFor(vu.targetFilePath, vu.targetRange, params.textDocument.uri);
  }

  const cu = sem.containerUses.find((c) => belongsToFile(c.filePath, docFsPath) && inRange(pos, c.range));
  if (cu?.targetRange) {
    return locationFor(cu.targetFilePath, cu.targetRange, params.textDocument.uri);
  }

  const cd = findContainerAtDeclaration(sem, pos, docFsPath);
  if (cd) {
    return containerLocation(cd, params.textDocument.uri);
  }

  const cs = sem.callSites.find((c) => belongsToFile(c.filePath, docFsPath) && inRange(pos, c.range));
  if (cs) {
    const selected = cs.resolution?.selected;
    const candidates = selected ? [selected] : (cs.resolution?.candidates?.length ? cs.resolution.candidates : (sem.funcs.get(cs.name) ?? []));
    const locations = locationsForFunctions(candidates, "call", docFsPath, params.textDocument.uri);
    if (locations.length === 1) return locations[0];
    if (locations.length > 1) return locations;
  }

  const fu = sem.funcValueUses.find((f) => belongsToFile(f.filePath, docFsPath) && inRange(pos, f.range));
  if (fu) {
    const locations = locationsForFunctions(sem.funcs.get(fu.name) ?? [], "value", docFsPath, params.textDocument.uri);
    if (locations.length === 1) return locations[0];
    if (locations.length > 1) return locations;
  }

  const fnDecl = findFuncAtDeclaration(sem, pos, docFsPath);
  if (fnDecl) {
    const loc = targetFuncLocation(fnDecl.fn, fnDecl.sourceKind, docFsPath, params.textDocument.uri);
    return loc ?? null;
  }

  return null;
});

connection.languages.semanticTokens.on((params) => {
  const sem = semByUri.get(params.textDocument.uri);
  const doc = documents.get(params.textDocument.uri);
  const builder = new SemanticTokensBuilder();

  if (!sem || !doc) return builder.build();

  const docFsPath = uriToFsPath(params.textDocument.uri);
  const tokens: { line: number; char: number; len: number; modifier: number }[] = [];
  const seen = new Set<string>();

  const addToken = (range: { start: Position; end: Position }, modifier = 0) => {
    const len = rangeLen(range);
    if (len <= 0) return;

    const key = `${range.start.line}:${range.start.character}:${len}`;
    if (seen.has(key)) return;
    seen.add(key);
    tokens.push({ line: range.start.line, char: range.start.character, len, modifier });
  };

  for (const macro of sem.macroDecls) {
    if (belongsToFile(macro.filePath, docFsPath)) addToken(macro.range, 1);
  }

  for (const use of sem.macroUses) {
    if (belongsToFile(use.filePath, docFsPath)) addToken(use.range);
  }

  const macroNames = new Set(sem.macroDecls.map((m) => m.name));
  if (macroNames.size > 0) {
    const text = doc.getText();
    const lineStarts: number[] = [0];
    for (let i = 0; i < text.length; i++) {
      if (text.charCodeAt(i) === 10) lineStarts.push(i + 1);
    }

    const positionAtOffset = (offset: number): Position => {
      let lo = 0, hi = lineStarts.length - 1;
      while (lo <= hi) {
        const mid = (lo + hi) >> 1;
        const start = lineStarts[mid];
        const next = mid + 1 < lineStarts.length ? lineStarts[mid + 1] : Number.POSITIVE_INFINITY;
        if (offset < start) hi = mid - 1;
        else if (offset >= next) lo = mid + 1;
        else return Position.create(mid, offset - start);
      }
      return Position.create(0, 0);
    };

    const identRe = /\b[A-Za-z_]\w*\b/g;
    let m: RegExpExecArray | null;
    while ((m = identRe.exec(text))) {
      if (!macroNames.has(m[0])) continue;
      const pos = positionAtOffset(m.index);
      addToken({ start: pos, end: Position.create(pos.line, pos.character + m[0].length) });
    }
  }

  tokens.sort((a, b) => a.line - b.line || a.char - b.char);
  for (const t of tokens) builder.push(t.line, t.char, t.len, 0, t.modifier);
  return builder.build();
});

documents.onDidChangeContent((change) => validateTextDocument(change.document));
documents.onDidOpen((e) => validateTextDocument(e.document));
documents.onDidSave((e) => validateTextDocument(e.document));

documents.listen(connection);
connection.listen();