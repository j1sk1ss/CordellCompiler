// One accessible selection/keyboard model for both example and compiler tabs.
export function initTabs(tabs, onSelect) {
  function select(tab) {
    tabs.forEach(item => {
      const selected = item === tab;
      item.setAttribute('aria-selected', String(selected));
      item.tabIndex = selected ? 0 : -1;
    });
    onSelect(tab);
  }

  tabs.forEach((tab, index) => {
    tab.addEventListener('click', () => select(tab));
    tab.addEventListener('keydown', event => {
      if (!['ArrowLeft', 'ArrowRight', 'Home', 'End'].includes(event.key)) return;
      event.preventDefault();
      const next = event.key === 'Home' ? 0 : event.key === 'End' ? tabs.length - 1 :
        (index + (event.key === 'ArrowRight' ? 1 : -1) + tabs.length) % tabs.length;
      tabs[next].focus();
      select(tabs[next]);
    });
  });

  const initial = tabs.find(tab => tab.getAttribute('aria-selected') === 'true') || tabs[0];
  if (initial) select(initial);
}
