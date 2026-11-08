import { cplHeaders, cplKeywords, cplTypes, cplConstants, cplStdlibFunctions, cplAnnotations, createSnippets } from './completion-data.js';

export function registerCompletions(monaco) {
  const cplSnippets = createSnippets(monaco);
  function completionRange(model, position) {
    const word = model.getWordUntilPosition(position);
    return {
      startLineNumber: position.lineNumber,
      endLineNumber: position.lineNumber,
      startColumn: word.startColumn,
      endColumn: word.endColumn
    };
  }

  function withCommonCompletionFields(item, range, sortPrefix) {
    const insertText = item.insertText || '';
    const usesSnippetPlaceholders = /\$\{?\d/.test(insertText);
    return {
      ...item,
      range,
      sortText: sortPrefix + item.label,
      insertTextRules: item.kind === monaco.languages.CompletionItemKind.Snippet || usesSnippetPlaceholders
        ? monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
        : item.insertTextRules
    };
  }

  monaco.languages.registerCompletionItemProvider('cpl', {
    triggerCharacters: ['#', '<', '"', '@', '[', '.', ':'],
    provideCompletionItems: function(model, position) {
      const line = model.getLineContent(position.lineNumber);
      const prefix = line.slice(0, position.column - 1);
      const includeMatch = prefix.match(/^\s*#\s*include\s*([<"])([^>"]*)$/);

      if (includeMatch) {
        const closer = includeMatch[1] === '<' ? '>' : '"';
        const typed = includeMatch[2] || '';
        const range = {
          startLineNumber: position.lineNumber,
          endLineNumber: position.lineNumber,
          startColumn: position.column - typed.length,
          endColumn: position.column
        };

        return {
          suggestions: cplHeaders.map(header => ({
            label: header,
            kind: monaco.languages.CompletionItemKind.Module,
            detail: 'cpllib header',
            documentation: '#include <' + header + '>',
            insertText: header + closer,
            range,
            sortText: '0' + header
          }))
        };
      }

      const annotationMatch = prefix.match(/(@\[?[A-Za-z_]*)$/);
      if (annotationMatch) {
        const range = {
          startLineNumber: position.lineNumber,
          endLineNumber: position.lineNumber,
          startColumn: position.column - annotationMatch[1].length,
          endColumn: position.column
        };

        return {
          suggestions: cplAnnotations.map(item => withCommonCompletionFields({
            ...item,
            kind: monaco.languages.CompletionItemKind.Property
          }, range, '0'))
        };
      }

      const range = completionRange(model, position);
      const suggestions = [
        ...cplSnippets.map(item => withCommonCompletionFields(item, range, '0')),
        ...cplAnnotations.map(item => withCommonCompletionFields({
          ...item,
          kind: monaco.languages.CompletionItemKind.Property
        }, range, '1')),
        ...cplKeywords.map(label => ({
          label,
          kind: monaco.languages.CompletionItemKind.Keyword,
          insertText: label,
          detail: 'CPL keyword',
          range,
          sortText: '2' + label
        })),
        ...cplTypes.map(label => ({
          label,
          kind: monaco.languages.CompletionItemKind.TypeParameter,
          insertText: label,
          detail: 'CPL type',
          range,
          sortText: '3' + label
        })),
        ...cplStdlibFunctions.map(item => withCommonCompletionFields({
          ...item,
          kind: monaco.languages.CompletionItemKind.Function
        }, range, '4')),
        ...cplConstants.map(label => ({
          label,
          kind: monaco.languages.CompletionItemKind.Constant,
          insertText: label,
          detail: 'cpllib constant',
          range,
          sortText: '5' + label
        })),
        ...cplHeaders.map(header => ({
          label: '#include <' + header + '>',
          kind: monaco.languages.CompletionItemKind.Module,
          insertText: '#include <' + header + '>',
          detail: 'cpllib header',
          range,
          sortText: '6' + header
        }))
      ];

      return { suggestions };
    }
  });

}
