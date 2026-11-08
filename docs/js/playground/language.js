import { registerCompletions } from './completions.js';

let registered = false;

export function registerCplLanguage(monaco) {
  if (registered) return;
  registered = true;

  monaco.languages.register({ id: 'cpl' });

  monaco.languages.setMonarchTokensProvider('cpl', {
    tokenizer: {
      root: [
        [/:\//, 'comment', '@comment_block_slash'],
        [/:(?=\s|$)/, 'comment', '@comment_block_colon'],
        [/:([^:\n]|\\:)*:/, 'comment'],
        [/"([^"\\]|\\.)*$/, 'string.invalid'],
        [/"/, 'string', '@string_double'],
        [/'([^'\\]|\\.)*$/, 'string.invalid'],
        [/'/, 'string', '@string_single'],
        [/\b(f64|f32|i64|i32|i16|i8|u64|u32|u16|u8|i0|container|arr|dref|ref|ptr)\b/, 'type'],
        [/\b(as|extern|start|exit|function|return|if|else|while|switch|case|default|syscall|asm|not|ro|glob|lis|break|loop)\b/, 'keyword'],
        [/\b(0[xX][0-9a-fA-F]+|0[bB][01]+|0[0-7]+|[0-9]*\.?[0-9]+([eE][+-]?[0-9]+)?)\b/, 'number'],
        [/[a-zA-Z_][\w]*/, 'identifier'],
        [/[{}()\[\]]/, '@brackets'],
        [/[-+*/%=!<>|&.,;]/, 'delimiter']
      ],
      comment_block_slash: [
        [/\/:/, 'comment', '@pop'],
        [/./, 'comment'],
        [/\n/, 'comment']
      ],
      comment_block_colon: [
        [/^:/, 'comment', '@pop'],
        [/./, 'comment'],
        [/\n/, 'comment']
      ],
      string_double: [
        [/[^\\"]+/, 'string'],
        [/\\./, 'string.escape'],
        [/"/, 'string', '@pop']
      ],
      string_single: [
        [/[^\\']+/, 'string'],
        [/\\./, 'string.escape'],
        [/'/, 'string', '@pop']
      ]
    }
  });

  monaco.languages.setLanguageConfiguration('cpl', {
    comments: { blockComment: [':', ':'] },
    brackets: [['{', '}'], ['[', ']'], ['(', ')']],
    autoClosingPairs: [
      { open: '{', close: '}' },
      { open: '[', close: ']' },
      { open: '(', close: ')' },
      { open: '"', close: '"' },
      { open: "'", close: "'" }
    ],
    surroundingPairs: [
      { open: '{', close: '}' },
      { open: '[', close: ']' },
      { open: '(', close: ')' },
      { open: '"', close: '"' },
      { open: "'", close: "'" }
    ]
  });

  registerCompletions(monaco);

  monaco.editor.defineTheme('cpl-light', {
    base: 'vs-dark',
    inherit: true,
    rules: [
      { token: 'keyword', foreground: 'f46623', fontStyle: 'bold' },
      { token: 'type', foreground: 'ffc832' },
      { token: 'comment', foreground: 'a99b8f' },
      { token: 'string', foreground: 'd6b28d' },
      { token: 'number', foreground: 'f7d774' }
    ],
    colors: {
      'editor.background': '#1f1b18',
      'editor.foreground': '#f9f2ec',
      'editorLineNumber.foreground': '#8c7f74',
      'editorLineNumber.activeForeground': '#f46623',
      'editorIndentGuide.background1': '#3a312b',
      'editor.selectionBackground': '#6b3a24',
      'editor.inactiveSelectionBackground': '#3a2a22'
    }
  });
}
