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
import { SemanticContext, formatType } from "./cplSemantics";

const connection = createConnection(ProposedFeatures.all);
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

const semByUri = new Map<string, SemanticContext>();

connection.onInitialize((_params: InitializeParams): InitializeResult => {
  const result: InitializeResult = {
    capabilities: {
      textDocumentSync: TextDocumentSyncKind.Incremental,
      hoverProvider: true
    }
  };
  return result;
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

  const diags: Diagnostic[] = issues.map(e => ({
    severity: DiagnosticSeverity.Error,
    range: e.range,
    message: e.message,
    source: "cpl-ls"
  }));

  connection.sendDiagnostics({ uri: doc.uri, diagnostics: diags });
}

function inRange(pos: Position, r: any) {
  if (pos.line < r.start.line || pos.line > r.end.line) return false;
  if (pos.line === r.start.line && pos.character < r.start.character) return false;
  if (pos.line === r.end.line && pos.character > r.end.character) return false;
  return true;
}

connection.onHover((params) => {
  const sem = semByUri.get(params.textDocument.uri);
  if (!sem) return null;

  const vu = sem.varUses.find(v => inRange(params.position, v.range));
  if (vu) {
    const value = `\`\`\`cpl\n${formatType(vu.type)} ${vu.name}\n\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  const cs = sem.callSites.find(c => inRange(params.position, c.range));
  if (cs) {
    const fn = sem.funcs.get(cs.name);
    if (fn) {
      const paramsStr = fn.params
        .map(p => `${formatType(p.type)} ${p.name}${p.hasDefault ? " = <default>" : ""}`)
        .join(", ");
      const retStr = formatType(fn.ret);
      const sig = retStr === "i0"
        ? `function ${fn.name}(${paramsStr})`
        : `function ${fn.name}(${paramsStr}) => ${retStr}`;
    
      let value = `\`\`\`cpl\n${sig}\n\`\`\``;
    
      if (fn.doc && fn.doc.trim()) {
        value += `\n\n${fn.doc}`;
      }
    
      return { contents: { kind: MarkupKind.Markdown, value } };
    }    
  }

  const vd = sem.varDecls.find(v => inRange(params.position, v.range));
  if (vd) {
    const value = `\`\`\`cpl\n${formatType(vd.type)} ${vd.name}\n\`\`\``;
    return { contents: { kind: MarkupKind.Markdown, value } };
  }

  return null;
});
  
documents.onDidChangeContent(change => validateTextDocument(change.document));
documents.onDidOpen(e => validateTextDocument(e.document));
documents.onDidSave(e => validateTextDocument(e.document));

documents.listen(connection);
connection.listen();
  