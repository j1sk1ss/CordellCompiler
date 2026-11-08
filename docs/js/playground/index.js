import { ensureMonaco } from './monaco.js';
import { registerCplLanguage } from './language.js';
import { renderCplRunnerMarkup } from './markup.js';
import { simpleFormat } from './format.js';
import { executeCpl } from './backend.js';

const editors = new Map();
let revision = 0;

function disposeRunners() {
  revision += 1;
  for (const { editor, observer } of editors.values()) {
    observer.disconnect();
    const model = editor.getModel();
    editor.dispose();
    model.dispose();
  }
  editors.clear();
}

function decodeCode(value) {
  try {
    return decodeURIComponent(value || '');
  } catch (e) {
    return value || '';
  }
}

function hydrateRunnerFallbacks(root) {
  const scope = root || document;
  scope.querySelectorAll('pre[data-lang="cpl-run"], pre > code.language-cpl-run').forEach(node => {
    const pre = node.tagName === 'PRE' ? node : node.closest('pre');
    if (!pre || pre.dataset.runnerHydrated === '1') return;
    pre.dataset.runnerHydrated = '1';

    const codeNode = pre.querySelector('code');
    const code = codeNode ? codeNode.textContent : pre.textContent;
    const holder = document.createElement('div');
    holder.innerHTML = renderCplRunnerMarkup(code || '');
    pre.replaceWith(holder.firstElementChild);
  });
}

function hydrateRunners(root) {
  const scope = root || document;
  hydrateRunnerFallbacks(scope);
  scope.querySelectorAll('.cpl-runner').forEach(block => {
    bindRunnerActions(block);
    initRunner(block);
  });
}

async function initRunner(block) {
  if (!block || block.dataset.monacoReady === '1') return;
  block.dataset.monacoReady = '1';

  const editorMount = block.querySelector('.cpl-runner-editor');
  const output = block.querySelector('.cpl-runner-output');
  const code = decodeCode(block.dataset.code || '');
  const startedAt = revision;

  try {
    const monaco = await ensureMonaco();
    if (startedAt !== revision || !block.isConnected) return;
    registerCplLanguage(monaco);

    const model = monaco.editor.createModel(code, 'cpl');
    const editor = monaco.editor.create(editorMount, {
      model,
      theme: 'cpl-light',
      automaticLayout: true,
      minimap: { enabled: false },
      scrollBeyondLastLine: false,
      fontSize: 14,
      lineHeight: 22,
      tabSize: 2,
      insertSpaces: false,
      detectIndentation: false,
      wordWrap: 'on',
      quickSuggestions: true,
      suggestOnTriggerCharacters: true,
      tabCompletion: 'on',
      snippetSuggestions: 'inline',
      acceptSuggestionOnEnter: 'on',
      formatOnPaste: true,
      formatOnType: true,
      roundedSelection: false,
      padding: { top: 12, bottom: 12 }
    });

    const resizeObserver = new ResizeObserver(() => editor.layout());
    resizeObserver.observe(block);
    editors.set(block, { editor, observer: resizeObserver });

    output.textContent = 'Click \'Run\' to compile and run the program.';
  } catch (err) {
    output.classList.add('is-error');
    output.textContent = 'Editor init failed: ' + err.message;
  }
}

function bindRunnerActions(block) {
  if (!block || block.dataset.bound === '1') return;
  block.dataset.bound = '1';

  const formatButton = block.querySelector('[data-action="format"]');
  const runButton = block.querySelector('[data-action="run"]');

  if (formatButton) {
    formatButton.addEventListener('click', function() {
      const editor = editors.get(block)?.editor;
      if (!editor) return;
      editor.setValue(simpleFormat(editor.getValue()));
    });
  }

  if (runButton) {
    runButton.addEventListener('click', function() {
      runCpl(block);
    });
  }
}

async function runCpl(block) {
  const output = block.querySelector('.cpl-runner-output');
  const button = block.querySelector('[data-action="run"]');
  const editor = editors.get(block)?.editor;
  if (!output || !button || !editor) return;
  button.disabled = true;
  output.classList.remove('is-error');
  output.textContent = 'Running...';
  try {
    output.textContent = await executeCpl({ code: editor.getValue(), block, editor });
  } catch (error) {
    output.classList.add('is-error');
    output.textContent = 'Run failed: ' + error.message;
  } finally {
    button.disabled = false;
  }
}

export function playgroundPlugin(hook) {
  hook.beforeEach(content => {
    disposeRunners();
    return content;
  });
  hook.doneEach(() => hydrateRunners(document));
  document.addEventListener('cpl:content-ready', event => {
    hydrateRunners(event.detail?.root || document);
  });
}
