let cleanup;

export function disposeHomeMotion() {
  cleanup?.();
  cleanup = null;
}

export function initHomeMotion() {
  disposeHomeMotion();
  const home = document.querySelector('.cpl-home');
  if (!home) return;
  const reduced = window.matchMedia('(prefers-reduced-motion: reduce)');
  if (reduced.matches) return;
  home.classList.add('motion-ready');
  const targets = Array.from(home.querySelectorAll('.csa-heading > *, .csa-command, .csa-lab, .csa-coverage > *, .no-defaults > *, .first-program > *, .section-heading, .language-item, .compiler-intro > *, .compiler-inspector, .guide-intro, .guide-links > a, .history-heading, .milestone-card, .get-started-section > *'));
  let observer;
  if ('IntersectionObserver' in window) {
    observer = new IntersectionObserver(entries => {
      entries.forEach(entry => {
        if (!entry.isIntersecting) return;
        entry.target.classList.add('is-revealed');
        observer.unobserve(entry.target);
      });
    }, { threshold: .05, rootMargin: '0px 0px -25px 0px' });
    targets.forEach((target, index) => {
      target.classList.add('motion-enter');
      target.style.setProperty('--reveal-delay', `${index % 3 * 65}ms`);
      observer.observe(target);
    });
  }

  const hero = home.querySelector('.home-hero');
  const art = hero.querySelector('.hero-art');
  let frame;
  function resetArt() {
    cancelAnimationFrame(frame);
    art.style.setProperty('--art-x', '0px');
    art.style.setProperty('--art-y', '0px');
  }
  function moveArt(event) {
    if (reduced.matches || event.pointerType !== 'mouse') return;
    cancelAnimationFrame(frame);
    const bounds = hero.getBoundingClientRect();
    const x = (event.clientX - bounds.left) / bounds.width - .5;
    const y = (event.clientY - bounds.top) / bounds.height - .5;
    frame = requestAnimationFrame(() => {
      art.style.setProperty('--art-x', `${x * 12}px`);
      art.style.setProperty('--art-y', `${y * 9}px`);
    });
  }
  function revealFocused(event) {
    event.target.closest('.motion-enter')?.classList.add('is-revealed');
  }
  function changePreference() {
    if (!reduced.matches) return;
    observer?.disconnect();
    targets.forEach(target => target.classList.add('is-revealed'));
    home.classList.remove('motion-ready');
    home.getAnimations({ subtree: true }).forEach(animation => animation.cancel());
    resetArt();
  }
  hero.addEventListener('pointermove', moveArt);
  hero.addEventListener('pointerleave', resetArt);
  home.addEventListener('focusin', revealFocused);
  reduced.addEventListener('change', changePreference);
  cleanup = () => {
    observer?.disconnect();
    resetArt();
    hero.removeEventListener('pointermove', moveArt);
    hero.removeEventListener('pointerleave', resetArt);
    home.removeEventListener('focusin', revealFocused);
    reduced.removeEventListener('change', changePreference);
  };
}
