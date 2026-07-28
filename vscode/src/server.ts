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
  SemanticTokensBuilder,
  CompletionItem,
  CompletionItemKind
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
  formatAnnotations,
  TypeNode
} from "./cplSemantics";
import {
  CplSysType,
  defaultSysTypeForHost,
  expandMakeValue,
  inferSysTypeFromCompilerArgs,
  parseMakefileVarsText,
  sysTypeToPredefinedMacro
} from "./cplTarget";

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
      completionProvider: {
        triggerCharacters: [".", ":"]
      },
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
  try {
    const text = fs.readFileSync(makefilePath, "utf8");
    return parseMakefileVarsText(text);
  } catch {}

  return new Map<string, string>();
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

function inferSysTypeForFile(fromFilePath?: string): CplSysType {
  const makefilePath = findMakefileUpwards(fromFilePath);
  if (makefilePath) {
    const vars = parseMakefileVars(makefilePath);
    const runArgsRaw = vars.get("RUN_ARGS");
    if (runArgsRaw) {
      const inferred = inferSysTypeFromCompilerArgs(expandMakeValue(runArgsRaw, vars));
      if (inferred) return inferred;
    }
  }

  return defaultSysTypeForHost(process.platform);
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

  const predefine = sysTypeToPredefinedMacro(inferSysTypeForFile(docFsPath));
  const { issues, sem } = analyze(text, include, docFsPath, {
    predefines: predefine ? [predefine] : []
  });
  semByUri.set(doc.uri, sem);

  const diags: Diagnostic[] = issues.map((e) => ({
    severity: DiagnosticSeverity.Error,
    range: e.range,
    message: e.message,
    source: "cpl-ls"
  }));

  connection.sendDiagnostics({ uri: doc.uri, diagnostics: diags });
}

function semanticContextForDocument(doc: TextDocument): SemanticContext {
  const existing = semByUri.get(doc.uri);
  if (existing) return existing;

  const docFsPath = uriToFsPath(doc.uri);
  const include = makeIncludeResolver(documents, docFsPath);
  const predefine = sysTypeToPredefinedMacro(inferSysTypeForFile(docFsPath));
  const { sem } = analyze(doc.getText(), include, docFsPath, {
    predefines: predefine ? [predefine] : []
  });
  semByUri.set(doc.uri, sem);
  return sem;
}

function inRange(pos: Position, r: { start: Position; end: Position }) {
  if (pos.line < r.start.line || pos.line > r.end.line) return false;
  if (pos.line === r.start.line && pos.character < r.start.character) return false;
  if (pos.line === r.end.line && pos.character > r.end.character) return false;
  return true;
}

