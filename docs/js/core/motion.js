const animations = new WeakMap();
export function animateChange(element) {
  if (!element?.animate || window.matchMedia('(prefers-reduced-motion: reduce)').matches) return;
  animations.get(element)?.cancel();
  const animation = element.animate([
    { opacity: .35, transform: 'translateY(9px)' },
    { opacity: 1, transform: 'translateY(0)' }
  ], { duration: 260, easing: 'cubic-bezier(.2,.7,.3,1)' });
  animations.set(element, animation);
}
