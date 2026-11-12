export function simpleFormat(value) {
  const lines = value.replace(/\r\n/g, '\n').split('\n');
  let indent = 0;
  return lines.map(line => {
    const trimmed = line.trim();
    if (!trimmed) return '';
    if (/^[}\]]/.test(trimmed)) indent = Math.max(indent - 1, 0);
    const result = '\t'.repeat(indent) + trimmed;
    const opens = (trimmed.match(/[\{\[]/g) || []).length;
    const closes = (trimmed.match(/[\}\]]/g) || []).length;
    indent = Math.max(indent + opens - closes, 0);
    return result;
  }).join('\n');
}
