import { cleanRoute, isHomeRoute } from '../js/core/routes.js';
import { escapeHtml } from '../js/core/html.js';
import { initTabs } from '../js/core/tabs.js';
import { copyText } from '../js/core/clipboard.js';
import { highlightCpl } from '../js/syntax/highlight.js';
import { expandHomeIncludes } from '../js/docsify/content.js';
import { executeCpl } from '../js/playground/backend.js';

const results = document.querySelector('#results');
let failures = 0;
function assert(condition, message) {
  if (!condition) throw new Error(message);
}
async function check(name, run) {
  const row = document.createElement('li');
  results.appendChild(row);
  try {
    await run();
    row.textContent = `PASS: ${name}`;
  } catch (error) {
    failures += 1;
    row.textContent = `FAIL: ${name} — ${error.message}`;
    console.error(error);
  }
}

await check('Home route aliases and documentation routes', () => {
  for (const route of ['', '#', '#/', '#/README', '#/README.md', '#/?id=language-history']) {
    assert(isHomeRoute(route), `Home alias rejected: ${route}`);
  }
  assert(!isHomeRoute('#/pages/types'), 'Documentation treated as home');
  assert(cleanRoute('#/pages/types/?id=types') === '#/pages/types', 'Query or trailing slash retained');
});

await check('HTML and CPL source stay escaped', () => {
  assert(escapeHtml('<>&"\'') === '&lt;&gt;&amp;&quot;&#39;', 'HTML escaping changed');
  const holder = document.createElement('div');
  const code = 'start() { : note : exit 18; }\n"<img src=x onerror=alert(1)>"';
  holder.innerHTML = highlightCpl(code);
  assert(holder.textContent === code, 'Highlighter changed source text');
  assert(!holder.querySelector('img'), 'Source markup was executed');
  assert(holder.querySelectorAll('.token.keyword').length === 2, 'Missing CPL keywords');
  assert(holder.querySelector('.token.number').textContent === '18', 'Missing numeric token');
});

await check('Tabs support clicks, keyboard selection and roving focus', () => {
  const holder = document.createElement('div');
  holder.innerHTML = '<button role="tab" aria-selected="true">A</button><button role="tab">B</button><button role="tab">C</button>';
  document.body.appendChild(holder);
  try {
    const tabs = Array.from(holder.children);
    let selected;
    initTabs(tabs, tab => { selected = tab; });
    tabs[1].click();
    assert(selected === tabs[1], 'Click did not change selection');
    tabs[1].dispatchEvent(new KeyboardEvent('keydown', { key: 'End', bubbles: true }));
    assert(selected === tabs[2] && document.activeElement === tabs[2], 'End did not select/focus last tab');
    tabs[2].dispatchEvent(new KeyboardEvent('keydown', { key: 'ArrowRight', bubbles: true }));
    assert(selected === tabs[0], 'Right arrow did not wrap');
    tabs[0].dispatchEvent(new KeyboardEvent('keydown', { key: 'ArrowLeft', bubbles: true }));
    assert(selected === tabs[2], 'Left arrow did not wrap');
    tabs[2].dispatchEvent(new KeyboardEvent('keydown', { key: 'Home', bubbles: true }));
    assert(selected === tabs[0], 'Home did not select first tab');
    assert(tabs.filter(tab => tab.tabIndex === 0).length === 1, 'More than one tabbable tab');
    assert(tabs.filter(tab => tab.getAttribute('aria-selected') === 'true').length === 1, 'More than one selected tab');
  } finally {
    holder.remove();
  }
});

await check('Denied clipboard falls back to selecting the complete text', async () => {
  const clipboard = Object.getOwnPropertyDescriptor(navigator, 'clipboard');
  const execCommand = document.execCommand;
  const holder = document.createElement('code');
  holder.textContent = 'cplc trace.cpl --emit-ast';
  document.body.appendChild(holder);
  try {
    Object.defineProperty(navigator, 'clipboard', { configurable: true, value: {
      writeText: () => Promise.reject(new Error('Denied'))
    } });
    document.execCommand = () => false;
    assert(!await copyText(holder.textContent, holder), 'Clipboard failure reported as success');
    assert(window.getSelection().toString() === holder.textContent, 'Fallback omitted text');
  } finally {
    if (clipboard) Object.defineProperty(navigator, 'clipboard', clipboard);
    else delete navigator.clipboard;
    document.execCommand = execCommand;
    window.getSelection().removeAllRanges();
    holder.remove();
  }
});

