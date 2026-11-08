import { initTabs } from '../core/tabs.js';
import { copyText } from '../core/clipboard.js';

export function initCompilerInspector() {
  const inspector = document.querySelector('.compiler-inspector');
  if (!inspector || inspector.dataset.ready === 'true') return;
  inspector.dataset.ready = 'true';
  const tabs = Array.from(inspector.querySelectorAll('[data-compiler-stage]'));
  const panels = Array.from(inspector.querySelectorAll('.emission-panel'));
  const command = inspector.querySelector('#emission-command');
  const copy = inspector.querySelector('.copy-command');
  let copyTimer;
  function selectStage(tab) {
    panels.forEach(panel => { panel.hidden = panel.id !== tab.getAttribute('aria-controls'); });
    command.textContent = document.getElementById(tab.getAttribute('aria-controls')).dataset.command;
    clearTimeout(copyTimer);
    copy.textContent = 'Copy ⧉';
  }
  initTabs(tabs, selectStage);
  copy.addEventListener('click', async () => {
    const copied = await copyText(command.textContent, command);
    copy.textContent = copied ? 'Copied ✓' : 'Selected';
    clearTimeout(copyTimer);
    copyTimer = setTimeout(() => { copy.textContent = 'Copy ⧉'; }, 2200);
  });
}
