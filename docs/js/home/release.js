const repository = 'https://github.com/j1sk1ss/CordellCompiler';
const endpoint = 'https://api.github.com/repos/j1sk1ss/CordellCompiler/releases/latest';
const cacheKey = 'cpl:latest-release';
const cacheLifetime = 15 * 60 * 1000;
let pendingRelease;

function releaseDetails(data) {
  const url = new URL(data.html_url);
  if (typeof data.tag_name !== 'string' || !data.tag_name.trim() ||
      data.draft || data.prerelease || url.origin !== 'https://github.com' ||
      !url.pathname.startsWith('/j1sk1ss/CordellCompiler/releases/tag/')) {
    throw new Error('Invalid published release');
  }
  return {
    tag_name: data.tag_name.trim(),
    name: typeof data.name === 'string' ? data.name : data.tag_name,
    html_url: url.href,
    published_at: data.published_at
  };
}

function readCache() {
  try {
    const cached = JSON.parse(sessionStorage.getItem(cacheKey));
    const age = Date.now() - cached.checkedAt;
    if (age >= 0 && age < cacheLifetime) return releaseDetails(cached.release);
  } catch { /* Storage may be unavailable or contain an older record. */ }
  return null;
}

async function latestRelease(force) {
  const cached = !force && readCache();
  if (cached) return cached;
  if (pendingRelease) return pendingRelease;
  pendingRelease = (async () => {
    const controller = new AbortController();
    const timeout = setTimeout(() => controller.abort(), 8000);
    try {
      const response = await fetch(endpoint, {
        headers: { Accept: 'application/vnd.github+json' },
        credentials: 'omit',
        cache: 'no-cache',
        signal: controller.signal
      });
      if (!response.ok) throw new Error(`GitHub release: ${response.status}`);
      const release = releaseDetails(await response.json());
      try {
        sessionStorage.setItem(cacheKey, JSON.stringify({ checkedAt: Date.now(), release }));
      } catch { /* A blocked cache must not prevent showing the release. */ }
      return release;
    } finally {
      clearTimeout(timeout);
    }
  })().finally(() => { pendingRelease = null; });
  return pendingRelease;
}

export function initLatestRelease() {
  const card = document.querySelector('.release-card');
  if (!card || card.dataset.bound === 'true') return;
  card.dataset.bound = 'true';
  const version = card.querySelector('[data-release-version]');
  const name = card.querySelector('[data-release-name]');
  const date = card.querySelector('[data-release-date]');
  const status = card.querySelector('[data-release-status]');
  const link = card.querySelector('[data-release-link]');
  const retry = card.querySelector('[data-release-retry]');

  async function load(force = false) {
    card.setAttribute('aria-busy', 'true');
    card.dataset.state = 'loading';
    status.textContent = 'Checking GitHub…';
    date.hidden = true;
    retry.hidden = true;
    try {
      const release = await latestRelease(force);
      if (!card.isConnected) return;
      version.textContent = release.tag_name;
      version.href = release.html_url;
      name.textContent = release.name;
      link.href = release.html_url;
      const published = new Date(release.published_at);
      if (Number.isFinite(published.getTime())) {
        date.dateTime = published.toISOString();
        date.textContent = new Intl.DateTimeFormat('en', {
          month: 'short', day: 'numeric', year: 'numeric', timeZone: 'UTC'
        }).format(published);
        date.hidden = false;
      }
      status.textContent = '';
      card.dataset.state = 'ready';
    } catch {
      if (!card.isConnected) return;
      version.textContent = 'CPL';
      version.href = `${repository}/releases/latest`;
      name.textContent = 'Latest published release';
      link.href = `${repository}/releases/latest`;
      status.textContent = 'See the latest version on GitHub.';
      retry.hidden = false;
      card.dataset.state = 'error';
    } finally {
      card.setAttribute('aria-busy', 'false');
    }
  }

  retry.addEventListener('click', () => load(true));
  load();
}