await check('Home composition resolves all nested sections and drawings', async () => {
  const manifest = await fetch(new URL('../README.md', import.meta.url)).then(response => response.text());
  const html = await expandHomeIncludes(manifest);
  assert(!html.includes('<!-- include:'), 'Unresolved nested include');
  const holder = document.createElement('div');
  holder.innerHTML = html;
  assert(holder.querySelectorAll('.milestone-card').length === 5, 'Missing language milestone');
  assert(holder.querySelectorAll('.emission-panel').length === 4, 'Missing compiler output');
  assert(!holder.querySelector('.optimization-demos'), 'Removed demos returned');
});

await check('Home includes reject traversal, cycles and missing files', async () => {
  for (const [content, ancestors] of [
    ['<!-- include:home/../../index.html -->', []],
    ['<!-- include:home/hero.html -->', ['home/hero.html']],
    ['<!-- include:home/does-not-exist.html -->', []]
  ]) {
    let rejected = false;
    try { await expandHomeIncludes(content, ancestors); } catch (error) { rejected = true; }
    assert(rejected, `Unsafe or missing partial accepted: ${content}`);
  }
});

await check('Runner supports custom adapters, backend JSON, stubs and failures', async () => {
  const original = {
    fetch: window.fetch,
    runner: window.CPL_RUNNER,
    url: window.CPL_RUNNER_URL,
    stub: window.CPL_RUNNER_STUB
  };
  const input = { code: 'start() { exit 0; }', block: {}, editor: {} };
  try {
    window.CPL_RUNNER = { run: async payload => {
      assert(payload.lang === 'cpl' && payload.code === input.code, 'Adapter payload changed');
      assert(payload.block === input.block && payload.editor === input.editor, 'Adapter context lost');
      return { stdout: 'stdout', stderr: 'stderr' };
    } };
    assert(await executeCpl(input) === 'stdout\nstderr', 'Custom result formatting changed');
    window.CPL_RUNNER = null;
    window.CPL_RUNNER_URL = '/module-test/run';
    window.fetch = async (url, options) => {
      assert(url === '/module-test/run', 'Configured backend ignored');
      assert(JSON.parse(options.body).code === input.code, 'Backend source changed');
      assert(options.method === 'POST', 'Backend method changed');
      return new Response(JSON.stringify({ stdout: 'done' }), { status: 200 });
    };
    assert(await executeCpl(input) === 'done', 'Backend result omitted');
    let stubCalls = 0;
    window.CPL_RUNNER_STUB = async () => { stubCalls += 1; return { stdout: 'stub' }; };
    window.fetch = async () => new Response('', { status: 404 });
    assert(await executeCpl(input) === 'stub', 'Missing endpoint did not use stub');
    window.fetch = async () => { throw new Error('Offline'); };
    assert(await executeCpl(input) === 'stub' && stubCalls === 2, 'Network error did not use stub');
    window.fetch = async () => new Response(JSON.stringify({ error: 'Compilation failed' }), { status: 500 });
    let failure;
    try { await executeCpl(input); } catch (error) { failure = error; }
    assert(failure?.message === 'Compilation failed', 'Backend failure was hidden');
    assert(stubCalls === 2, 'Backend failure incorrectly used stub');
  } finally {
    window.fetch = original.fetch;
    for (const [name, value] of Object.entries({ CPL_RUNNER: original.runner, CPL_RUNNER_URL: original.url, CPL_RUNNER_STUB: original.stub })) {
      if (value === undefined) delete window[name];
      else window[name] = value;
    }
  }
});

document.documentElement.dataset.testStatus = failures ? 'failed' : 'passed';
document.querySelector('#status').textContent = failures ? `${failures} checks failed.` : 'All module checks passed.';
