import { escapeHtml } from '../core/html.js';
import { highlightCpl } from '../syntax/highlight.js';

export const stages = {
  ast: { flag: 'ast', label: 'Syntax tree', unit: 'statements' },
  hir: { flag: 'ir', label: 'Typed values', unit: 'operations' },
  lir: { flag: 'lir', label: 'Virtual registers', unit: 'operations' },
  asm: { flag: 'asm', label: 'Linux x86-64', unit: 'instructions' }
};

export function compilerCommand(mode, stage) {
  const flag = stages[stage].flag;
  return `cplc trace.cpl ${mode.flags.join(' ')} --no-debug -no --emit-${flag} --${flag}-output trace.${stage} --arch x86_64 --sys-type linux64`;
}

export function renderSource(source) {
  return source.trimEnd().split('\n').map((line, index) =>
    `<div class="source-line" data-line="${index + 1}"><span class="source-number" aria-hidden="true">${index + 1}</span><code>${highlightCpl(line) || ' '}</code></div>`
  ).join('');
}

function sourceTag(line) {
  return `<span class="source-tag" aria-hidden="true">L${line}</span>`;
}

function expression(node) {
  if (!node) return '';
  if (!node.children.length) return `<span class="syntax-token">${escapeHtml(node.token)}</span>`;
  if (node.token === 'as') {
    return `${expression(node.children[1])}<span class="syntax-operator">as</span>${expression(node.children[0])}`;
  }
  return `${expression(node.children[0])}<span class="syntax-operator">${escapeHtml(node.token)}</span>${expression(node.children[1])}`;
}

function renderAst(rows) {
  const branches = rows.map((node, index) => {
    const declaration = node.token !== 'exit';
    const name = declaration ? `<small>${escapeHtml(node.token)}</small>${escapeHtml(node.children[0].token)}` : 'exit';
    const value = declaration ? node.children[1].children[0] : node.children[0];
    return `<li><button type="button" class="syntax-step" data-source-line="${index + 2}" title="Source line ${index + 2}"><span class="syntax-binding${declaration ? '' : ' syntax-exit'}">${name}</span><span class="syntax-connector" aria-hidden="true">${declaration ? '=' : '↳'}</span><span class="syntax-expression">${expression(value)}</span>${sourceTag(index + 2)}</button></li>`;
  }).join('');
  return `<div class="syntax-root"><span>start</span><small>scope</small></div><ol class="syntax-branches">${branches}</ol>`;
}

function renderHir(rows) {
  return `<div class="block-label">fn main()<span>typed / SSA values</span></div><div class="hir-rows">${rows.map(row =>
    `<button type="button" class="hir-row${row.type === 'exit' ? ' hir-return' : ''}" data-source-line="${row.line}" title="Source line ${row.line}"><span class="type-tag">${escapeHtml(row.type)}</span><b>${escapeHtml(row.target)}</b><span class="operation-sign" aria-hidden="true">${row.type === 'exit' ? '↗' : '←'}</span><span class="step-value">${escapeHtml(row.value)}</span>${sourceTag(row.line)}</button>`
  ).join('')}</div>`;
}

function renderLir(rows, baseline) {
  return `<div class="block-label">BB1<span>start</span></div><ol class="register-flow">${rows.map(row => {
    const changed = baseline.find(item => item.target === row.target)?.value !== row.value;
    return `<li><button type="button" class="register-step${row.target === 'exit' ? ' register-exit' : ''}${changed ? ' is-changed' : ''}" data-source-line="${row.line}" title="Source line ${row.line}"><b>${escapeHtml(row.target)}</b><span class="operation-sign" aria-hidden="true">${row.target === 'exit' ? '→' : '←'}</span><span class="step-value">${escapeHtml(row.value)}</span>${sourceTag(row.line)}</button></li>`;
  }).join('')}</ol><div class="block-end">BB2 <span>∅</span></div>`;
}

function renderAsm(rows, baseline) {
  return `<div class="block-label">main:<span>section .text / global main</span></div><ol class="assembly-flow">${rows.map(row => {
    const changed = !baseline.some(item => item.mnemonic === row.mnemonic && item.operands === row.operands);
    return `<li><button type="button" class="instruction${row.mnemonic === 'syscall' ? ' instruction-exit' : ''}${changed ? ' is-changed' : ''}" data-source-line="${row.line}" title="Source line ${row.line}"><b>${escapeHtml(row.mnemonic)}</b><span class="step-value">${escapeHtml(row.operands)}</span>${sourceTag(row.line)}</button></li>`;
  }).join('')}</ol>`;
}

export function renderCompilerOutput(mode, stage, baseline) {
  if (stage === 'ast') return renderAst(mode.ast);
  if (stage === 'hir') return renderHir(mode.hir);
  if (stage === 'lir') return renderLir(mode.lir, baseline.lir);
  return renderAsm(mode.asm, baseline.asm);
}
