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
  MarkupKind
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
  FuncOverloadSym
} from "./cplSemantics";

const connection = createConnection(ProposedFeatures.all);
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

const semByUri = new Map<string, SemanticContext>();

connection.onInitialize((_params: InitializeParams): InitializeResult => {
  return {
    capabilities: {
      textDocumentSync: TextDocumentSyncKind.Incremental,
      hoverProvider: true
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
  rootDocFsPath?: string
): IncludeResolver {
  return (includePath, fromFilePath) => {
    const inc = includePath.replace(/\\/g, "/");

    const baseDir =
      fromFilePath ? path.dirname(fromFilePath)
      : rootDocFsPath ? path.dirname(rootDocFsPath)
      : process.cwd();

    const resolved = path.isAbsolute(inc) ? inc : path.resolve(baseDir, inc);

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

connection.onHover((params) => {
  const sem = semByUri.get(params.textDocument.uri);
  if (!sem) return null;

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

documents.onDidChangeContent((change) => validateTextDocument(change.document));
documents.onDidOpen((e) => validateTextDocument(e.document));
documents.onDidSave((e) => validateTextDocument(e.document));

documents.listen(connection);
connection.listen();