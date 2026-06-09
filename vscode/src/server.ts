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
  CompletionItemKind,
  InsertTextFormat,
  FileChangeType
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
  formatAnnotations,
  FuncOverloadSym,
  ContainerSym,
  sizeofType
} from "./cplSemantics";

const connection = createConnection(ProposedFeatures.all);
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

const semByUri = new Map<string, SemanticContext>();
let workspaceFolderPaths: string[] = [];

connection.onInitialize((params: InitializeParams): InitializeResult => {
  workspaceFolderPaths = (params.workspaceFolders ?? [])
    .map((f) => uriToFsPath(f.uri))
    .filter((p): p is string => !!p);

  const root = params.rootUri ? uriToFsPath(params.rootUri) : undefined;
  if (root && !workspaceFolderPaths.includes(root)) workspaceFolderPaths.push(root);

  return {
    capabilities: {
      textDocumentSync: TextDocumentSyncKind.Incremental,
      hoverProvider: true,
      completionProvider: {
        triggerCharacters: ["@", "[", "(", "."]
      },
      workspace: {
        workspaceFolders: { supported: true }
      }
    }
  };
});

type IncludeResolverResult = { text: string; filePath: string };
type IncludeResolver = (includePath: string, fromFilePath?: string) => IncludeResolverResult | undefined;

function uriToFsPath(uri: string): string | undefined {
  try {
    if (uri.startsWith("file://")) return fileURLToPath(uri);
  } catch {}
  return undefined;
}

function makeIncludeResolver(
  documents: TextDocuments<TextDocument>,
  rootDocFsPath?: string,
  workspaceRoots: string[] = []
): IncludeResolver {
  return (includePath, fromFilePath) => {
    const inc = includePath.replace(/\\/g, "/");

    const baseDir =
      fromFilePath ? path.dirname(fromFilePath)
      : rootDocFsPath ? path.dirname(rootDocFsPath)
      : process.cwd();

    const bases = [baseDir, ...workspaceRoots].filter((v, i, a) => v && a.indexOf(v) === i);
    const rawCandidates = path.isAbsolute(inc)
      ? [inc]
      : bases.map((base) => path.resolve(base, inc));

    const candidates: string[] = [];
    for (const c of rawCandidates) {
      candidates.push(c);
      if (!path.extname(c)) {
        candidates.push(`${c}.cpl`, `${c}_h.cpl`, `${c}.h`);
      }
    }

    for (const resolved of candidates.filter((v, i, a) => a.indexOf(v) === i)) {
      try {
        const uri = pathToFileURL(resolved).toString();
        const openDoc = documents.get(uri);
        if (openDoc) return { text: openDoc.getText(), filePath: resolved };
      } catch {}

      try {
        if (fs.existsSync(resolved)) {
          const text = fs.readFileSync(resolved, "utf8");
          return { text, filePath: resolved };
        }
      } catch {}
    }

    return undefined;
  };
}

