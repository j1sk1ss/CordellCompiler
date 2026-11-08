export function cleanRoute(route) {
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


export function isHomeRoute(route = location.hash) {
  return ['', '#', '#/README', '#/README.md'].includes(cleanRoute(route));
}

export function updateRouteClasses() {
  const home = isHomeRoute();
  document.documentElement.classList.toggle('cpl-home-route', home);
  document.body.classList.toggle('cpl-home-route', home);
}
