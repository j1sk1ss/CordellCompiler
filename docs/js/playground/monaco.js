let monacoReadyPromise = null;

export function ensureMonaco() {
  if (window.monaco && window.require) return Promise.resolve(window.monaco);
  if (monacoReadyPromise) return monacoReadyPromise;

  monacoReadyPromise = new Promise((resolve, reject) => {
    const finish = () => {
      try {
        window.require.config({
          paths: {
            vs: 'https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/vs'
          }
        });
        window.MonacoEnvironment = {
          getWorkerUrl: function() {
            return 'data:text/javascript;charset=utf-8,' + encodeURIComponent(`self.MonacoEnvironment={baseUrl:"https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/"};importScripts("https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/vs/base/worker/workerMain.js");`);
          }
        };
        window.require(['vs/editor/editor.main'], function() {
          resolve(window.monaco);
        }, reject);
      } catch (err) {
        reject(err);
      }
    };

    if (window.require && window.require.config) {
      finish();
      return;
    }

    const script = document.createElement('script');
    script.src = 'https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/min/vs/loader.js';
    script.onload = finish;
    script.onerror = () => reject(new Error('Failed to load Monaco loader'));
    document.head.appendChild(script);
  });

  return monacoReadyPromise;
}
