import { initTabs } from '../core/tabs.js';
import { copyText } from '../core/clipboard.js';
import { animateChange } from '../core/motion.js';
import { highlightCpl } from '../syntax/highlight.js';
import { escapeHtml } from '../core/html.js';
import { analyzerCases } from './analyzer-cases.js';
import { analyzerDrawings } from './analyzer-drawings.js';
import { advancedAnalyzerCases } from './analyzer-advanced-cases.js';
import { advancedAnalyzerDrawings } from './analyzer-advanced-drawings.js';

const cases = { ...analyzerCases, ...advancedAnalyzerCases };
const drawings = { ...analyzerDrawings, ...advancedAnalyzerDrawings };
const groups = {
  advanced: Object.keys(advancedAnalyzerCases),
  basic: Object.keys(analyzerCases)
};

export function initAnalyzer() {
  const section = document.querySelector('.csa-section');
  if (!section || section.dataset.ready === 'true') return;
  const panel = section.querySelector('#csa-panel');
  const code = section.querySelector('[data-csa-code]');
  const drawing = section.querySelector('[data-csa-drawing]');
  const copy = section.querySelector('.csa-copy-command');
  const tabs = section.querySelector('.csa-tabs');
  const variantButton = section.querySelector('[data-csa-variant]');
  const trace = section.querySelector('[data-csa-trace]');
  let group = 'advanced';
  let selectedCase;
  let variant = false;
  let selectedOnce = false;
  let copyTimer;

  function renderExample() {
    const original = cases[selectedCase];
    const example = variant ? { ...original, ...original.variant } : original;
    panel.setAttribute('aria-labelledby', `csa-tab-${selectedCase}`);
    panel.dataset.case = selectedCase;
    panel.dataset.variant = String(variant);
    code.innerHTML = example.code.trimEnd().split('\n').map((line, index) =>
      `<div class="csa-code-line${index + 1 === example.line ? ' csa-code-hit' : ''}" data-csa-source-line="${index + 1}"><span class="csa-line-number" aria-hidden="true">${index + 1}</span><code>${highlightCpl(line) || ' '}</code></div>`
    ).join('');
    code.scrollTop = 0;
    const download = section.querySelector('[data-csa-download]');
    download.textContent = `${example.file} ↓`;
    download.href = `assets/csa/${example.file}`;
    section.querySelector('[data-csa-test]').href = `https://github.com/j1sk1ss/CordellCompiler/blob/dev/tests/code_utesting/csa/${example.test}`;
    section.querySelector('[data-csa-phase]').textContent = example.phase;
    drawing.innerHTML = drawings[example.drawing || selectedCase];
    const finding = section.querySelector('[data-csa-finding]');
    finding.dataset.severity = example.severity;
    section.querySelector('[data-csa-severity]').textContent = example.severity.toUpperCase();
    section.querySelector('[data-csa-location]').textContent = `${example.file}:${example.line}:${example.column}`;
    section.querySelector('[data-csa-message]').textContent = example.message;
    section.querySelector('[data-csa-explanation]').textContent = example.explanation;
    trace.hidden = !example.trace?.length;
    trace.innerHTML = example.trace?.length ? `<p class="csa-trace-label">FOLLOW THE SOURCE</p><ul>${example.trace.map(step =>
      `<li><button type="button" data-csa-line="${step.line}" title="Highlight source line ${step.line}"><span class="csa-trace-kind">${escapeHtml(step.kind)}</span> <span>${escapeHtml(step.text)}</span> <small>L${step.line}</small></button></li>`
    ).join('')}</ul>` : '';
    variantButton.hidden = !original.variant;
    variantButton.setAttribute('aria-pressed', String(variant));
    variantButton.textContent = variant ? 'Restore unguarded call ↶' : 'Keep only the guarded call ↗';
    if (selectedOnce) animateChange(panel);
    selectedOnce = true;
  }

  function renderGroup() {
    section.querySelectorAll('[data-csa-level]').forEach(button => {
      button.setAttribute('aria-pressed', String(button.dataset.csaLevel === group));
    });
    tabs.innerHTML = groups[group].map((key, index) =>
      `<button type="button" role="tab" id="csa-tab-${key}" aria-controls="csa-panel" aria-selected="${index === 0}" tabindex="${index === 0 ? 0 : -1}" data-csa-case="${key}">${escapeHtml(cases[key].label)}</button>`
    ).join('');
    initTabs(Array.from(tabs.querySelectorAll('[data-csa-case]')), tab => {
      selectedCase = tab.dataset.csaCase;
      variant = false;
      renderExample();
    });
  }

  section.querySelectorAll('[data-csa-level]').forEach(button => {
    button.addEventListener('click', () => {
      if (group === button.dataset.csaLevel) return;
      group = button.dataset.csaLevel;
      renderGroup();
    });
  });
  variantButton.addEventListener('click', () => {
    variant = !variant;
    renderExample();
  });

  function highlightTrace(event) {
    const step = event.target.closest('[data-csa-line]');
    if (!step) return;
    code.querySelectorAll('[data-csa-source-line]').forEach(line => {
      line.classList.toggle('csa-code-trace', line.dataset.csaSourceLine === step.dataset.csaLine);
    });
  }
  trace.addEventListener('pointerover', highlightTrace);
  trace.addEventListener('focusin', highlightTrace);
  trace.addEventListener('click', event => {
    highlightTrace(event);
    const line = code.querySelector('.csa-code-trace');
    if (line && code.scrollHeight > code.clientHeight) {
      code.scrollTop += line.getBoundingClientRect().top - code.getBoundingClientRect().top - 24;
    }
  });
  trace.addEventListener('pointerleave', () => {
    if (trace.contains(document.activeElement)) return;
    code.querySelectorAll('.csa-code-trace').forEach(line => line.classList.remove('csa-code-trace'));
  });
  renderGroup();

  copy.addEventListener('click', async () => {
    const command = section.querySelector('.csa-command code');
    const copied = await copyText(command.textContent, command);
    clearTimeout(copyTimer);
    copy.textContent = copied ? 'Copied ✓' : 'Select command';
    copyTimer = setTimeout(() => { copy.textContent = 'Copy ⧉'; }, 2200);
  });
  section.dataset.ready = 'true';
}
