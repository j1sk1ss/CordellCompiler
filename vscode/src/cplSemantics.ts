import { Range } from "vscode-languageserver/node";

export type MacroValue =
  | { kind: "string"; value: string }
  | { kind: "number"; value: number }
  | { kind: "raw"; text: string };

export type MacroSym = {
  kind: "macro";
  name: string;
  value: MacroValue;
  range: Range;
  valueRange: Range;
  doc?: string;
};

export type TypeNode =
  | { kind: "prim"; name: string }
  | { kind: "ptr"; to: TypeNode }
  | { kind: "arr"; len: number | null; elem: TypeNode }
  | { kind: "unknown" };

export type ParamSig = {
  name: string;
  type: TypeNode;
  hasDefault: boolean;
  isVarArgs?: boolean;
  range: Range;
};

export function formatType(t: TypeNode): string {
  switch (t.kind) {
    case "prim":
      return t.name;
    case "ptr":
      return `ptr ${formatType(t.to)}`;
    case "arr":
      return `arr[${t.len ?? "?"}, ${formatType(t.elem)}]`;
    default:
      return "?";
  }
}

export type Issue = { message: string; range: Range };

export type VarSym = {
  kind: "var";
  name: string;
  type: TypeNode;
  range: Range;
  readonly?: boolean;
};

export type FuncOverloadSym = {
  kind: "func";
  name: string;
  params: ParamSig[];
  ret: TypeNode;
  decls: Range[];
  def?: Range;
  primaryRange: Range;
  doc?: string;
};

export type CallSite = {
  name: string;
  argc: number;
  range: Range;
  resolution?: {
    status: "resolved" | "unknown" | "no_match" | "ambiguous";
    candidates: FuncOverloadSym[];
    selected?: FuncOverloadSym;
  };
};

export type FuncValueUse = {
  name: string;
  range: Range;
};

export type IndirectCallSite = {
  argc: number;
  range: Range;
  calleeType: TypeNode;
};

export function formatFunctionSignature(fn: FuncOverloadSym): string {
  const paramsStr = fn.params
    .map((p) => {
      if (p.isVarArgs) return "...";
      return `${formatType(p.type)} ${p.name}${p.hasDefault ? " = <default>" : ""}`;
    })
    .join(", ");

  const retStr = formatType(fn.ret);
  return retStr === "i0"
    ? `function ${fn.name}(${paramsStr})`
    : `function ${fn.name}(${paramsStr}) -> ${retStr}`;
}

class Scope {
  vars = new Map<string, VarSym>();
  constructor(public parent?: Scope) {}

  resolveVar(name: string): VarSym | undefined {
    for (let s: Scope | undefined = this; s; s = s.parent) {
      const hit = s.vars.get(name);
      if (hit) return hit;
    }
    return undefined;
  }
}

function sameType(a: TypeNode, b: TypeNode): boolean {
  if (a.kind !== b.kind) return false;
  switch (a.kind) {
    case "prim":
      return a.name === (b as any).name;
    case "ptr":
      return sameType(a.to, (b as any).to);
    case "arr":
      return a.len === (b as any).len && sameType(a.elem, (b as any).elem);
    case "unknown":
      return true;
  }
}

function isCallablePtrI0(t: TypeNode): boolean {
  return t.kind === "ptr" && t.to.kind === "prim" && t.to.name === "i0";
}

function sameParamIdentity(a: ParamSig[], b: ParamSig[]): boolean {
  const aVar = a.findIndex((p) => p.isVarArgs);
  const bVar = b.findIndex((p) => p.isVarArgs);
  if (aVar !== bVar) return false;

  const aLen = aVar >= 0 ? aVar : a.length;
  const bLen = bVar >= 0 ? bVar : b.length;
  if (aLen !== bLen) return false;

  for (let i = 0; i < aLen; i++) {
    if (a[i].hasDefault !== b[i].hasDefault) return false;
    if (!sameType(a[i].type, b[i].type)) return false;
  }

  return true;
}

function sameParamTypesOnly(a: ParamSig[], b: ParamSig[]): boolean {
  const aVar = a.findIndex((p) => p.isVarArgs);
  const bVar = b.findIndex((p) => p.isVarArgs);
  if (aVar !== bVar) return false;

  const aLen = aVar >= 0 ? aVar : a.length;
  const bLen = bVar >= 0 ? bVar : b.length;
  if (aLen !== bLen) return false;

  for (let i = 0; i < aLen; i++) {
    if (!sameType(a[i].type, b[i].type)) return false;
  }

  return true;
}

