export function configureRunner() {
  window.CPL_RUNNER_URL = window.CPL_RUNNER_URL || (
    /^(127\.0\.0\.1|localhost)$/.test(location.hostname)
      ? 'http://127.0.0.1:8000/cpl/run'
      : '/cpl/run'
  );
  window.CPL_RUNNER_STUB = window.CPL_RUNNER_STUB || async function(payload) {
    await new Promise(resolve => setTimeout(resolve, 280));
    return {
      success: true,
      stdout: [
        '=== backend stub ===',
        'POST ' + window.CPL_RUNNER_URL,
        'Compilation and execution endpoint is not connected in this environment yet.',
        '',
        'Payload accepted:',
        '- lang: ' + payload.lang,
        '- code size: ' + payload.code.length + ' bytes',
        '',
        'Production backend should return stdout/stderr/compiler metadata with the same JSON shape.'
      ].join('\n'),
      stderr: '',
      compiler_exit_code: 0,
      program_exit_code: 0
    };
  };
}

function resultText(result) {
  if (typeof result === 'string') return result || 'Execution finished with empty output.';
  if (!result || typeof result !== 'object') return 'Execution finished.';
  const parts = [];
  if (result.stdout) parts.push(result.stdout);
  if (result.stderr) parts.push(result.stderr);
  if (result.result && !parts.length) {
    parts.push(typeof result.result === 'string' ? result.result : JSON.stringify(result.result, null, 2));
  }
  return parts.join('\n') || 'Execution finished with empty output.';
}

export async function executeCpl({ code, block, editor }) {
  const payload = { lang: 'cpl', code };
  if (window.CPL_RUNNER && typeof window.CPL_RUNNER.run === 'function') {
    return resultText(await window.CPL_RUNNER.run({ ...payload, block, editor }));
  }

  let response;
  try {
    response = await fetch(window.CPL_RUNNER_URL || '/cpl/run', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });
  } catch (error) {
    if (typeof window.CPL_RUNNER_STUB === 'function') {
      const result = await window.CPL_RUNNER_STUB(payload, error);
      return result.stdout || 'Backend stub completed.';
    }
    throw error;
  }

  let data = null;
  try {
    data = await response.json();
  } catch (error) {
    // A failed backend may return a non-JSON error page.
  }

  if (!response.ok) {
    if ([404, 405].includes(response.status) && typeof window.CPL_RUNNER_STUB === 'function') {
      const result = await window.CPL_RUNNER_STUB(payload);
      return result.stdout || 'Backend stub completed.';
    }
    throw new Error((data && (data.error || data.stderr || data.message)) ||
      `Request failed with status ${response.status}`);
  }
  return data ? resultText(data) : 'Execution finished with empty output.';
}
