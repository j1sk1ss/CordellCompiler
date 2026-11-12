export function renderCplRunnerMarkup(code) {
  const encoded = encodeURIComponent(code || '');
  return `
    <div class="cpl-runner" data-lang="cpl" data-code="${encoded}">
      <div class="cpl-runner-toolbar">
        <div class="cpl-runner-toolbar-left">
          <div class="cpl-runner-title">CPL playground</div>
          <span class="cpl-runner-badge">Compile and run via backend</span>
        </div>
        <div class="cpl-runner-toolbar-right">
          <button class="cpl-runner-button secondary" type="button" data-action="format">Format</button>
          <button class="cpl-runner-button" type="button" data-action="run">Run</button>
        </div>
      </div>
      <div class="cpl-runner-editor-shell">
        <div class="cpl-runner-tabs">
          <span class="cpl-runner-tab">main.cpl</span>
        </div>
        <div class="cpl-runner-editor"></div>
      </div>
      <pre class="cpl-runner-output">Ready to run. The browser will POST this code to the CPL backend.</pre>
    </div>
  `;
}