function arityBounds(params: ParamSig[]): { minArgs: number; maxArgs: number } {
  const varIndex = params.findIndex((p) => p.isVarArgs);
  const fixed = varIndex >= 0 ? params.slice(0, varIndex) : params;
  const minArgs = fixed.filter((p) => !p.hasDefault).length;
  const maxArgs = varIndex >= 0 ? Infinity : params.length;
  return { minArgs, maxArgs };
}

function matchesArity(fn: FuncOverloadSym, argc: number): boolean {
  const { minArgs, maxArgs } = arityBounds(fn.params);
  return argc >= minArgs && argc <= maxArgs;
}

function expectedArityString(fn: FuncOverloadSym): string {
  const { minArgs, maxArgs } = arityBounds(fn.params);
  if (maxArgs === Infinity) return `${minArgs}+`;
  if (minArgs === maxArgs) return `${minArgs}`;
  return `${minArgs}..${maxArgs}`;
}

export class SemanticContext {
  issues: Issue[] = [];

  funcs = new Map<string, FuncOverloadSym[]>();
  globals = new Map<string, VarSym>();

  macros = new Map<string, MacroSym>();
  macroDecls: MacroSym[] = [];
  macroUses: { name: string; range: Range }[] = [];

  varDecls: VarSym[] = [];
  varUses: { name: string; type: TypeNode; range: Range }[] = [];
  funcValueUses: FuncValueUse[] = [];
  callSites: CallSite[] = [];
  indirectCallSites: IndirectCallSite[] = [];

  private scope: Scope = new Scope();
  private pendingCalls: { name: string; argc: number; range: Range }[] = [];

  defineMacro(name: string, value: MacroValue, nameRange: Range, valueRange: Range, doc?: string) {
    if (this.macros.has(name)) {
      this.issues.push({ message: `Macro '${name}' already defined`, range: nameRange });
      return;
    }
    const sym: MacroSym = { kind: "macro", name, value, range: nameRange, valueRange, doc };
    this.macros.set(name, sym);
    this.macroDecls.push(sym);
  }

  useMacro(name: string, range: Range) {
    this.macroUses.push({ name, range });
  }

  getVarType(name: string): TypeNode | undefined {
    const v = this.scope.resolveVar(name) ?? this.globals.get(name);
    return v?.type;
  }

  hasFunctionNamed(name: string): boolean {
    const list = this.funcs.get(name);
    return !!(list && list.length > 0);
  }

  useVar(name: string, range: Range) {
    const v = this.scope.resolveVar(name) ?? this.globals.get(name);
    if (v) {
      this.varUses.push({ name, type: v.type, range });
      return;
    }

    const m = this.macros.get(name);
    if (m) {
      this.useMacro(name, range);
      return;
    }

    const overloads = this.funcs.get(name);
    if (overloads && overloads.length > 0) {
      this.funcValueUses.push({ name, range });
      return;
    }

    this.issues.push({ message: `Unknown variable '${name}'`, range });
  }

  enterScope() {
    this.scope = new Scope(this.scope);
  }

  exitScope() {
    if (this.scope.parent) this.scope = this.scope.parent;
  }

  declareGlobalVar(
    name: string,
    type: TypeNode,
    range: Range,
    opts?: { readonly?: boolean }
  ) {
    if (this.globals.has(name)) {
      this.issues.push({ message: `Global '${name}' already declared`, range });
    }
    const sym: VarSym = { kind: "var", name, type, range, readonly: opts?.readonly };
    this.globals.set(name, sym);
    this.varDecls.push(sym);
  }

  declareLocalVar(
    name: string,
    type: TypeNode,
    range: Range,
    opts?: { readonly?: boolean }
  ) {
    if (this.scope.vars.has(name)) {
      this.issues.push({ message: `Variable '${name}' already declared in this scope`, range });
    }
    const sym: VarSym = { kind: "var", name, type, range, readonly: opts?.readonly };
    this.scope.vars.set(name, sym);
    this.varDecls.push(sym);
  }

