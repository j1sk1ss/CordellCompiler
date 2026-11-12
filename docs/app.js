// Keep setup before vendor modules: Docsify reads $docsify during startup.
// Static imports finish evaluating in this order before its DOM-ready callback.
import './js/docsify/setup.js';
import 'https://cdn.jsdelivr.net/npm/chart.js@4.4.8/dist/chart.umd.min.js';
import 'https://cdn.jsdelivr.net/npm/docsify@4/lib/docsify.min.js';
import 'https://cdn.jsdelivr.net/npm/docsify@4/lib/plugins/search.min.js';
import 'https://cdn.jsdelivr.net/npm/docsify-copy-code@2/dist/docsify-copy-code.min.js';
