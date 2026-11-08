import { initHomeExamples } from './examples.js';
import { initCompilerInspector } from './compiler.js';
import { initLanguageHistory } from './history.js';

export function initHome() {
  const home = document.querySelector('.cpl-home');
  if (!home || home.dataset.ready === 'true') return;
  initHomeExamples();
  initCompilerInspector();
  initLanguageHistory();
  home.dataset.ready = 'true';
}