async function validateTextDocument(doc: TextDocument) {
  const text = doc.getText();

  const docFsPath = uriToFsPath(doc.uri);
  const include = makeIncludeResolver(documents, docFsPath, workspaceFolderPaths);

  const { issues, sem } = analyze(text, include, docFsPath);
  semByUri.set(doc.uri, sem);

  const diags: Diagnostic[] = issues.map((e) => ({
    severity:
      e.severity === "warning" ? DiagnosticSeverity.Warning
      : e.severity === "information" ? DiagnosticSeverity.Information
      : e.severity === "hint" ? DiagnosticSeverity.Hint
      : DiagnosticSeverity.Error,
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
    const ann = formatAnnotations(ordered[0].annotations);
    if (ann) lines.push("", `_Annotations: ${ann}_`);
    if (ordered[0].weak) lines.push("", "_Weak symbol_");
    if (ordered[0].implContainer) lines.push("", `_Container impl: ${ordered[0].implContainer}_`);
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

  if (selected) {
    const ann = formatAnnotations(selected.annotations);
    if (ann) lines.push("", `_Annotations: ${ann}_`);
    if (selected.weak) lines.push("", "_Weak symbol_");
    if (selected.implContainer) lines.push("", `_Container impl: ${selected.implContainer}_`);
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

function findFuncDeclHover(sem: SemanticContext, pos: Position): FuncOverloadSym | undefined {
  for (const list of sem.funcs.values()) {
    for (const fn of list) {
      if (inRange(pos, fn.primaryRange)) return fn;
      if (fn.def && inRange(pos, fn.def)) return fn;
      if (fn.decls.some((r) => inRange(pos, r))) return fn;
    }
  }
  return undefined;
}


function findContainerDeclHover(sem: SemanticContext, pos: Position): ContainerSym | undefined {
  for (const c of sem.containerDecls) {
    if (inRange(pos, c.range)) return c;
  }
  return undefined;
}

function renderContainer(container: ContainerSym): string {
  const lines: string[] = [];
  lines.push(`**container ${container.name}**`, "");

  const annotations = formatAnnotations(container.annotations);
  if (annotations) lines.push(`_Annotations: ${annotations}_`, "");
  if (container.isUnion) lines.push("Union layout: memory is reserved for the largest field.", "");
  if (container.likeC) lines.push("C-like field layout handling is requested.", "");

  if (container.fields.size) {
    lines.push("Fields:");
    for (const f of container.fields.values()) lines.push(`- \`${formatType(f.type)} ${f.name}\``);
    lines.push("");
  }

  const methods = [...container.methods.entries()];
  if (methods.length) {
    lines.push("Methods:");
    for (const [, overloads] of methods) {
      for (const fn of overloads) {
        const params = (fn.selfMethod || fn.params[0]?.name === "self") ? fn.params.slice(1) : fn.params;
        const callSig = `${fn.name}(${params.map((p) => p.isVarArgs ? "..." : `${formatType(p.type)} ${p.name}`).join(", ")})`;
        lines.push(`- \`${callSig}\``);
      }
    }
    lines.push("");
  }

  if (container.doc?.trim()) lines.push(container.doc);
  return lines.join("\n").trimEnd();
}

function completionAnnotations() {
  return [
    { label: "self", detail: "@[self] explicit container receiver", insertText: "self]", kind: CompletionItemKind.Property },
    { label: "impl", detail: "@[impl(Container)] implement container method; inherits prototype annotations", insertText: "impl(${1:Container})]", kind: CompletionItemKind.Property, insertTextFormat: InsertTextFormat.Snippet },
    { label: "union", detail: "@[union] union-like container layout", insertText: "union]", kind: CompletionItemKind.Property },
    { label: "weak", detail: "@[weak] weak function symbol", insertText: "weak]", kind: CompletionItemKind.Property },
    { label: "like_c", detail: "@[like_c] C-like container layout", insertText: "like_c]", kind: CompletionItemKind.Property },
    { label: "abi", detail: "@[abi] ABI-compatible function", insertText: "abi]", kind: CompletionItemKind.Property },
    { label: "inline", detail: "@[inline(always|never|model)] inline hint", insertText: "inline(${1:always})]", kind: CompletionItemKind.Property, insertTextFormat: InsertTextFormat.Snippet },
    { label: "section", detail: "@[section(\"name\")] place symbol into section", insertText: "section(\"${1:.text}\")]", kind: CompletionItemKind.Property, insertTextFormat: InsertTextFormat.Snippet },
    { label: "align", detail: "@[align(N)] alignment", insertText: "align(${1:16})]", kind: CompletionItemKind.Property, insertTextFormat: InsertTextFormat.Snippet }
  ];
}


connection.onCompletion((params) => {
  const doc = documents.get(params.textDocument.uri);
  if (!doc) return [];

  const sem = semByUri.get(params.textDocument.uri);
  const offset = doc.offsetAt(params.position);
  const before = doc.getText().slice(0, offset);

  if (/@\[\s*impl\(\s*[A-Za-z_]*$/.test(before)) {
    return [...(sem?.containers.values() ?? [])].map((c) => ({
      label: c.name,
      kind: CompletionItemKind.Struct,
      detail: `container ${c.name}`,
      insertText: c.name
    }));
  }

  if (/@\[\s*[A-Za-z_]*$/.test(before)) {
    return completionAnnotations();
  }

  const memberMatch = /([A-Za-z_]\w*)\.$/.exec(before);
  if (memberMatch && sem) {
    const baseType = sem.getVarType(memberMatch[1]);
    const members = baseType ? sem.getContainerMembersForType(baseType) : undefined;
    if (members) {
      const fieldItems = members.fields.map((f) => ({
        label: f.name,
        kind: CompletionItemKind.Field,
        detail: `${formatType(f.type)} ${members.containerName}.${f.name}`,
        insertText: f.name
      }));
      const methodItems = members.methods.map((m) => {
        const params = (m.selfMethod || m.params[0]?.name === "self") ? m.params.slice(1) : m.params;
        return {
          label: m.name,
          kind: CompletionItemKind.Method,
          detail: `${members.containerName}.${m.name}(${params.map((p) => p.isVarArgs ? "..." : `${formatType(p.type)} ${p.name}`).join(", ")})`,
          insertText: m.name
        };
      });
      return [...fieldItems, ...methodItems];
    }
  }

  if (sem) {
    return [...sem.containers.values()].map((c) => ({
      label: c.name,
      kind: CompletionItemKind.Struct,
      detail: `container ${c.name}`,
      insertText: c.name
    }));
  }

  return [];
});

connection.onHover((params) => {
  const sem = semByUri.get(params.textDocument.uri);
  if (!sem) return null;

  const sz = sem.sizeofSites.find((s) => inRange(params.position, s.range));
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

  const vu = sem.varUses.find((v) => inRange(params.position, v.range));
  if (vu) {
    const value = `\`\`\`cpl\n${formatType(vu.type)} ${vu.name}\n\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const mu = sem.macroUses.find((m) => inRange(params.position, m.range));
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

  const cs = sem.callSites.find((c) => inRange(params.position, c.range));
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

  const fu = sem.funcValueUses.find((f) => inRange(params.position, f.range));
  if (fu) {
    const overloads = sem.funcs.get(fu.name) ?? [];
    if (overloads.length > 0) {
      const value = renderOverloads(overloads, { title: `Function ${fu.name}` });
      return { contents: { kind: MarkupKind.Markdown, value } };
    }
  }

  const ics = sem.indirectCallSites.find((c) => inRange(params.position, c.range));
  if (ics) {
    const value = `\`\`\`cpl
callable ${formatType(ics.calleeType)}
\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const containerDecl = findContainerDeclHover(sem, params.position);
  if (containerDecl) {
    return { contents: { kind: MarkupKind.Markdown, value: renderContainer(containerDecl) } };
  }

  const fnDecl = findFuncDeclHover(sem, params.position);
  if (fnDecl) {
    const list = sem.funcs.get(fnDecl.name) ?? [fnDecl];
    const value = renderOverloads(list, { selected: fnDecl, title: `Function ${fnDecl.name}` });
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const vd = sem.varDecls.find((v) => inRange(params.position, v.range));
  if (vd) {
    const value = `\`\`\`cpl\n${formatType(vd.type)} ${vd.name}\n\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  return null;
});

function validateOpenDocuments() {
  for (const doc of documents.all()) void validateTextDocument(doc);
}

documents.onDidChangeContent((change) => validateTextDocument(change.document));
documents.onDidOpen((e) => validateTextDocument(e.document));
documents.onDidSave(() => validateOpenDocuments());

connection.onDidChangeWatchedFiles((params) => {
  if (params.changes.some((c) => c.type === FileChangeType.Changed || c.type === FileChangeType.Created || c.type === FileChangeType.Deleted)) {
    validateOpenDocuments();
  }
});

documents.listen(connection);
connection.listen();