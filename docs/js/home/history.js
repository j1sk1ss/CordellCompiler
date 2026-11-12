export function initLanguageHistory() {
  const history = document.querySelector('.language-history');
  if (!history || history.dataset.ready === 'true') return;
  history.dataset.ready = 'true';
  const button = history.querySelector('.fold-sketch-button');
  const drawing = history.querySelector('.art-optimizer');
  button.addEventListener('click', function() {
    const folded = drawing.classList.toggle('is-folded');
    button.setAttribute('aria-pressed', String(folded));
    button.innerHTML = folded ? 'Reset <span aria-hidden="true">↶</span>' : 'Apply constant folding <span aria-hidden="true">↗</span>';
  });
}

export function scrollHomeAnchor() {
  if (!document.documentElement.classList.contains('cpl-home-route')) return;
  const query = (location.hash.split('?')[1] || '');
  const id = new URLSearchParams(query).get('id');
  if (!['language-history', 'era-title-draft', 'hero-title'].includes(id)) return;
  const target = document.getElementById(id);
  if (target) window.requestAnimationFrame(() => target.scrollIntoView({ block: 'start' }));
}
