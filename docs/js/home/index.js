import { initHomeMotion } from './motion.js';
import { initHomeExamples } from './examples.js';
import { initCompilerInspector } from './compiler.js';
import { initLanguageHistory } from './history.js';
import { initLatestRelease } from './release.js';
import { initAnalyzer } from './analyzer.js';

export function initHome() {
  const home = document.querySelector('.cpl-home');
  if (!home || home.dataset.ready === 'true') return;
  initHomeExamples();
  initCompilerInspector();
  initLanguageHistory();
  initLatestRelease();
  initAnalyzer();
  initHomeMotion();
  home.dataset.ready = 'true';
}
