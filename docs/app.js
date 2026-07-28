(function() {
  const route = (location.hash || '').split('?')[0].replace(/\/+$/, '');
  const isHome = !route || route === '#' || route === '#/' || route === '#/README' || route === '#/README.md';
  document.documentElement.classList.toggle('cpl-home-route', isHome);
})();

window.CPL_RUNNER_URL = window.CPL_RUNNER_URL || (
  /^(127\.0\.0\.1|localhost)$/.test(location.hostname)
    ? 'http://127.0.0.1:8000/cpl/run'
    : '/cpl/run'
);
window.CPL_RUNNER_STUB = window.CPL_RUNNER_STUB || async function(payload) {
  await new Promise(resolve => setTimeout(resolve, 280));
  return {
    success: true,
    stdout: [
      '=== backend stub ===',
      'POST ' + window.CPL_RUNNER_URL,
      'Compilation and execution endpoint is not connected in this environment yet.',
      '',
      'Payload accepted:',
      '- lang: ' + payload.lang,
      '- code size: ' + payload.code.length + ' bytes',
      '',
      'Production backend should return stdout/stderr/compiler metadata with the same JSON shape.'
    ].join('\n'),
    stderr: '',
    compiler_exit_code: 0,
    program_exit_code: 0
  };
};

function cleanRoute(route) {
  if (!route) return '';
  let value = route;
  try {
    const url = new URL(route, window.location.origin);
    value = url.hash || route;
  } catch (e) {}
  value = value.split('?')[0];
  value = value.replace(/\/+$/, '');
  return value;
}

function escapeHtml(value) {
  return String(value)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#39;');
}

function spanToken(type, value) {
  return '<span class="token ' + type + '">' + escapeHtml(value) + '</span>';
}

