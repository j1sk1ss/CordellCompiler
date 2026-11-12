import { escapeHtml } from '../core/html.js';

function spanToken(type, value) {
  return '<span class="token ' + type + '">' + escapeHtml(value) + '</span>';
}

export function highlightCpl(code) {
  const keywords = new Set([
    'as', 'break', 'case', 'default', 'defer', 'else', 'exit', 'extern', 'function',
    'glob', 'if', 'implements', 'interface', 'loop', 'not', 'place', 'return', 'ro', 'start', 'switch', 'while'
  ]);
  const types = new Set([
    'arr', 'dref', 'f32', 'f64', 'i0', 'i8', 'i16', 'i32', 'i64',
    'ptr', 'ref', 'container', 'u8', 'u16', 'u32', 'u64'
  ]);
  const builtins = new Set(['asm', 'sizeof', 'syscall']);


  let out = '';
  let i = 0;

  while (i < code.length) {
    const rest = code.slice(i);

    if (rest.startsWith(':/')) {
      const end = code.indexOf('/:', i + 2);
      const value = end === -1 ? code.slice(i) : code.slice(i, end + 2);
      out += spanToken('comment', value);
      i += value.length;
      continue;
    }

    if (code[i] === ':' && (i === 0 || code[i - 1] === '\n' || /\s/.test(code[i - 1]))) {
      const end = code.indexOf(':', i + 1);
      const newline = code.indexOf('\n', i + 1);
      if (end !== -1 && (newline === -1 || end < newline)) {
        const value = code.slice(i, end + 1);
        out += spanToken('comment', value);
        i += value.length;
        continue;
      }
    }

    if (code[i] === '#') {
      const end = code.indexOf('\n', i + 1);
      const value = end === -1 ? code.slice(i) : code.slice(i, end);
      out += spanToken('preprocessor', value);
      i += value.length;
      continue;
    }

    if (rest.startsWith('@[')) {
      const end = code.indexOf(']', i + 2);
      const value = end === -1 ? code.slice(i) : code.slice(i, end + 1);
      out += spanToken('annotation', value);
      i += value.length;
      continue;
    }

    if (code[i] === '"' || code[i] === "'") {
      const quote = code[i];
      let j = i + 1;
      while (j < code.length) {
        if (code[j] === '\\') {
          j += 2;
          continue;
        }
        if (code[j] === quote) {
          j += 1;
          break;
        }
        j += 1;
      }
      out += spanToken(quote === '"' ? 'string' : 'char', code.slice(i, j));
      i = j;
      continue;
    }

    const number = rest.match(/^(0[xX][0-9a-fA-F]+|0[bB][01]+|[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?)/);
    if (number) {
      out += spanToken('number', number[0]);
      i += number[0].length;
      continue;
    }

    const word = rest.match(/^[A-Za-z_][A-Za-z0-9_]*/);
    if (word) {
      const value = word[0];
      const next = code.slice(i + value.length).match(/^\s*\(/);
      if (keywords.has(value)) out += spanToken('keyword', value);
      else if (types.has(value)) out += spanToken('type', value);
      else if (builtins.has(value)) out += spanToken('keyword', value);
      else if (next) out += spanToken('function', value);
      else out += escapeHtml(value);
      i += value.length;
      continue;
    }

    const op = rest.match(/^(=>|->|==|!=|<=|>=|\+=|-=|\*=|\/=|%=|&&|\|\||[{}\(\)\[\],.;:+\-*\/%=&|!<>])/);
    if (op) {
      out += spanToken(/[{}()[\],.;:]/.test(op[0]) ? 'punctuation' : 'operator', op[0]);
      i += op[0].length;
      continue;
    }

    out += escapeHtml(code[i]);
    i += 1;
  }

  return out;
}

export function highlightCode(code, lang) {
  const normalized = (lang || 'markup').toLowerCase();
  if (normalized === 'cpl') return highlightCpl(code);

  if (window.Prism && window.Prism.languages && window.Prism.languages[normalized]) {
    return window.Prism.highlight(code, window.Prism.languages[normalized], normalized);
  }

  return escapeHtml(code);
}
