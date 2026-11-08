import { disposeHomeMotion } from '../home/motion.js';
import { updateRouteClasses } from '../core/routes.js';
import { initHome } from '../home/index.js';
import { scrollHomeAnchor } from '../home/history.js';

export function pageLifecyclePlugin(hook) {
  hook.beforeEach(content => {
    disposeHomeMotion();
    return content;
  });
  hook.ready(updateRouteClasses);
  hook.doneEach(() => {
    updateRouteClasses();
    initHome();
    setTimeout(scrollHomeAnchor, 0);
  });
  window.addEventListener('hashchange', () => {
    updateRouteClasses();
    setTimeout(scrollHomeAnchor, 0);
  });
}
