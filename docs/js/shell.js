import { cleanRoute } from './core/routes.js';
import { scrollHomeAnchor } from './home/history.js';

export function initShell() {
  const toggle = document.querySelector('.menu-toggle');
  const nav = document.querySelector('.site-navigation');
  function closeMenu() {
    nav.classList.remove('is-open');
    toggle.setAttribute('aria-expanded', 'false');
  }
  toggle.addEventListener('click', function() {
    const open = nav.classList.toggle('is-open');
    toggle.setAttribute('aria-expanded', String(open));
  });
  nav.addEventListener('click', function(event) {
    if (event.target.closest('a')) closeMenu();
  });
  document.addEventListener('keydown', function(event) {
    if (event.key === 'Escape' && nav.classList.contains('is-open')) {
      closeMenu();
      toggle.focus();
    }
  });
  document.addEventListener('click', function(event) {
    if (!event.target.closest('.site-header')) closeMenu();
    const link = event.target.closest('a[href]');
    if (link && link.hash === location.hash && /^#\/\?id=(language-history|era-title-draft|hero-title)$/.test(link.hash)) {
      event.preventDefault();
      scrollHomeAnchor();
    }
  });
  document.querySelector('.skip-link').addEventListener('click', function(event) {
    event.preventDefault();
    document.querySelector('#main-content').focus();
  });
  function updateNavigation() {
    closeMenu();
    nav.querySelectorAll('a').forEach(link => {
      if (cleanRoute(link.getAttribute('href')) === cleanRoute(location.hash)) link.setAttribute('aria-current', 'page');
      else link.removeAttribute('aria-current');
    });
  }
  window.addEventListener('hashchange', updateNavigation);
  updateNavigation();
}