  declareFunc(
    name: string,
    params: ParamSig[],
    ret: TypeNode,
    range: Range,
    isDefinition: boolean,
    doc?: string
  ) {
    const list = this.funcs.get(name) ?? [];

    const exact = list.find((f) => sameParamIdentity(f.params, params));
    if (!exact) {
      const sameTypesDifferentDefaults = list.find(
        (f) => sameParamTypesOnly(f.params, params) && !sameParamIdentity(f.params, params)
      );
      if (sameTypesDifferentDefaults) {
        this.issues.push({
          message: `Function '${name}' overload differs only by default arguments`,
          range
        });
      }

      const sym: FuncOverloadSym = {
        kind: "func",
        name,
        params,
        ret,
        decls: isDefinition ? [] : [range],
        def: isDefinition ? range : undefined,
        primaryRange: range,
        doc
      };

      list.push(sym);
      this.funcs.set(name, list);
      return;
    }

    if (!sameType(exact.ret, ret)) {
      this.issues.push({
        message: `Function '${name}' overload with same parameters has different return type`,
        range
      });
      return;
    }

    if (doc && !exact.doc) exact.doc = doc;

    if (isDefinition) {
      if (exact.def) {
        this.issues.push({ message: `Function '${name}' overload already defined`, range });
        return;
      }
      exact.def = range;
      exact.primaryRange = range;
      return;
    }

    exact.decls.push(range);
    if (!exact.def && exact.decls.length === 1) exact.primaryRange = exact.decls[0];
  }

  noteCallSite(name: string, range: Range) {
    const existing = this.callSites.find(
      (c) =>
        c.name === name &&
        c.range.start.line === range.start.line &&
        c.range.start.character === range.start.character &&
        c.range.end.line === range.end.line &&
        c.range.end.character === range.end.character
    );

    if (!existing) {
      this.callSites.push({ name, argc: -1, range });
    }
  }

  private upsertCallSite(name: string, argc: number, range: Range, resolution?: CallSite["resolution"]) {
    const site = this.callSites.find(
      (c) =>
        c.name === name &&
        c.range.start.line === range.start.line &&
        c.range.start.character === range.start.character &&
        c.range.end.line === range.end.line &&
        c.range.end.character === range.end.character
    );

    if (site) {
      site.argc = argc;
      if (resolution) site.resolution = resolution;
      return;
    }

    this.callSites.push({ name, argc, range, resolution });
  }

  private resolveCall(name: string, argc: number): CallSite["resolution"] {
    const overloads = this.funcs.get(name) ?? [];
    if (overloads.length === 0) {
      return { status: "unknown", candidates: [] };
    }

    const arityMatches = overloads.filter((fn) => matchesArity(fn, argc));
    if (arityMatches.length === 0) {
      return { status: "no_match", candidates: overloads };
    }

    if (arityMatches.length === 1) {
      return { status: "resolved", candidates: arityMatches, selected: arityMatches[0] };
    }

    return { status: "ambiguous", candidates: arityMatches };
  }

  callFunc(name: string, argc: number, range: Range) {
    if (name === "syscall") return;
    this.pendingCalls.push({ name, argc, range });
    this.upsertCallSite(name, argc, range, this.resolveCall(name, argc));
  }

  callNamedOrValue(name: string, argc: number, range: Range) {
    if (name === "syscall") return;

    if (this.hasFunctionNamed(name)) {
      this.callFunc(name, argc, range);
      return;
    }

    const vt = this.getVarType(name);
    if (vt) {
      if (isCallablePtrI0(vt)) {
        this.indirectCallSites.push({ argc, range, calleeType: vt });
        return;
      }
      this.issues.push({
        message: `Expression '${name}' is not callable (expected ptr i0)`,
        range
      });
      return;
    }

    this.callFunc(name, argc, range);
  }

  callIndirectExpr(calleeType: TypeNode, argc: number, range: Range) {
    if (isCallablePtrI0(calleeType)) {
      this.indirectCallSites.push({ argc, range, calleeType });
      return;
    }

    this.issues.push({
      message: `Expression is not callable (expected ptr i0)`,
      range
    });
  }

  finish() {
    for (const c of this.pendingCalls) {
      const resolution = this.resolveCall(c.name, c.argc);
      if (resolution == undefined) continue;
      this.upsertCallSite(c.name, c.argc, c.range, resolution);

      if (resolution.status === "unknown") {
        this.issues.push({ message: `Unknown function '${c.name}'`, range: c.range });
        continue;
      }

      if (resolution.status === "no_match") {
        const expected = resolution.candidates
          .map(expectedArityString)
          .filter((v, i, a) => a.indexOf(v) === i)
          .join(" | ");

        this.issues.push({
          message: `Call '${c.name}': no matching overload for ${c.argc} args (available: ${expected || "none"})`,
          range: c.range
        });
        continue;
      }

    }

    this.pendingCalls = [];
  }
}