function quoteCplChar(value: string): string {
  return `'${value
    .replace(/\\/g, "\\\\")
    .replace(/'/g, "\\'")
    .replace(/\n/g, "\\n")
    .replace(/\t/g, "\\t")
    .replace(/\r/g, "\\r")
    .replace(/\0/g, "\\0")}'`;
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

function annotatedFunctionLines(fn: FuncOverloadSym): string[] {
  return [...formatAnnotations(fn.annotations), formatFunctionSignature(fn)];
}

function annotatedFunctionInline(fn: FuncOverloadSym): string {
  return [...formatAnnotations(fn.annotations), formatFunctionSignature(fn)].join(" ");
}

function annotatedVariableLines(
  name: string,
  type: TypeNode,
  annotations?: readonly string[],
  readonly = false
): string[] {
  return [
    ...formatAnnotations(annotations),
    `${readonly ? "ro " : ""}${formatType(type)} ${name}`
  ];
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
    lines.push("```cpl", ...annotatedFunctionLines(ordered[0]), "```");
    if (ordered[0].doc?.trim()) lines.push("", ordered[0].doc);
    if (opts?.extra) lines.push("", opts.extra);
    return lines.join("\n");
  }

  const selected = opts?.selected;
  if (selected) {
    lines.push("```cpl", ...annotatedFunctionLines(selected), "```", "");
    lines.push(`_Overloads: ${ordered.length}_`, "");
  } else {
    lines.push(`_Found overloads: ${ordered.length}_`, "");
  }

  for (const fn of ordered) {
    const sig = annotatedFunctionInline(fn);
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

function isInstanceMethod(fn: FuncOverloadSym): boolean {
  return !!fn.isSelfMethod || fn.params[0]?.name === "self";
}

function containerNameFromType(sem: SemanticContext, t: TypeNode): string | undefined {
  if (t.kind === "container") return t.name;
  if (t.kind === "prim" && sem.containers.has(t.name)) return t.name;
  if (t.kind === "ptr") return containerNameFromType(sem, t.to);
  return undefined;
}

function renderContainerMembers(container: ContainerSym, title = "Available members"): string {
  const lines: string[] = [`**${title}**`];

  const fields = [...container.fields.values()]
    .sort((a, b) => a.name.localeCompare(b.name));
  if (fields.length) {
    lines.push("", "**Fields**");
    for (const field of fields) {
      const declaration = annotatedVariableLines(field.name, field.type, field.annotations, !!field.readonly).join(" ");
      lines.push(`- \`${declaration}\``);
    }
  }

  const methods = [...container.methods.values()].flat();
  const instanceMethods = methods
    .filter(isInstanceMethod)
    .sort((a, b) => formatFunctionSignature(a).localeCompare(formatFunctionSignature(b)));
  const associatedFunctions = methods
    .filter((fn) => !isInstanceMethod(fn))
    .sort((a, b) => formatFunctionSignature(a).localeCompare(formatFunctionSignature(b)));

  if (instanceMethods.length) {
    lines.push("", "**Methods**");
    for (const fn of instanceMethods) lines.push(`- \`${annotatedFunctionInline(fn)}\``);
  }

  if (associatedFunctions.length) {
    lines.push("", "**Functions**");
    for (const fn of associatedFunctions) lines.push(`- \`${annotatedFunctionInline(fn)}\``);
  }

  if (lines.length === 1) lines.push("", "_No fields or functions declared._");
  return lines.join("\n");
}

function renderContainerHover(sem: SemanticContext, container: ContainerSym): string {
  const lines = [
    "```cpl",
    ...formatAnnotations(container.annotations),
    `container ${container.name}`,
    "```"
  ];

  const size = sem.sizeofType({ kind: "container", name: container.name });
  if (size != null) lines.push("", `**Size:** \`${size} bytes\``);
  if (container.doc?.trim()) lines.push("", container.doc);
  lines.push("", renderContainerMembers(container));
  return lines.join("\n");
}

function renderContainerDetailsForType(sem: SemanticContext, type: TypeNode): string | undefined {
  const name = containerNameFromType(sem, type);
  if (!name) return undefined;

  const container = sem.containers.get(name);
  if (!container) return undefined;

  const lines: string[] = [];
  if (container.annotations?.length) {
    lines.push("**Container annotations**", "", `\`${formatAnnotations(container.annotations).join(" ")}\``, "");
  }

  const size = sem.sizeofType({ kind: "container", name });
  if (size != null) lines.push(`**Size:** \`${size} bytes\``, "");
  lines.push(renderContainerMembers(container, `Available on ${name}`));
  return lines.join("\n");
}

function positionBeforeOrEqual(a: Position, b: Position): boolean {
  return a.line < b.line || (a.line === b.line && a.character <= b.character);
}

function signaturesMarkdown(fns: FuncOverloadSym[]): string {
  const lines = ["```cpl"];
  for (const fn of fns) lines.push(...annotatedFunctionLines(fn));
  lines.push("```");
  return lines.join("\n");
}

function methodCompletionItems(container: ContainerSym, access: "::" | "."): CompletionItem[] {
  const items: CompletionItem[] = [];

  for (const [name, overloads] of container.methods) {
    const associated = overloads.filter((fn) => !isInstanceMethod(fn));
    const instance = overloads.filter(isInstanceMethod);
    const visible = access === "::"
      ? [...associated, ...instance]
      : [...instance, ...associated];
    if (!visible.length) continue;

    const first = visible[0];
    const docs = visible
      .map((fn) => fn.doc?.trim())
      .filter((doc): doc is string => !!doc);
    const notes: string[] = [];
    if (access === "::" && associated.length === 0 && instance.length > 0) {
      notes.push("Instance method; call it with `.` on a value.");
    }
    if (access === "." && instance.length === 0 && associated.length > 0) {
      notes.push("Associated function; call it with `::` on the container type.");
    }

    items.push({
      label: name,
      kind: isInstanceMethod(first) ? CompletionItemKind.Method : CompletionItemKind.Function,
      detail: visible.map(annotatedFunctionInline).join(" | "),
      documentation: {
        kind: MarkupKind.Markdown,
        value: [
          signaturesMarkdown(visible),
          ...notes,
          ...docs
        ].filter(Boolean).join("\n\n")
      },
      insertText: name,
      sortText: `${isInstanceMethod(first) ? "1" : "0"}_${name}`
    });
  }

  return items;
}

function fieldCompletionItems(container: ContainerSym): CompletionItem[] {
  return [...container.fields.values()].map((field) => {
    const declaration = annotatedVariableLines(field.name, field.type, field.annotations, !!field.readonly);
    return {
      label: field.name,
      kind: CompletionItemKind.Field,
      detail: declaration.join(" "),
      documentation: {
        kind: MarkupKind.Markdown,
        value: ["```cpl", ...declaration, "```"].join("\n")
      },
      insertText: field.name,
      sortText: `0_${field.name}`
    };
  });
}

function findVarContainerAtPosition(
  sem: SemanticContext,
  name: string,
  pos: Position,
  currentFilePath: string | undefined
): ContainerSym | undefined {
  const candidates = sem.varDecls
    .filter((v) => v.name === name && belongsToFile(v.filePath, currentFilePath) && positionBeforeOrEqual(v.range.start, pos))
    .sort((a, b) => b.range.start.line - a.range.start.line || b.range.start.character - a.range.start.character);

  for (const candidate of candidates) {
    const containerName = containerNameFromType(sem, candidate.type);
    if (!containerName) continue;
    const container = sem.containers.get(containerName);
    if (container) return container;
  }

  const global = sem.globals.get(name);
  const globalContainerName = global ? containerNameFromType(sem, global.type) : undefined;
  return globalContainerName ? sem.containers.get(globalContainerName) : undefined;
}

connection.onCompletion((params): CompletionItem[] => {
  const doc = documents.get(params.textDocument.uri);
  if (!doc) return [];

  const sem = semanticContextForDocument(doc);
  const docFsPath = uriToFsPath(params.textDocument.uri);
  const prefix = doc.getText({
    start: Position.create(params.position.line, 0),
    end: params.position
  });

  const scopeMatch = prefix.match(/\b([A-Za-z_]\w*)::([A-Za-z_]\w*)?$/);
  if (scopeMatch) {
    const container = sem.containers.get(scopeMatch[1]);
    return container ? methodCompletionItems(container, "::") : [];
  }

  const dotMatch = prefix.match(/\b([A-Za-z_]\w*)\.([A-Za-z_]\w*)?$/);
  if (dotMatch) {
    const container = findVarContainerAtPosition(sem, dotMatch[1], params.position, docFsPath);
    return container ? [...fieldCompletionItems(container), ...methodCompletionItems(container, ".")] : [];
  }

  return [];
});

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

  const docLink = sem.docLinks.find((d) => belongsToFile(d.filePath, docFsPath) && inRange(params.position, d.range));
  if (docLink) {
    const value = `Linked comment for \`${docLink.targetName}\`` + (docLink.doc.trim() ? `\n\n${docLink.doc}` : "");
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const sz = sem.sizeofSites.find((s) => belongsToFile(s.filePath, docFsPath) && inRange(params.position, s.range));
  if (sz) {
    const computed = sem.sizeofType(sz.targetType) ?? sz.size ?? null;
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
    const details = renderContainerDetailsForType(sem, vu.type);
    const declaration = annotatedVariableLines(vu.name, vu.type, vu.annotations, !!vu.readonly);
    const value = ["```cpl", ...declaration, "```"].join("\n") + (details ? `\n\n${details}` : "");
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const mu = sem.macroUses.find((m) => belongsToFile(m.filePath, docFsPath) && inRange(params.position, m.range));
  if (mu) {
    const ms = sem.macros.get(mu.name);
    if (ms) {
      const v =
        ms.value.kind === "string" ? JSON.stringify(ms.value.value) :
        ms.value.kind === "char" ? quoteCplChar(ms.value.value) :
        ms.value.kind === "number" ? String(ms.value.value) :
        ms.value.text;

      const value = `\`\`\`cpl\n#define ${ms.name} ${v}\n\`\`\`` + (ms.doc?.trim() ? `\n\n${ms.doc}` : "");
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
      return { contents: { kind: MarkupKind.Markdown, value: renderContainerHover(sem, c) } };
    }
  }

  const cd = findContainerAtDeclaration(sem, params.position, docFsPath);
  if (cd) {
    return { contents: { kind: MarkupKind.Markdown, value: renderContainerHover(sem, cd) } };
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
    const details = renderContainerDetailsForType(sem, vd.type);
    const declaration = annotatedVariableLines(vd.name, vd.type, vd.annotations, !!vd.readonly);
    const value = ["```cpl", ...declaration, "```"].join("\n") + (details ? `\n\n${details}` : "");
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

  const docLink = sem.docLinks.find((d) => belongsToFile(d.filePath, docFsPath) && inRange(pos, d.range));
  if (docLink) {
    return locationFor(docLink.targetFilePath, docLink.targetRange, params.textDocument.uri);
  }

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
