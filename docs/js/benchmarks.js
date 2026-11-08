const charts = new Map();

export function destroyBenchmarkCharts() {
  for (const chart of charts.values()) {
    chart.destroy();
  }
  charts.clear();
}

export function renderBenchmarkCharts() {
  destroyBenchmarkCharts();

  const cards = document.querySelectorAll('.benchmark-card');
  if (typeof window.Chart === 'undefined') {
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

      const chart = new window.Chart(canvas.getContext('2d'), {
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

      charts.set(canvas, chart);
    } catch (err) {
      const wrap = card.querySelector('.benchmark-chart-wrap');
      if (wrap) {
        wrap.innerHTML = `<div class="benchmark-note">Chart error: ${err.message}</div>`;
      }
      console.error(err);
    }
  }
}


export function benchmarksPlugin(hook) {
  hook.beforeEach(content => {
    destroyBenchmarkCharts();
    return content;
  });
  hook.doneEach(renderBenchmarkCharts);
  document.addEventListener('cpl:content-ready', renderBenchmarkCharts);
}
