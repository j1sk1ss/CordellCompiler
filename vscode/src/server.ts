import {
    createConnection,
    TextDocuments,
    Diagnostic,
    DiagnosticSeverity,
    InitializeParams,
    InitializeResult,
    ProposedFeatures,
    TextDocumentSyncKind
} from "vscode-languageserver/node";

import { TextDocument } from "vscode-languageserver-textdocument";
import { parseAndDiagnose } from "./cplParser";

const connection = createConnection(ProposedFeatures.all);
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

connection.onInitialize((_params: InitializeParams): InitializeResult => {
const result: InitializeResult = {
    capabilities: {
    textDocumentSync: TextDocumentSyncKind.Incremental
    }
};
return result;
});

async function validateTextDocument(doc: TextDocument) {
const text = doc.getText();
const diags: Diagnostic[] = parseAndDiagnose(text).map(e => ({
    severity: DiagnosticSeverity.Error,
    range: e.range,
    message: e.message,
    source: "cpl-ls"
}));
connection.sendDiagnostics({ uri: doc.uri, diagnostics: diags });
}

documents.onDidChangeContent(change => validateTextDocument(change.document));
documents.onDidOpen(e => validateTextDocument(e.document));
documents.onDidSave(e => validateTextDocument(e.document));

documents.listen(connection);
connection.listen();
  