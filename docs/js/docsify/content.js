import { isHomeRoute } from '../core/routes.js';

const partialRoot = new URL('../../partials/', import.meta.url);
const partials = new Map();

function loadPartial(path) {
  if (!/^home\/[a-z0-9/-]+\.html$/.test(path)) {
    return Promise.reject(new Error(`Invalid home partial: ${path}`));
  }
  if (!partials.has(path)) {
    const request = fetch(new URL(path, partialRoot)).then(response => {
      if (!response.ok) throw new Error(`Cannot load ${path}: ${response.status}`);
      return response.text();
    }).catch(error => {
      partials.delete(path);
      throw error;
    });
    partials.set(path, request);
  }
  return partials.get(path);
}

export async function expandHomeIncludes(content, ancestors = []) {
  const pattern = /<!-- include:([a-z0-9/.-]+) -->/g;
  const matches = Array.from(content.matchAll(pattern));
  const fragments = await Promise.all(matches.map(async match => {
    const path = match[1];
    if (ancestors.includes(path)) throw new Error(`Circular home include: ${path}`);
    return expandHomeIncludes(await loadPartial(path), [...ancestors, path]);
  }));
  let index = 0;
  return content.replace(pattern, () => fragments[index++]);
}

export function homeContentPlugin(hook) {
  hook.beforeEach((content, next) => {
    if (!isHomeRoute()) return next(content);
    expandHomeIncludes(content).then(html => {
      // Docsify parses Markdown: blank lines inside an HTML block end that block.
      // Keep partial source readable, then compact only whitespace-only lines.
      next(html.split('\n').filter(line => line.trim()).join('\n'));
    }).catch(error => {
      console.error(error);
      next('<p role="alert">Could not load the home page. Reload to retry.</p>');
    });
  });
}