function highlightCpl(code) {
  const keywords = new Set([
    'as', 'break', 'case', 'default', 'else', 'exit', 'extern', 'function',
    'glob', 'if', 'loop', 'return', 'ro', 'start', 'switch', 'while'
  ]);
  const types = new Set([
    'arr', 'dref', 'f32', 'f64', 'i0', 'i8', 'i16', 'i32', 'i64',
    'ptr', 'ref', 'container', 'u8', 'u16', 'u32', 'u64'
  ]);
  const builtins = new Set(['asm', 'sizeof', 'syscall']);

  let out = '';
  let i = 0;

  while (i < code.length) {
    const rest = code.slice(i);

    if (rest.startsWith(':/')) {
      const end = code.indexOf('/:', i + 2);
      const value = end === -1 ? code.slice(i) : code.slice(i, end + 2);
      out += spanToken('comment', value);
      i += value.length;
      continue;
    }

    if (code[i] === ':' && (i === 0 || code[i - 1] === '\n' || /\s/.test(code[i - 1]))) {
      const end = code.indexOf(':', i + 1);
      const newline = code.indexOf('\n', i + 1);
      if (end !== -1 && (newline === -1 || end < newline)) {
        const value = code.slice(i, end + 1);
        out += spanToken('comment', value);
        i += value.length;
        continue;
      }
    }

    if (code[i] === '#') {
      const end = code.indexOf('\n', i + 1);
      const value = end === -1 ? code.slice(i) : code.slice(i, end);
      out += spanToken('preprocessor', value);
      i += value.length;
      continue;
    }

    if (rest.startsWith('@[')) {
      const end = code.indexOf(']', i + 2);
      const value = end === -1 ? code.slice(i) : code.slice(i, end + 1);
      out += spanToken('annotation', value);
      i += value.length;
      continue;
    }

    if (code[i] === '"' || code[i] === "'") {
      const quote = code[i];
      let j = i + 1;
      while (j < code.length) {
        if (code[j] === '\\') {
          j += 2;
          continue;
        }
        if (code[j] === quote) {
          j += 1;
          break;
        }
        j += 1;
      }
      out += spanToken(quote === '"' ? 'string' : 'char', code.slice(i, j));
      i = j;
      continue;
    }

    const number = rest.match(/^(0[xX][0-9a-fA-F]+|0[bB][01]+|[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?)/);
    if (number) {
      out += spanToken('number', number[0]);
      i += number[0].length;
      continue;
    }

    const word = rest.match(/^[A-Za-z_][A-Za-z0-9_]*/);
    if (word) {
      const value = word[0];
      const next = code.slice(i + value.length).match(/^\s*\(/);
      if (keywords.has(value)) out += spanToken('keyword', value);
      else if (types.has(value)) out += spanToken('type', value);
      else if (builtins.has(value)) out += spanToken('keyword', value);
      else if (next) out += spanToken('function', value);
      else out += escapeHtml(value);
      i += value.length;
      continue;
    }

    const op = rest.match(/^(=>|->|==|!=|<=|>=|\+=|-=|\*=|\/=|%=|&&|\|\||[{}\(\)\[\],.;:+\-*\/%=&|!<>])/);
    if (op) {
      out += spanToken(/[{}()[\],.;:]/.test(op[0]) ? 'punctuation' : 'operator', op[0]);
      i += op[0].length;
      continue;
    }

    out += escapeHtml(code[i]);
    i += 1;
  }

  return out;
}

function highlightCode(code, lang) {
  const normalized = (lang || 'markup').toLowerCase();
  if (normalized === 'cpl') return highlightCpl(code);

  if (window.Prism && Prism.languages && Prism.languages[normalized]) {
    return Prism.highlight(code, Prism.languages[normalized], normalized);
  }

  return escapeHtml(code);
}

window.__benchmarkCharts = new Map();

function destroyBenchmarkCharts() {
  for (const chart of window.__benchmarkCharts.values()) {
    chart.destroy();
  }
  window.__benchmarkCharts.clear();
}

function renderBenchmarkCharts() {
  destroyBenchmarkCharts();

  const cards = document.querySelectorAll('.benchmark-card');
  if (typeof Chart === 'undefined') {
    for (const card of cards) {
      const wrap = card.querySelector('.benchmark-chart-wrap');
      const next = wrap ? wrap.nextElementSibling : null;
      if (wrap && !(next && next.classList && next.classList.contains('benchmark-note'))) {
        const note = document.createElement('div');
        note.className = 'benchmark-note';
        note.textContent = 'Chart.js is still loading or unavailable.';
        wrap.insertAdjacentElement('afterend', note);
      }
    }
    return;
  }

  for (const card of cards) {
    card.querySelectorAll('.benchmark-note').forEach(note => note.remove());

    const canvas = card.querySelector('canvas.benchmark-chart');
    if (!canvas) continue;

    try {
      let cfg = null;

      const dataNode = card.querySelector('textarea.benchmark-data');
      if (dataNode) {
        cfg = JSON.parse(dataNode.value || dataNode.textContent);
      } else {
        const labels= (card.dataset.labels || '').split('|').map(x => x.trim()).filter(Boolean);
        const values= (card.dataset.values || '').split('|').map(x => Number(x.trim())).filter(x => !Number.isNaN(x));

        cfg = {
          title: card.dataset.title || '',
          labels: labels,
          datasets: [{
            label: card.dataset.datasetLabel || 'Value',
            data: values,
            borderWidth: 1,
            borderRadius: 8,
            maxBarThickness: 72
          }],
          yLabel: card.dataset.yLabel || card.dataset.datasetLabel || 'Value',
          tooltipSuffix: card.dataset.tooltipSuffix || ''
        };
      }

      if (!cfg.labels || !cfg.labels.length || !cfg.datasets || !cfg.datasets.length) {
        throw new Error('Empty benchmark config');
      }

      const datasets = cfg.datasets.map(ds => ({
        borderWidth: 1,
        borderRadius: 8,
        maxBarThickness: 72,
        ...ds
      }));

      const chart = new Chart(canvas.getContext('2d'), {
        type: cfg.type || 'bar',
        data: {
          labels: cfg.labels,
          datasets: datasets
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          interaction: {
            mode: 'index',
            intersect: false
          },
          plugins: {
            legend: {
              display: true,
              position: 'top'
            },
            title: {
              display: !!cfg.title,
              text: cfg.title || ''
            },
            tooltip: {
              callbacks: {
                label: function(context) {
                  const suffix = cfg.tooltipSuffix || '';
                  const raw = context.parsed && typeof context.parsed.y !== 'undefined' ? context.parsed.y : context.raw;
                  const value = typeof raw === 'number' ? raw.toFixed(6) : raw;
                  return `${context.dataset.label}: ${value}${suffix}`;
                }
              }
            }
          },
          scales: {
            y: {
              beginAtZero: true,
              title: {
                display: true,
                text: cfg.yLabel || cfg.datasetLabel || 'Value'
              }
            }
          }
        }
      });

      window.__benchmarkCharts.set(canvas, chart);
    } catch (err) {
      const wrap = card.querySelector('.benchmark-chart-wrap');
      if (wrap) {
        wrap.innerHTML = `<div class="benchmark-note">Chart error: ${err.message}</div>`;
      }
      console.error(err);
    }
  }
}

window.addEventListener('load', function() {
  setTimeout(renderBenchmarkCharts, 0);
});

const cplMonkeyScrollController = (function() {
  const startAngle = 0;
  const endAngle = 92;
  let ticking = false;
  let bound = false;

  function clamp(value, min, max) {
    return Math.min(Math.max(value, min), max);
  }

  function isHomeRoute() {
    const route = cleanRoute(location.hash);
    return !route || route === '#' || route === '#/' || route === '#/README' || route === '#/README.md';
  }

  function getScrollTop() {
    return window.scrollY || document.documentElement.scrollTop || document.body.scrollTop || 0;
  }

  function update() {
    ticking = false;

    const root = document.documentElement;
    const monkey = document.querySelector('.cpl-scroll-monkey');
    if (!monkey || !isHomeRoute()) {
      root.style.setProperty('--monkey-arm-angle', startAngle + 'deg');
      return;
    }

    const rect = monkey.getBoundingClientRect();
    const scale = clamp(Math.min(rect.width / 689, rect.height / 512), 0.72, 1.82);
    root.style.setProperty('--monkey-scale', scale);

    const home = document.querySelector('.cpl-home');
    const scrollRange = Math.max(1, (home ? home.offsetHeight : document.documentElement.scrollHeight) - window.innerHeight);
    const progress = clamp(getScrollTop() / scrollRange, 0, 1);
    const frames = 9;
    const steppedProgress = Math.round(progress * frames) / frames;
    const angle = startAngle + (endAngle - startAngle) * steppedProgress;
    root.style.setProperty('--monkey-arm-angle', angle.toFixed(0) + 'deg');
  }

  function requestUpdate() {
    if (ticking) return;
    ticking = true;
    window.requestAnimationFrame(update);
  }

  function init() {
    if (!bound) {
      bound = true;
      window.addEventListener('scroll', requestUpdate, { passive: true });
      window.addEventListener('resize', requestUpdate);
      window.addEventListener('hashchange', requestUpdate);
    }
    requestUpdate();
  }

  return { init, update: requestUpdate };
})();

function renderCplRunnerMarkup(code) {
  const encoded = encodeURIComponent(code || '');
  return `
    <div class="cpl-runner" data-lang="cpl" data-code="${encoded}">
      <div class="cpl-runner-toolbar">
        <div class="cpl-runner-toolbar-left">
          <div class="cpl-runner-title">CPL playground</div>
          <span class="cpl-runner-badge">Compile and run via backend</span>
        </div>
        <div class="cpl-runner-toolbar-right">
          <button class="cpl-runner-button secondary" type="button" data-action="format">Format</button>
          <button class="cpl-runner-button" type="button" data-action="run">Run</button>
        </div>
      </div>
      <div class="cpl-runner-editor-shell">
        <div class="cpl-runner-tabs">
          <span class="cpl-runner-tab">main.cpl</span>
        </div>
        <div class="cpl-runner-editor"></div>
      </div>
      <pre class="cpl-runner-output">Ready to run. The browser will POST this code to the CPL backend.</pre>
    </div>
  `;
}

function notifyCplContentReady(root) {
  let event;
  try {
    event = new CustomEvent('cpl:content-ready', { detail: { root: root || document } });
  } catch (e) {
    event = document.createEvent('Event');
    event.initEvent('cpl:content-ready', true, true);
    event.detail = { root: root || document };
  }
  document.dispatchEvent(event);
}

window.$docsify = {
  name: 'CPL v3.6<small>Language docs & compiler internals</small>',
  repo: 'https://github.com/j1sk1ss/CordellCompiler',
  homepage: 'README.md',
  loadSidebar: 'sidebar.md',
  loadNavbar: 'navbar.md',
  mergeNavbar: true,
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
  plugins: [
    function(hook) {
      function updateShellRoute() {
        const route = cleanRoute(location.hash);
        const isHome = !route || route === '#' || route === '#/' || route === '#/README' || route === '#/README.md';
        document.documentElement.classList.toggle('cpl-home-route', isHome);
        document.body.classList.toggle('cpl-home-route', isHome);
      }

      hook.ready(updateShellRoute);
      hook.doneEach(function() {
        updateShellRoute();
        cplMonkeyScrollController.init();
      });
      window.addEventListener('hashchange', updateShellRoute);
    },

    function(hook) {
      const folderIcon = 'icons/win95-folder.png';
      const docIcon = 'icons/win95-list.png';
      let shellReady = false;
      const windowState = {
        explorer: { x: 18, y: 18, minimized: false, maximized: false, closed: false, zIndex: 20 },
        monkey: { x: null, y: 34, minimized: false, maximized: false, closed: false, zIndex: 21 }
      };
      const docWindows = new Map();
      let nextDocOffset = 0;
      let topWindowZ = 30;

      function routeOf(href) {
        const route = cleanRoute(href || '');
        if (!route || route === '#') return '#/';
        return route.replace(/\.md$/, '');
      }

      function currentRoute() {
        return routeOf(location.hash || '#/');
      }

      function isHome(route) {
        return !route || route === '#' || route === '#/' || route === '#/README' || route === '#/README.md';
      }

      function collectSections() {
        const nav = document.querySelector('.sidebar-nav');
        if (!nav) return [];

        const topItems = Array.from(nav.querySelectorAll(':scope > ul > li'));
        const items = topItems.length ? topItems : Array.from(nav.querySelectorAll('li')).filter(li => li.querySelector('p'));

        const sections = items.map(li => {
          const label = (li.querySelector(':scope > p') || li.querySelector('p'));
          const links = Array.from(li.querySelectorAll('a'))
            .map(a => ({
              href: routeOf(a.getAttribute('href') || ''),
              title: (a.textContent || '').trim()
            }))
            .filter(item => item.href && item.title);

          return {
            title: label ? label.textContent.trim() : 'Documents',
            links
          };
        }).filter(section => section.links.length);

        if (sections.length) return sections;

        const links = Array.from(nav.querySelectorAll('a'))
          .map(a => ({
            href: routeOf(a.getAttribute('href') || ''),
            title: (a.textContent || '').trim()
          }))
          .filter(item => item.href && item.title);

        return links.length ? [{ title: 'Documents', links }] : [];
      }

      function flattenSections(sections) {
        const seen = new Set();
        const result = [];

        sections.forEach(section => {
          section.links.forEach(link => {
            if (seen.has(link.href)) return;
            seen.add(link.href);
            result.push({ ...link, section: section.title });
          });
        });

        return result;
      }

      function findCurrentTitle(items) {
        const route = currentRoute();
        if (isHome(route)) return 'CPL Explorer';
        const item = items.find(candidate => candidate.href === route);
        if (item) return item.title;

        const h1 = document.querySelector('.markdown-section h1');
        return h1 ? h1.textContent.trim() : 'Document';
      }

      function routeToMarkdownPath(route) {
        let path = String(route || '#/README').replace(/^#\/?/, '');
        if (!path || path === '/') path = 'README';
        if (!/\.md$/i.test(path)) path += '.md';
        return path;
      }

      function normalizeInternalDocHref(href) {
        if (!href) return null;

        let value = String(href).trim();
        if (!value || value === '#') return null;

        try {
          const url = new URL(value, window.location.href);
          if (url.origin !== window.location.origin) return null;
          if (url.hash && url.hash.startsWith('#/')) {
            value = url.hash;
          } else if (!value.startsWith('#/')) {
            value = url.pathname.replace(/^\/+/, '') + url.search + url.hash;
          }
        } catch (e) {}

        if (value.startsWith('#/') || value.startsWith('#!')) {
          return routeOf(value);
        }

        if (value.startsWith('#')) return null;

        value = value
          .replace(/^\.?\//, '')
          .replace(/^docs\//, '')
          .split('#')[0]
          .split('?')[0];

        if (!value) return null;
        if (value === 'README.md' || value === 'README') return '#/';
        if (!/^pages\//.test(value)) return null;

        return '#/' + value.replace(/\.md$/i, '');
      }

      function titleForRoute(route, fallback) {
        const items = flattenSections(collectSections());
        const item = items.find(candidate => candidate.href === routeOf(route));
        return item ? item.title : (fallback || 'Document');
      }

      function renderMarkdown(markdown) {
        if (window.marked) {
          if (typeof window.marked.parse === 'function') return window.marked.parse(markdown);
          if (typeof window.marked === 'function') return window.marked(markdown);
        }

        const codeBlocks = [];
        let value = String(markdown || '').replace(/```([\s\S]*?)```/g, function(_, code) {
          const token = '\u0000CODE' + codeBlocks.length + '\u0000';
          codeBlocks.push('<pre><code>' + escapeHtml(code.replace(/^[^\n]*\n/, '')) + '</code></pre>');
          return token;
        });

        value = escapeHtml(value)
          .replace(/^### (.*)$/gm, '<h3>$1</h3>')
          .replace(/^## (.*)$/gm, '<h2>$1</h2>')
          .replace(/^# (.*)$/gm, '<h1>$1</h1>')
          .replace(/\*\*(.*?)\*\*/g, '<strong>$1</strong>')
          .replace(/\[(.*?)\]\((.*?)\)/g, '<a href="$2">$1</a>')
          .split(/\n{2,}/)
          .map(block => {
            if (/^<h[1-3]|^\u0000CODE/.test(block)) return block;
            return '<p>' + block.replace(/\n/g, '<br>') + '</p>';
          })
          .join('\n');

        codeBlocks.forEach((block, index) => {
          value = value.replace('\u0000CODE' + index + '\u0000', block);
        });

        return value;
      }

      function documentKey(route) {
        return 'doc:' + route;
      }

      function ensureDocumentState(key) {
        if (!windowState[key]) {
          const offset = nextDocOffset % 7;
          nextDocOffset += 1;
          windowState[key] = {
            x: 80 + offset * 28,
            y: 54 + offset * 24,
            minimized: false,
            maximized: false,
            closed: false,
            zIndex: ++topWindowZ
          };
        }
        return windowState[key];
      }

      function formatClock(date) {
        let hours = date.getHours();
        const minutes = String(date.getMinutes()).padStart(2, '0');
        const suffix = hours >= 12 ? 'PM' : 'AM';
        hours = hours % 12 || 12;
        return hours + ':' + minutes + ' ' + suffix;
      }

      function updateClock() {
        const tray = document.querySelector('.win95-tray');
        if (!tray) return;
        tray.textContent = formatClock(new Date());
      }

      function setWindowPosition(win, state) {
        if (!win || state.maximized) return;
        if (typeof state.x === 'number') {
          win.style.left = state.x + 'px';
          win.style.right = 'auto';
        }
        if (typeof state.y === 'number') {
          win.style.top = state.y + 'px';
          win.style.bottom = 'auto';
        }
      }

      function clampWindowPosition(win, x, y) {
        const rect = win.getBoundingClientRect();
        const maxX = Math.max(0, window.innerWidth - rect.width - 6);
        const maxY = Math.max(0, window.innerHeight - rect.height - 44);
        return {
          x: Math.min(Math.max(0, x), maxX),
          y: Math.min(Math.max(0, y), maxY)
        };
      }

      function applyWindowState(key, win) {
        const state = windowState[key];
        if (!state || !win) return;
        win.classList.add('win95-window');
        win.classList.toggle('is-minimized', state.minimized);
        win.classList.toggle('is-maximized', state.maximized);
        win.classList.toggle('is-closed', state.closed);
        if (typeof state.zIndex === 'number') win.style.zIndex = state.zIndex;
        setWindowPosition(win, state);
      }

      function focusWindow(key) {
        const state = windowState[key];
        const win = document.querySelector('[data-window="' + key + '"]');
        if (!state || !win || state.closed) return;
        state.zIndex = ++topWindowZ;
        win.style.zIndex = state.zIndex;
      }

      function setTaskActive(key, active) {
        const button = document.querySelector('[data-window-toggle="' + key + '"]');
        if (button) button.classList.toggle('is-active', active);
      }

      function minimizeWindow(key) {
        const state = windowState[key];
        const win = document.querySelector('[data-window="' + key + '"]');
        if (!state || !win) return;
        state.minimized = true;
        win.classList.add('is-minimized');
        setTaskActive(key, false);
      }

      function closeWindow(key) {
        const state = windowState[key];
        const win = document.querySelector('[data-window="' + key + '"]');
        if (!state || !win) return;
        state.closed = true;
        state.minimized = false;
        state.maximized = false;
        win.classList.add('is-closed');
        win.classList.remove('is-minimized', 'is-maximized');
        renderTaskbar(flattenSections(collectSections()));
      }

      function openWindow(key) {
        const state = windowState[key];
        const win = document.querySelector('[data-window="' + key + '"]');
        if (!state) return;
        state.closed = false;
        state.minimized = false;
        if (win) {
          win.classList.remove('is-closed', 'is-minimized');
          win.classList.toggle('is-maximized', state.maximized);
          setWindowPosition(win, state);
        }
        focusWindow(key);
        renderTaskbar(flattenSections(collectSections()));
      }

      function restoreWindow(key) {
        const state = windowState[key];
        const win = document.querySelector('[data-window="' + key + '"]');
        if (!state || !win) return;
        state.closed = false;
        state.minimized = false;
        win.classList.remove('is-closed', 'is-minimized');
        win.classList.toggle('is-maximized', state.maximized);
        setWindowPosition(win, state);
        setTaskActive(key, true);
        focusWindow(key);
      }

      function toggleMaximize(key) {
        const state = windowState[key];
        const win = document.querySelector('[data-window="' + key + '"]');
        if (!state || !win) return;
        state.closed = false;
        state.minimized = false;
        state.maximized = !state.maximized;
        win.classList.remove('is-closed', 'is-minimized');
        win.classList.toggle('is-maximized', state.maximized);
        if (!state.maximized) setWindowPosition(win, state);
        setTaskActive(key, true);
        focusWindow(key);
      }

      function bindWindowChrome(key, win, handle) {
        if (!win || win.dataset.chromeBound === '1') return;
        win.dataset.chromeBound = '1';
        win.dataset.window = key;

        win.addEventListener('pointerdown', function() {
          focusWindow(key);
        });

        win.querySelectorAll('[data-window-action]').forEach(button => {
          button.addEventListener('click', function(event) {
            event.preventDefault();
            event.stopPropagation();
            const action = button.dataset.windowAction;
            if (action === 'minimize') minimizeWindow(key);
            if (action === 'maximize') toggleMaximize(key);
            if (action === 'close') closeWindow(key);
          });
        });

        if (!handle) return;
        handle.addEventListener('pointerdown', function(event) {
          if (event.button !== 0 || event.target.closest('[data-window-action]')) return;
          const state = windowState[key];
          if (!state || state.maximized) return;

          event.preventDefault();
          focusWindow(key);
          const rect = win.getBoundingClientRect();
          const offsetX = event.clientX - rect.left;
          const offsetY = event.clientY - rect.top;

          win.setPointerCapture(event.pointerId);

          function move(moveEvent) {
            const next = clampWindowPosition(win, moveEvent.clientX - offsetX, moveEvent.clientY - offsetY);
            state.x = next.x;
            state.y = next.y;
            setWindowPosition(win, state);
          }

          function stop() {
            win.removeEventListener('pointermove', move);
            win.removeEventListener('pointerup', stop);
            win.removeEventListener('pointercancel', stop);
          }

          win.addEventListener('pointermove', move);
          win.addEventListener('pointerup', stop);
          win.addEventListener('pointercancel', stop);
        });
      }

      function bindTaskbarWindowButtons() {
        document.querySelectorAll('[data-window-toggle]').forEach(button => {
          if (button.dataset.bound === '1') return;
          button.dataset.bound = '1';
          button.addEventListener('click', function(event) {
            const key = button.dataset.windowToggle;
            if (!key) return;
            if (currentRoute() !== '#/') return;
            event.preventDefault();
            const state = windowState[key];
            if (!state) return;
            if (state.closed) {
              openWindow(key);
              return;
            }
            if (state.minimized) {
              restoreWindow(key);
            } else {
              minimizeWindow(key);
            }
          });
        });
      }

      function bindStartWindowButtons() {
        document.querySelectorAll('[data-window-open]').forEach(button => {
          if (button.dataset.bound === '1') return;
          button.dataset.bound = '1';
          button.addEventListener('click', function(event) {
            const key = button.dataset.windowOpen;
            if (!key) return;
            event.preventDefault();
            if (currentRoute() !== '#/') {
              location.hash = '#/';
              setTimeout(function() { openWindow(key); }, 140);
              return;
            }
            openWindow(key);
          });
        });
      }

      function ensureMonkeyWindow() {
        const monkey = document.querySelector('.cpl-scroll-monkey');
        if (!monkey || monkey.dataset.windowReady === '1') return monkey;

        monkey.dataset.windowReady = '1';
        monkey.classList.add('has-window-ui', 'win95-window');
        monkey.dataset.window = 'monkey';

        const titlebar = document.createElement('div');
        titlebar.className = 'win95-monkey-titlebar';
        titlebar.innerHTML = `
          <div class="win95-window-title">monkey.gif</div>
          <div class="win95-window-controls">
            <button class="win95-window-button minimize" type="button" data-window-action="minimize" aria-label="Minimize monkey.gif"></button>
            <button class="win95-window-button maximize" type="button" data-window-action="maximize" aria-label="Maximize monkey.gif"></button>
            <button class="win95-window-button close" type="button" data-window-action="close" aria-label="Close monkey.gif"></button>
          </div>
        `;
        monkey.insertBefore(titlebar, monkey.firstChild);
        return monkey;
      }

      function bindDocumentBodyLinks(body) {
        body.querySelectorAll('a[href]').forEach(link => {
          const href = link.getAttribute('href') || '';
          const route = normalizeInternalDocHref(href);
          if (!route) return;
          link.addEventListener('click', function(event) {
            event.preventDefault();
            const title = titleForRoute(route, link.textContent.trim() || 'Document');
            openDocumentWindow(route, title);
          });
        });
      }

      async function openDocumentWindow(route, title) {
        const normalizedRoute = routeOf(route);
        const key = documentKey(normalizedRoute);
        const state = ensureDocumentState(key);
        state.closed = false;
        state.minimized = false;

        let record = docWindows.get(key);
        if (!record) {
          record = { route: normalizedRoute, title, loaded: false, html: '' };
          docWindows.set(key, record);
        } else {
          record.title = title || record.title;
        }

        renderDocumentWindows();
        focusWindow(key);
        renderTaskbar(flattenSections(collectSections()));

        const win = document.querySelector('[data-window="' + key + '"]');
        const body = win && win.querySelector('.win95-doc-body');
        if (!body || record.loaded) return;

        body.innerHTML = '<p>Loading...</p>';
        try {
          const response = await fetch(routeToMarkdownPath(normalizedRoute), { cache: 'no-cache' });
          if (!response.ok) throw new Error('HTTP ' + response.status);
          const markdown = await response.text();
          record.html = renderMarkdown(markdown);
          body.innerHTML = record.html;
          bindDocumentBodyLinks(body);
          notifyCplContentReady(body);
          record.loaded = true;
        } catch (err) {
          body.innerHTML = '<p>Could not open file: ' + escapeHtml(err.message) + '</p>';
        }
      }

      function renderDocumentWindows() {
        const desktop = document.querySelector('.win95-desktop');
        if (!desktop) return;

        for (const [key, record] of docWindows.entries()) {
          let win = desktop.querySelector('[data-window="' + key + '"]');
          const state = ensureDocumentState(key);

          if (!win) {
            win = document.createElement('div');
            win.className = 'win95-doc-window win95-window';
            win.dataset.window = key;
            win.innerHTML = `
              <div class="win95-window-titlebar">
                <img src="${docIcon}" alt="" />
                <div class="win95-window-title">${escapeHtml(record.title)} - CPL Docs</div>
                <div class="win95-window-controls">
                  <button class="win95-window-button minimize" type="button" data-window-action="minimize" aria-label="Minimize ${escapeHtml(record.title)}"></button>
                  <button class="win95-window-button maximize" type="button" data-window-action="maximize" aria-label="Maximize ${escapeHtml(record.title)}"></button>
                  <button class="win95-window-button close" type="button" data-window-action="close" aria-label="Close ${escapeHtml(record.title)}"></button>
                </div>
              </div>
              <div class="win95-doc-body"></div>
            `;
            desktop.appendChild(win);
            bindWindowChrome(key, win, win.querySelector('.win95-window-titlebar'));
            if (record.html) {
              const body = win.querySelector('.win95-doc-body');
              body.innerHTML = record.html;
              bindDocumentBodyLinks(body);
              notifyCplContentReady(body);
            }
          } else {
            const titleNode = win.querySelector('.win95-window-title');
            if (titleNode) titleNode.textContent = record.title + ' - CPL Docs';
          }

          applyWindowState(key, win);
          if (state.closed) win.classList.add('is-closed');
        }
      }

      function ensureShell() {
        if (shellReady) return;
        shellReady = true;

        const desktop = document.createElement('div');
        desktop.className = 'win95-desktop';
        desktop.setAttribute('aria-label', 'CPL documentation explorer');
        document.body.appendChild(desktop);

        const startMenu = document.createElement('div');
        startMenu.className = 'win95-start-menu';
        startMenu.innerHTML = '<div class="win95-start-rail">CPL v3.6</div><div class="win95-start-list"></div>';
        document.body.appendChild(startMenu);

        const taskbar = document.createElement('div');
        taskbar.className = 'win95-taskbar';
        taskbar.innerHTML = `
          <button class="win95-start-button" type="button" aria-expanded="false">
            <img src="${folderIcon}" alt="" /> Start
          </button>
          <div class="win95-task-buttons"></div>
          <div class="win95-tray" aria-label="Clock"></div>
        `;
        document.body.appendChild(taskbar);

        const startButton = taskbar.querySelector('.win95-start-button');
        updateClock();
        window.setInterval(updateClock, 30000);

        startButton.addEventListener('click', function(event) {
          event.stopPropagation();
          const open = !startMenu.classList.contains('is-open');
          startMenu.classList.toggle('is-open', open);
          startButton.classList.toggle('is-open', open);
          startButton.setAttribute('aria-expanded', open ? 'true' : 'false');
        });

        document.addEventListener('click', function(event) {
          if (!startMenu.contains(event.target) && !startButton.contains(event.target)) {
            startMenu.classList.remove('is-open');
            startButton.classList.remove('is-open');
            startButton.setAttribute('aria-expanded', 'false');
          }
        });

        startMenu.addEventListener('click', function(event) {
          if (event.target.closest('a')) {
            startMenu.classList.remove('is-open');
            startButton.classList.remove('is-open');
            startButton.setAttribute('aria-expanded', 'false');
          }
        });
      }

      function renderDesktop(items) {
        const desktop = document.querySelector('.win95-desktop');
        if (!desktop) return;

        const icons = items.map(item => `
          <a class="win95-file-icon" href="${item.href}" data-doc-route="${item.href}" data-doc-title="${escapeHtml(item.title)}" title="${escapeHtml(item.section + ': ' + item.title)}">
            <img src="${docIcon}" alt="" />
            <span>${escapeHtml(item.title)}</span>
          </a>
        `).join('');

        desktop.innerHTML = `
          <div class="win95-explorer win95-window" data-window="explorer" role="region" aria-label="CPL Explorer window">
            <div class="win95-window-titlebar">
              <img src="${folderIcon}" alt="" />
              <div class="win95-window-title">CPL Explorer - docs</div>
              <div class="win95-window-controls">
                <button class="win95-window-button minimize" type="button" data-window-action="minimize" aria-label="Minimize CPL Explorer"></button>
                <button class="win95-window-button maximize" type="button" data-window-action="maximize" aria-label="Maximize CPL Explorer"></button>
                <button class="win95-window-button close" type="button" data-window-action="close" aria-label="Close CPL Explorer"></button>
              </div>
            </div>
            <div class="win95-explorer-toolbar">
              <strong>Address</strong>
              <div class="win95-address">C:\\CordellCompiler\\docs</div>
            </div>
            <div class="win95-icon-grid">${icons}</div>
          </div>
        `;

        const explorer = desktop.querySelector('[data-window="explorer"]');
        applyWindowState('explorer', explorer);
        bindWindowChrome('explorer', explorer, explorer && explorer.querySelector('.win95-window-titlebar'));
        desktop.querySelectorAll('[data-doc-route]').forEach(link => {
          link.addEventListener('click', function(event) {
            event.preventDefault();
            openDocumentWindow(link.dataset.docRoute, link.dataset.docTitle || link.textContent.trim() || 'Document');
          });
        });
        renderDocumentWindows();
      }

      function renderStartMenu(sections) {
        const list = document.querySelector('.win95-start-list');
        if (!list) return;

        list.innerHTML = `
          <a class="win95-start-link" href="#/" data-window-open="explorer">
            <img src="${folderIcon}" alt="" />
            <span>Explorer Home</span>
          </a>
          <a class="win95-start-link" href="#/" data-window-open="monkey">
            <img src="${docIcon}" alt="" />
            <span>monkey.gif</span>
          </a>
          ${sections.map(section => `
            <div class="win95-start-section">${escapeHtml(section.title)}</div>
            ${section.links.map(link => `
              <a class="win95-start-link" href="${link.href}" data-doc-route="${link.href}" data-doc-title="${escapeHtml(link.title)}">
                <img src="${docIcon}" alt="" />
                <span>${escapeHtml(link.title)}</span>
              </a>
            `).join('')}
          `).join('')}
          <div class="win95-start-section">External</div>
          <a class="win95-start-link" href="https://github.com/j1sk1ss/CordellCompiler">
            <img src="${folderIcon}" alt="" />
            <span>GitHub</span>
          </a>
        `;
        bindStartWindowButtons();
        list.querySelectorAll('[data-doc-route]').forEach(link => {
          if (link.dataset.bound === '1') return;
          link.dataset.bound = '1';
          link.addEventListener('click', function(event) {
            event.preventDefault();
            const route = normalizeInternalDocHref(link.dataset.docRoute);
            if (!route) return;
            const title = link.dataset.docTitle || link.textContent.trim() || 'Document';
            if (currentRoute() !== '#/') {
              location.hash = '#/';
              setTimeout(function() { openDocumentWindow(route, title); }, 140);
              return;
            }
            openDocumentWindow(route, title);
          });
        });
      }

      function renderTaskbar(items) {
        const taskButtons = document.querySelector('.win95-task-buttons');
        if (!taskButtons) return;

        const route = currentRoute();
        const title = findCurrentTitle(items);
        const homeActive = isHome(route);
        const docButtons = Array.from(docWindows.entries())
          .filter(([key]) => windowState[key] && !windowState[key].closed)
          .map(([key, record]) => `
            <button class="win95-task-button ${windowState[key].minimized ? '' : 'is-active'}" type="button" data-window-toggle="${key}">
              <img src="${docIcon}" alt="" />
              <span>${escapeHtml(record.title)}</span>
            </button>
          `).join('');

        taskButtons.innerHTML = `
          ${windowState.explorer.closed ? '' : `
            <a class="win95-task-button ${homeActive && !windowState.explorer.minimized ? 'is-active' : ''}" href="#/" data-window-toggle="explorer">
              <img src="${folderIcon}" alt="" />
              <span>CPL Explorer</span>
            </a>
          `}
          ${homeActive && !windowState.monkey.closed ? `
            <button class="win95-task-button ${windowState.monkey.minimized ? '' : 'is-active'}" type="button" data-window-toggle="monkey">
              <img src="${docIcon}" alt="" />
              <span>monkey.gif</span>
            </button>
          ` : ''}
          ${homeActive ? docButtons : ''}
          ${homeActive ? '' : `
            <a class="win95-task-button is-active" href="${route}">
              <img src="${docIcon}" alt="" />
              <span>${escapeHtml(title)}</span>
            </a>
          `}
        `;
        bindTaskbarWindowButtons();
      }

      function renderWindowTitle(items) {
        const article = document.querySelector('.markdown-section');
        if (!article || document.documentElement.classList.contains('cpl-home-route')) return;

        const old = article.querySelector('.win95-window-titlebar');
        if (old) old.remove();

        const title = findCurrentTitle(items);
        const bar = document.createElement('div');
        bar.className = 'win95-window-titlebar';
        bar.innerHTML = `
          <img src="${docIcon}" alt="" />
          <div class="win95-window-title">${escapeHtml(title)} - CPL Docs</div>
          <div class="win95-window-controls">
            <a class="win95-window-button minimize" href="#/" aria-label="Minimize"></a>
            <button class="win95-window-button maximize" type="button" aria-label="Maximize"></button>
            <a class="win95-window-button close" href="#/" aria-label="Close"></a>
          </div>
        `;
        article.insertBefore(bar, article.firstChild);
      }

      function renderShell() {
        ensureShell();

        const sections = collectSections();
        const items = flattenSections(sections);
        if (!items.length) return;

        renderDesktop(items);
        renderStartMenu(sections);
        renderTaskbar(items);
        renderWindowTitle(items);
        const monkey = ensureMonkeyWindow();
        if (monkey) {
          if (windowState.monkey.x === null) {
            windowState.monkey.x = Math.max(18, window.innerWidth - monkey.getBoundingClientRect().width - 18);
          }
          applyWindowState('monkey', monkey);
          bindWindowChrome('monkey', monkey, monkey.querySelector('.win95-monkey-titlebar'));
        }
      }

      hook.doneEach(function() {
        setTimeout(renderShell, 80);
      });

      window.addEventListener('hashchange', function() {
        setTimeout(renderShell, 80);
      });
    },

    function(hook) {
      const pagePrefix = '#/pages/';

      function getPageLinks() {
        const nav = document.querySelector('.sidebar-nav');
        if (!nav) return [];
        const seen = new Set();
        return Array.from(nav.querySelectorAll('a'))
          .map(a => ({
            href: cleanRoute(a.getAttribute('href') || ''),
            title: (a.textContent || '').trim()
          }))
          .filter(item => item.href.startsWith(pagePrefix))
          .filter(item => {
            if (!item.href || seen.has(item.href)) return false;
            seen.add(item.href);
            return true;
          });
      }

      function removePager() {
        const old = document.querySelector('.docsify-pagination-container');
        if (old) old.remove();
      }

      function normalizeHash(href) {
        if (!href) return '';
        try {
          const url = new URL(href, window.location.origin);
          let hash = url.hash || href;
          hash = hash.replace(/\.md([?#].*)?$/, '');
          return hash;
        } catch (e) {
          return href.replace(/\.md([?#].*)?$/, '');
        }
      }

      function renderPager() {
        removePager();
      }

      hook.doneEach(function() {
        setTimeout(renderPager, 120);
      });

      window.addEventListener('hashchange', function() {
        setTimeout(renderPager, 120);
      });
    },

    function(hook) {
      const editors = new Map();
      let monacoReadyPromise = null;

      function decodeCode(value) {
        try {
          return decodeURIComponent(value || '');
        } catch (e) {
          return value || '';
        }
      }

      function ensureMonaco() {
        if (window.monaco && window.require) return Promise.resolve(window.monaco);
        if (monacoReadyPromise) return monacoReadyPromise;

        monacoReadyPromise = new Promise((resolve, reject) => {
          const finish = () => {
            try {
              window.require.config({
                paths: {
                  vs: 'https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/vs'
                }
              });
              window.MonacoEnvironment = {
                getWorkerUrl: function() {
                  return 'data:text/javascript;charset=utf-8,' + encodeURIComponent(`self.MonacoEnvironment={baseUrl:"https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/"};importScripts("https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/vs/base/worker/workerMain.js");`);
                }
              };
              window.require(['vs/editor/editor.main'], function() {
                resolve(window.monaco);
              }, reject);
            } catch (err) {
              reject(err);
            }
          };

          if (window.require && window.require.config) {
            finish();
            return;
          }

          const script = document.createElement('script');
          script.src = 'https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/vs/loader.js';
          script.onload = finish;
          script.onerror = () => reject(new Error('Failed to load Monaco loader'));
          document.head.appendChild(script);
        });

        return monacoReadyPromise;
      }

      function registerCplLanguage(monaco) {
        if (window.__cplMonacoRegistered) return;
        window.__cplMonacoRegistered = true;

        monaco.languages.register({ id: 'cpl' });

        monaco.languages.setMonarchTokensProvider('cpl', {
          tokenizer: {
            root: [
              [/:\//, 'comment', '@comment_block_slash'],
              [/:(?=\s|$)/, 'comment', '@comment_block_colon'],
              [/:([^:\n]|\\:)*:/, 'comment'],
              [/"([^"\\]|\\.)*$/, 'string.invalid'],
              [/"/, 'string', '@string_double'],
              [/'([^'\\]|\\.)*$/, 'string.invalid'],
              [/'/, 'string', '@string_single'],
              [/\b(f64|f32|i64|i32|i16|i8|u64|u32|u16|u8|i0|container|arr|dref|ref|ptr)\b/, 'type'],
              [/\b(as|extern|start|exit|function|return|if|else|while|switch|case|default|syscall|asm|not|ro|glob|lis|break|loop)\b/, 'keyword'],
              [/\b(0[xX][0-9a-fA-F]+|0[bB][01]+|0[0-7]+|[0-9]*\.?[0-9]+([eE][+-]?[0-9]+)?)\b/, 'number'],
              [/[a-zA-Z_][\w]*/, 'identifier'],
              [/[{}()\[\]]/, '@brackets'],
              [/[-+*/%=!<>|&.,;]/, 'delimiter']
            ],
            comment_block_slash: [
              [/\/:/, 'comment', '@pop'],
              [/./, 'comment'],
              [/\n/, 'comment']
            ],
            comment_block_colon: [
              [/^:/, 'comment', '@pop'],
              [/./, 'comment'],
              [/\n/, 'comment']
            ],
            string_double: [
              [/[^\\"]+/, 'string'],
              [/\\./, 'string.escape'],
              [/"/, 'string', '@pop']
            ],
            string_single: [
              [/[^\\']+/, 'string'],
              [/\\./, 'string.escape'],
              [/'/, 'string', '@pop']
            ]
          }
        });

        monaco.languages.setLanguageConfiguration('cpl', {
          comments: { blockComment: [':', ':'] },
          brackets: [['{', '}'], ['[', ']'], ['(', ')']],
          autoClosingPairs: [
            { open: '{', close: '}' },
            { open: '[', close: ']' },
            { open: '(', close: ')' },
            { open: '"', close: '"' },
            { open: "'", close: "'" }
          ],
          surroundingPairs: [
            { open: '{', close: '}' },
            { open: '[', close: ']' },
            { open: '(', close: ')' },
            { open: '"', close: '"' },
            { open: "'", close: "'" }
          ]
        });

        const cplHeaders = [
          'ctype_h.cpl','errno_h.cpl','fcntl_h.cpl','float_h.cpl','http_h.cpl',
          'inttypes_h.cpl','io_h.cpl','limits_h.cpl','list_h.cpl','locale_h.cpl',
          'math_h.cpl','platform_h.cpl','poll_h.cpl','queue_h.cpl','signal_h.cpl',
          'stack_h.cpl','stdbool_h.cpl','stddef_h.cpl','stdint_h.cpl','stdio_h.cpl',
          'stdlib_h.cpl','string_h.cpl','sys_stat_h.cpl','sys_types_h.cpl',
          'sys_wait_h.cpl','technoblade_h.cpl','time_h.cpl','town_h.cpl',
          'types_h.cpl','unistd_h.cpl'
        ];

        const cplKeywords = [
          'as','asm','break','case','container','default','dref','else','exit',
          'extern','function','glob','if','lis','loop','not','ptr','ref','return',
          'ro','start','switch','syscall','while'
        ];

        const cplTypes = [
          'i0','i8','i16','i32','i64','u8','u16','u32','u64','f32','f64',
          'arr','ptr','FILE','http_request','http_response','http_server',
          'linked_list','queue','stack','string'
        ];

        const cplConstants = [
          'EOF','NULL','O_RDONLY','O_WRONLY','O_RDWR','O_CREAT','O_TRUNC',
          'O_APPEND','SEEK_SET','SEEK_CUR','SEEK_END','STDIN_FILENO',
          'STDOUT_FILENO','STDERR_FILENO','HTTP_MAX_ROUTES','HTTP_REQUEST_BUFFER_SIZE'
        ];

        const cplStdlibFunctions = [
          { label: 'printf', detail: 'stdio_h.cpl', insertText: 'printf(ref "${1:text}\\n");', documentation: 'Formatted stdout. In the Playground, call fflush(stdout) before exit if you use stdio output.' },
          { label: 'puts', detail: 'stdio_h.cpl', insertText: 'puts(ref "${1:text}");', documentation: 'Writes a string and newline through C stdio.' },
          { label: 'fflush', detail: 'stdio_h.cpl', insertText: 'fflush(${1:stdout});', documentation: 'Flushes a C stdio stream.' },
          { label: 'fopen', detail: 'stdio_h.cpl', insertText: 'fopen(ref "${1:path}", ref "${2:r}")', documentation: 'Open a C FILE stream.' },
          { label: 'fclose', detail: 'stdio_h.cpl', insertText: 'fclose(${1:file});', documentation: 'Close a C FILE stream.' },
          { label: 'strlen', detail: 'string_h.cpl', insertText: 'strlen(${1:text})', documentation: 'Length of a null-terminated string.' },
          { label: 'strcmp', detail: 'string_h.cpl', insertText: 'strcmp(${1:left}, ${2:right})', documentation: 'Compare two null-terminated strings.' },
          { label: 'strcpy', detail: 'string_h.cpl', insertText: 'strcpy(${1:dst}, ${2:src});', documentation: 'Copy a string.' },
          { label: 'strcat', detail: 'string_h.cpl', insertText: 'strcat(${1:dst}, ${2:src});', documentation: 'Append a string.' },
          { label: 'memcpy', detail: 'string_h.cpl', insertText: 'memcpy(${1:dst}, ${2:src}, ${3:n});', documentation: 'Copy bytes.' },
          { label: 'memset', detail: 'string_h.cpl', insertText: 'memset(${1:dst}, ${2:value}, ${3:n});', documentation: 'Fill bytes.' },
          { label: 'malloc', detail: 'stdlib_h.cpl', insertText: 'malloc(${1:size})', documentation: 'Allocate memory.' },
          { label: 'free', detail: 'stdlib_h.cpl', insertText: 'free(${1:pointer});', documentation: 'Free allocated memory.' },
          { label: 'atoi', detail: 'stdlib_h.cpl', insertText: 'atoi(${1:text})', documentation: 'Parse integer text.' },
          { label: 'system', detail: 'stdlib_h.cpl', insertText: 'system(ref "${1:command}")', documentation: 'Run a shell command.' },
          { label: 'read', detail: 'unistd_h.cpl', insertText: 'read(${1:fd}, ${2:buffer}, ${3:size})', documentation: 'Read bytes from a file descriptor.' },
          { label: 'write', detail: 'unistd_h.cpl', insertText: 'write(${1:fd}, ${2:buffer}, ${3:size})', documentation: 'Write bytes to a file descriptor.' },
          { label: 'close', detail: 'unistd_h.cpl', insertText: 'close(${1:fd});', documentation: 'Close a file descriptor.' },
          { label: 'sleep', detail: 'unistd_h.cpl', insertText: 'sleep(${1:seconds});', documentation: 'Sleep for seconds.' },
          { label: 'getpid', detail: 'unistd_h.cpl', insertText: 'getpid()', documentation: 'Return the current process id.' },
          { label: 'open', detail: 'fcntl_h.cpl', insertText: 'open(ref "${1:path}", ${2:O_RDONLY})', documentation: 'Open a file descriptor.' },
          { label: 'creat', detail: 'fcntl_h.cpl', insertText: 'creat(ref "${1:path}", ${2:384})', documentation: 'Create or truncate a file.' }
        ];

        const cplSnippets = [
          {
            label: '#include <...>',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: '#include <${1:stdio_h.cpl}>\n$0',
            documentation: 'Include a cpllib header'
          },
          {
            label: 'start',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'start() {\n\t$0\n\texit 0;\n}',
            documentation: 'Program entry point'
          },
          {
            label: 'hello syscall',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'start() {\n\tsyscall(1, 1, ref "${1:Hello, World!}\\n", ${2:14});\n\texit 0;\n}',
            documentation: 'Linux stdout example that works without stdio buffering'
          },
          {
            label: 'hello printf',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: '#include <stdio_h.cpl>\n\nstart() {\n\tprintf(ref "${1:Hello!}\\n");\n\tfflush(stdout);\n\texit 0;\n}',
            documentation: 'stdio example with explicit fflush for Playground output'
          },
          {
            label: 'function',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'function ${1:name}(${2}) -> ${3:i0} {\n\t$0\n}',
            documentation: 'Function declaration'
          },
          {
            label: 'container',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'container ${1:name} {\n\t${2:i64 field};\n}',
            documentation: 'Container declaration'
          },
          {
            label: 'if',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'if ${1:condition}; {\n\t$0\n}',
            documentation: 'If block'
          },
          {
            label: 'if else',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'if ${1:condition}; {\n\t$2\n}\nelse {\n\t$0\n}',
            documentation: 'If/else block'
          },
          {
            label: 'while',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'while ${1:condition}; {\n\t$0\n}',
            documentation: 'While loop'
          },
          {
            label: 'loop',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'loop {\n\t$0\n}',
            documentation: 'Unconditional loop'
          },
          {
            label: 'switch',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: 'switch ${1:value}; {\n\tcase ${2:1}; {\n\t\t$3\n\t\tbreak;\n\t}\n\tdefault {\n\t\t$0\n\t\tbreak;\n\t}\n}',
            documentation: 'CPL switch block'
          },
          {
            label: 'http server',
            kind: monaco.languages.CompletionItemKind.Snippet,
            insertText: '#include <http_h.cpl>\n\n@[section(".bss")]\nglob http_server server;\n\nfunction root(ptr http_request req, ptr http_response res) -> i0 {\n\tres.text(200 as i32, ref "${1:Hello from CPL}\\n");\n}\n\nstart() {\n\tserver.init(ref "127.0.0.1", ${2:8080} as i32);\n\tserver.get(ref "/", root);\n\texit server.listen() as u8;\n}',
            documentation: 'Tiny cpllib/http_h.cpl server'
          }
        ];

        const cplAnnotations = [
          { label: '@[entry]', insertText: '@[entry("${1:main}")]', documentation: 'Mark function/start as entry symbol.' },
          { label: '@[section]', insertText: '@[section("${1:.data}")]', documentation: 'Place symbol into a section.' },
          { label: '@[align]', insertText: '@[align(${1:8})]', documentation: 'Request alignment.' },
          { label: '@[inline(always)]', insertText: '@[inline(always)]', documentation: 'Prefer always-inline.' },
          { label: '@[inline(never)]', insertText: '@[inline(never)]', documentation: 'Prevent inlining.' },
          { label: '@[abi]', insertText: '@[abi]', documentation: 'Use ABI-compatible function lowering.' },
          { label: '@[vname]', insertText: '@[vname("${1:symbol}")]', documentation: 'Set backend/linker-visible symbol name.' },
          { label: '@[like_c]', insertText: '@[like_c]', documentation: 'Use C-like container layout.' },
          { label: '@[no_fall]', insertText: '@[no_fall]', documentation: 'Make switch cases not fall through.' },
          { label: '@[straight]', insertText: '@[straight]', documentation: 'Use linear switch generation.' },
          { label: '@[counter]', insertText: '@[counter(${1:10})]', documentation: 'Counted loop annotation.' }
        ];

        function completionRange(model, position) {
          const word = model.getWordUntilPosition(position);
          return {
            startLineNumber: position.lineNumber,
            endLineNumber: position.lineNumber,
            startColumn: word.startColumn,
            endColumn: word.endColumn
          };
        }

        function withCommonCompletionFields(item, range, sortPrefix) {
          const insertText = item.insertText || '';
          const usesSnippetPlaceholders = /\$\{?\d/.test(insertText);
          return {
            ...item,
            range,
            sortText: sortPrefix + item.label,
            insertTextRules: item.kind === monaco.languages.CompletionItemKind.Snippet || usesSnippetPlaceholders
              ? monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
              : item.insertTextRules
          };
        }

        monaco.languages.registerCompletionItemProvider('cpl', {
          triggerCharacters: ['#', '<', '"', '@', '[', '.', ':'],
          provideCompletionItems: function(model, position) {
            const line = model.getLineContent(position.lineNumber);
            const prefix = line.slice(0, position.column - 1);
            const includeMatch = prefix.match(/^\s*#\s*include\s*([<"])([^>"]*)$/);

            if (includeMatch) {
              const closer = includeMatch[1] === '<' ? '>' : '"';
              const typed = includeMatch[2] || '';
              const range = {
                startLineNumber: position.lineNumber,
                endLineNumber: position.lineNumber,
                startColumn: position.column - typed.length,
                endColumn: position.column
              };

              return {
                suggestions: cplHeaders.map(header => ({
                  label: header,
                  kind: monaco.languages.CompletionItemKind.Module,
                  detail: 'cpllib header',
                  documentation: '#include <' + header + '>',
                  insertText: header + closer,
                  range,
                  sortText: '0' + header
                }))
              };
            }

            const annotationMatch = prefix.match(/(@\[?[A-Za-z_]*)$/);
            if (annotationMatch) {
              const range = {
                startLineNumber: position.lineNumber,
                endLineNumber: position.lineNumber,
                startColumn: position.column - annotationMatch[1].length,
                endColumn: position.column
              };

              return {
                suggestions: cplAnnotations.map(item => withCommonCompletionFields({
                  ...item,
                  kind: monaco.languages.CompletionItemKind.Property
                }, range, '0'))
              };
            }

            const range = completionRange(model, position);
            const suggestions = [
              ...cplSnippets.map(item => withCommonCompletionFields(item, range, '0')),
              ...cplAnnotations.map(item => withCommonCompletionFields({
                ...item,
                kind: monaco.languages.CompletionItemKind.Property
              }, range, '1')),
              ...cplKeywords.map(label => ({
                label,
                kind: monaco.languages.CompletionItemKind.Keyword,
                insertText: label,
                detail: 'CPL keyword',
                range,
                sortText: '2' + label
              })),
              ...cplTypes.map(label => ({
                label,
                kind: monaco.languages.CompletionItemKind.TypeParameter,
                insertText: label,
                detail: 'CPL type',
                range,
                sortText: '3' + label
              })),
              ...cplStdlibFunctions.map(item => withCommonCompletionFields({
                ...item,
                kind: monaco.languages.CompletionItemKind.Function
              }, range, '4')),
              ...cplConstants.map(label => ({
                label,
                kind: monaco.languages.CompletionItemKind.Constant,
                insertText: label,
                detail: 'cpllib constant',
                range,
                sortText: '5' + label
              })),
              ...cplHeaders.map(header => ({
                label: '#include <' + header + '>',
                kind: monaco.languages.CompletionItemKind.Module,
                insertText: '#include <' + header + '>',
                detail: 'cpllib header',
                range,
                sortText: '6' + header
              }))
            ];

            return { suggestions };
          }
        });

        monaco.editor.defineTheme('cpl-light', {
          base: 'vs-dark',
          inherit: true,
          rules: [
            { token: 'keyword', foreground: 'f46623', fontStyle: 'bold' },
            { token: 'type', foreground: 'ffc832' },
            { token: 'comment', foreground: 'a99b8f' },
            { token: 'string', foreground: 'd6b28d' },
            { token: 'number', foreground: 'f7d774' }
          ],
          colors: {
            'editor.background': '#1f1b18',
            'editor.foreground': '#f9f2ec',
            'editorLineNumber.foreground': '#8c7f74',
            'editorLineNumber.activeForeground': '#f46623',
            'editorIndentGuide.background1': '#3a312b',
            'editor.selectionBackground': '#6b3a24',
            'editor.inactiveSelectionBackground': '#3a2a22'
          }
        });
      }

      function simpleFormat(value) {
        const lines = value.replace(/\r\n/g, '\n').split('\n');
        let indent = 0;
        return lines.map(line => {
          const trimmed = line.trim();
          if (!trimmed) return '';
          if (/^[}\]]/.test(trimmed)) indent = Math.max(indent - 1, 0);
          const result = '\t'.repeat(indent) + trimmed;
          const opens = (trimmed.match(/[\{\[]/g) || []).length;
          const closes = (trimmed.match(/[\}\]]/g) || []).length;
          indent = Math.max(indent + opens - closes, 0);
          return result;
        }).join('\n');
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

        try {
          const monaco = await ensureMonaco();
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

          editors.set(block, editor);

          const resizeObserver = new ResizeObserver(() => editor.layout());
          resizeObserver.observe(block);
          block.__resizeObserver = resizeObserver;

          output.textContent = 'Click \'Run\' to compile and run the program.';
        } catch (err) {
          output.classList.add('is-error');
          output.textContent = 'Editor init failed: ' + err.message;
        }
      }

      async function runCpl(block) {
        const output= block.querySelector('.cpl-runner-output');
        const runButton= block.querySelector('[data-action="run"]');
        const editor= editors.get(block);
        if (!output || !runButton || !editor) return;
    
        runButton.disabled= true;
        output.classList.remove('is-error');
        output.textContent= 'Running...';
    
        const code= editor.getValue();
        const customRunner= window.CPL_RUNNER;
        const backendUrl= window.CPL_RUNNER_URL || '/cpl/run';
    
        try {
          if (customRunner && typeof customRunner.run === 'function') {
            const result= await customRunner.run({ lang: 'cpl', code, block, editor });
    
            if (typeof result === 'string') {
              output.textContent= result || 'Execution finished with empty output.';
            } else if (result && typeof result === 'object') {
              const parts= [];
              if (result.stdout) parts.push(result.stdout);
              if (result.stderr) parts.push(result.stderr);
              if (result.result && !parts.length) {
                parts.push(typeof result.result === 'string' ? result.result : JSON.stringify(result.result, null, 2));
              }
              output.textContent= parts.join('\n') || 'Execution finished with empty output.';
            } else {
              output.textContent= 'Execution finished.';
            }
            return;
          }

          let response = null;
          const payload = { lang: 'cpl', code };

          try {
            response= await fetch(backendUrl, {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json'
              },
              body: JSON.stringify(payload)
            });
          } catch (networkError) {
            if (window.CPL_RUNNER_STUB && typeof window.CPL_RUNNER_STUB === 'function') {
              const stubResult = await window.CPL_RUNNER_STUB(payload, networkError);
              output.textContent = stubResult.stdout || 'Backend stub completed.';
              return;
            }
            throw networkError;
          }
    
          let data= null;
          try {
            data= await response.json();
          } catch (e) {
            data= null;
          }
    
          if (!response.ok) {
            if ((response.status === 404 || response.status === 405) && window.CPL_RUNNER_STUB && typeof window.CPL_RUNNER_STUB === 'function') {
              const stubResult = await window.CPL_RUNNER_STUB(payload);
              output.textContent = stubResult.stdout || 'Backend stub completed.';
              return;
            }
            const message=
              (data && (data.error || data.stderr || data.message)) ||
              `Request failed with status ${response.status}`;
            throw new Error(message);
          }
    
          const parts= [];
          if (data) {
            if (data.stdout) parts.push(data.stdout);
            if (data.stderr) parts.push(data.stderr);
            if (data.result && !parts.length) {
              parts.push(typeof data.result === 'string' ? data.result : JSON.stringify(data.result, null, 2));
            }
          }
    
          output.textContent= parts.join('\n') || 'Execution finished with empty output.';
        } catch (err) {
          output.classList.add('is-error');
          output.textContent= 'Run failed: ' + err.message;
        } finally {
          runButton.disabled= false;
        }
      }

      function bindRunnerActions(block) {
        if (!block || block.dataset.bound === '1') return;
        block.dataset.bound = '1';

        const formatButton = block.querySelector('[data-action="format"]');
        const runButton = block.querySelector('[data-action="run"]');

        if (formatButton) {
          formatButton.addEventListener('click', function() {
            const editor = editors.get(block);
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

      hook.doneEach(function() {
        hydrateRunners(document);
      });

      document.addEventListener('cpl:content-ready', function(event) {
        hydrateRunners(event.detail && event.detail.root ? event.detail.root : document);
      });
    },

    function(hook) {
      hook.doneEach(function() {
        setTimeout(function() {
          renderBenchmarkCharts();
        }, 0);
      });

      document.addEventListener('cpl:content-ready', function() {
        setTimeout(renderBenchmarkCharts, 0);
      });
    }
  ]
};
