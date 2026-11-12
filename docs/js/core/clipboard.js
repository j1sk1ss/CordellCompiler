export async function copyText(value, selectionTarget) {
  try {
    if (navigator.clipboard && window.isSecureContext) {
      await navigator.clipboard.writeText(value);
      return true;
    }
  } catch (error) {
    // Fall back to the document clipboard when the API is unavailable or denied.
  }

  const textarea = document.createElement('textarea');
  textarea.value = value;
  textarea.style.cssText = 'position:fixed;top:-9999px';
  document.body.appendChild(textarea);
  textarea.select();
  let copied = false;
  try {
    copied = document.execCommand('copy');
  } catch (error) {
    copied = false;
  } finally {
    textarea.remove();
  }

  if (!copied && selectionTarget) {
    const selection = window.getSelection();
    const range = document.createRange();
    range.selectNodeContents(selectionTarget);
    selection.removeAllRanges();
    selection.addRange(range);
  }
  return copied;
}
