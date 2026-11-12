import { animateChange } from '../core/motion.js';
import { homeExamples } from './examples-data.js';
import { highlightCpl } from '../syntax/highlight.js';
import { escapeHtml } from '../core/html.js';
import { initTabs } from '../core/tabs.js';
import { copyText } from '../core/clipboard.js';

export function initHomeExamples() {
  const home = document.querySelector('.code-example');
  if (!home || home.dataset.ready === 'true') return;
  home.dataset.ready = 'true';
  const tablist = home.querySelector('.example-tabs');
  tablist.innerHTML = Object.entries(homeExamples).map(([key, example], index) =>
    `<button class="example-tab" id="tab-${key}" role="tab" aria-controls="example-panel" aria-selected="${index === 0}" tabindex="${index === 0 ? 0 : -1}" data-example="${key}" type="button">${escapeHtml(example.label)}</button>`
  ).join('');
  const tabs = Array.from(tablist.querySelectorAll('[data-example]'));
  const code = home.querySelector('#home-example-code');
  const panel = home.querySelector('#example-panel');
  const caption = home.querySelector('#example-explanation');
  const filename = home.querySelector('#example-filename');
  const copy = home.querySelector('.copy-example');
  let activeExample = 'start';
  let copyTimer;
  let selectedOnce = false;

  function selectExample(tab) {
    activeExample = tab.dataset.example;
    panel.setAttribute('aria-labelledby', tab.id);
    code.innerHTML = highlightCpl(homeExamples[activeExample].code);
    caption.textContent = `Exit code: ${homeExamples[activeExample].exit}`;
    filename.textContent = homeExamples[activeExample].file;
    clearTimeout(copyTimer);
    copy.innerHTML = 'Copy <span aria-hidden="true">⧉</span>';
    if (selectedOnce) animateChange(code.parentElement);
    selectedOnce = true;
  }

  initTabs(tabs, selectExample);
  copy.addEventListener('click', async () => {
    const value = homeExamples[activeExample].code;
    const copied = await copyText(value, code);
    copy.textContent = copied ? 'Copied ✓' : 'Select code to copy';
    clearTimeout(copyTimer);
    copyTimer = setTimeout(() => { copy.innerHTML = 'Copy <span aria-hidden="true">⧉</span>'; }, 2200);
  });
}
