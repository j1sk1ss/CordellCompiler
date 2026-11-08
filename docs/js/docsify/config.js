import { escapeHtml } from '../core/html.js';
import { highlightCode } from '../syntax/highlight.js';
import { renderCplRunnerMarkup } from '../playground/markup.js';
import { homeContentPlugin } from './content.js';
import { pageLifecyclePlugin } from './lifecycle.js';
import { playgroundPlugin } from '../playground/index.js';
import { benchmarksPlugin } from '../benchmarks.js';

export const docsifyConfig = {
  name: 'CPL <span>4.0</span><small>THE FIELD GUIDE</small>',
  repo: false,
  homepage: 'README.md',
  loadSidebar: 'sidebar.md',
  loadNavbar: false,
  mergeNavbar: false,
  requestHeaders: {
    'cache-control': 'no-cache'
  },
  alias: {
    '/.*/sidebar.md': 'sidebar.md',
    '/.*/navbar.md': 'navbar.md'
  },
  subMaxLevel: 2,
  maxLevel: 3,
  routerMode: 'hash',
  auto2top: true,
  topMargin: 24,
  search: {
    maxAge: 86400000,
    paths: 'auto',
    placeholder: 'Search docs...',
    noData: 'No results found',
    depth: 3,
    hideOtherSidebarContent: false,
    namespace: 'cpl-docs-pages-v2'
  },
  copyCode: {
    buttonText: 'Copy',
    errorText: 'Error',
    successText: 'Copied'
  },
  markdown: {
    renderer: {
      code: function(code, lang) {
        if (lang === 'cpl-run') {
          return renderCplRunnerMarkup(code);
        }

        const language = String(lang || 'markup').toLowerCase();
        return '<pre data-lang="' + escapeHtml(language) + '"><code class="language-' + escapeHtml(language) + '">' + highlightCode(code, language) + '</code></pre>';
      }
    }
  },
  plugins: [homeContentPlugin, pageLifecyclePlugin, playgroundPlugin, benchmarksPlugin]
};
