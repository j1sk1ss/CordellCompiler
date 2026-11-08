import { initTabs } from '../core/tabs.js';
import { copyText } from '../core/clipboard.js';
import { animateChange } from '../core/motion.js';
import { stages, compilerCommand, renderSource, renderCompilerOutput } from './compiler-view.js';

const sampleUrl = new URL('../../assets/compiler/samples.json', import.meta.url);
let samplesPromise;
function loadSamples() {
  if (!samplesPromise) {
    samplesPromise = fetch(sampleUrl, { cache: 'no-cache' }).then(response => {
      if (!response.ok) throw new Error(`Compiler samples: ${response.status}`);
      return response.json();
    }).catch(error => {
      samplesPromise = null;
      throw error;
    });
  }
  return samplesPromise;
}

export function initCompilerInspector() {
  const inspector = document.querySelector('.compiler-inspector');
  if (!inspector || inspector.dataset.bound === 'true') return;
  inspector.dataset.bound = 'true';
  const tabs = Array.from(inspector.querySelectorAll('[data-compiler-stage]'));
  const modes = Array.from(inspector.querySelectorAll('[data-compiler-mode]'));
  const panels = Array.from(inspector.querySelectorAll('.emission-panel'));
  const command = inspector.querySelector('#emission-command');
  const source = inspector.querySelector('.source-lines');
  const status = inspector.querySelector('.compiler-load-status');
  const retry = inspector.querySelector('.compiler-retry');
  const copy = inspector.querySelector('.copy-command');
  const copySource = inspector.querySelector('.copy-source');
  let samples;
  let modeId = 'none';
  let stageId = 'ast';
  let copyTimer;
  let sourceCopyTimer;

  function render(animate = true) {
    if (!samples) return;
    const baseline = samples.modes[0];
    const mode = samples.modes.find(item => item.id === modeId);
    const stage = stages[stageId];
    panels.forEach(panel => { panel.hidden = panel.id !== `output-${stageId}`; });
    modes.forEach(button => button.setAttribute('aria-pressed', String(button.dataset.compilerMode === modeId)));
    const panel = inspector.querySelector(`#output-${stageId}`);
    const drawing = panel.querySelector('.emission-drawing');
    drawing.innerHTML = renderCompilerOutput(mode, stageId, baseline);
    const download = panel.querySelector('.emission-download');
    download.href = new URL(`${mode.directory}trace.${stageId}`, sampleUrl).href;
    panel.querySelector('.emission-kind').textContent = stage.label;
    const count = mode[stageId].length;
    const originalCount = baseline[stageId].length;
    panel.querySelector('.emission-metric').textContent = `${count === originalCount ? count : `${originalCount} → ${count}`} ${stage.unit}`;
    inspector.querySelector('.trace-mode').textContent = mode.flags.join(' ');
    command.textContent = compilerCommand(mode, stageId);
    clearTimeout(copyTimer);
    copy.textContent = 'Copy ⧉';
    source.querySelectorAll('.is-linked').forEach(line => line.classList.remove('is-linked'));
    if (animate) animateChange(drawing);
  }

  initTabs(tabs, tab => {
    stageId = tab.dataset.compilerStage;
    render();
  });
  modes.forEach(button => button.addEventListener('click', () => {
    modeId = button.dataset.compilerMode;
    render();
  }));
  copy.addEventListener('click', async () => {
    const copied = await copyText(command.textContent, command);
    copy.textContent = copied ? 'Copied ✓' : 'Selected';
    clearTimeout(copyTimer);
    copyTimer = setTimeout(() => { copy.textContent = 'Copy ⧉'; }, 2200);
  });
  copySource.addEventListener('click', async () => {
    const copied = await copyText(samples.source, source);
    copySource.textContent = copied ? 'Copied ✓' : 'Selected';
    clearTimeout(sourceCopyTimer);
    sourceCopyTimer = setTimeout(() => { copySource.textContent = 'Copy ⧉'; }, 2200);
  });

  function linkSource(event) {
    const step = event.target.closest('[data-source-line]');
    if (!step) return;
    source.querySelectorAll('[data-line]').forEach(line => {
      line.classList.toggle('is-linked', line.dataset.line === step.dataset.sourceLine);
    });
  }
  inspector.addEventListener('pointerover', linkSource);
  inspector.addEventListener('focusin', linkSource);
  inspector.querySelector('.compiler-outputs').addEventListener('pointerleave', () => {
    if (!inspector.querySelector('.compiler-outputs').contains(document.activeElement)) {
      source.querySelectorAll('.is-linked').forEach(line => line.classList.remove('is-linked'));
    }
  });

  async function load() {
    inspector.setAttribute('aria-busy', 'true');
    retry.hidden = true;
    status.hidden = false;
    status.textContent = 'Loading compiler output…';
    try {
      samples = await loadSamples();
      if (!inspector.isConnected) return;
      source.innerHTML = renderSource(samples.source);
      inspector.querySelector('.source-exit-value').textContent = samples.exit;
      inspector.querySelectorAll('button:not(.compiler-retry)').forEach(button => { button.disabled = false; });
      render(false);
      status.hidden = true;
      inspector.dataset.ready = 'true';
    } catch (error) {
      status.textContent = 'Could not load compiler output.';
      retry.hidden = false;
    } finally {
      inspector.setAttribute('aria-busy', 'false');
    }
  }
  retry.addEventListener('click', load);
  load();
}
