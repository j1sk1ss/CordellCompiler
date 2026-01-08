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

export type ParamSig = { name: string; type: TypeNode; hasDefault: boolean; range: Range };

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

type VarSym = { kind: "var"; name: string; type: TypeNode; range: Range };

type FuncSym = {
  kind: "func";
  name: string;
  params: ParamSig[];
  ret: TypeNode;
  decls: Range[];
  def?: Range;
  primaryRange: Range;
  doc?: string;
};

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

function sameParams(a: ParamSig[], b: ParamSig[]): boolean {
  if (a.length !== b.length) return false;
  for (let i = 0; i < a.length; i++) {
    if (a[i].hasDefault !== b[i].hasDefault) return false;
    if (!sameType(a[i].type, b[i].type)) return false;
  }
  return true;
}

export class SemanticContext {
  issues: Issue[] = [];

  funcs = new Map<string, FuncSym>();
  globals = new Map<string, VarSym>();

  macros = new Map<string, MacroSym>();
  macroDecls: MacroSym[] = [];
  macroUses: { name: string; range: Range }[] = [];

  varDecls: VarSym[] = [];
  varUses: { name: string; type: TypeNode; range: Range }[] = [];
  callSites: { name: string; range: Range }[] = [];

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

    this.issues.push({ message: `Unknown variable '${name}'`, range });
  }

  enterScope() {
    this.scope = new Scope(this.scope);
  }

  exitScope() {
    if (this.scope.parent) this.scope = this.scope.parent;
  }

  declareGlobalVar(name: string, type: TypeNode, range: Range) {
    if (this.globals.has(name)) {
      this.issues.push({ message: `Global '${name}' already declared`, range });
    }
    const sym: VarSym = { kind: "var", name, type, range };
    this.globals.set(name, sym);
    this.varDecls.push(sym);
  }

  declareLocalVar(name: string, type: TypeNode, range: Range) {
    if (this.scope.vars.has(name)) {
      this.issues.push({ message: `Variable '${name}' already declared in this scope`, range });
    }
    const sym: VarSym = { kind: "var", name, type, range };
    this.scope.vars.set(name, sym);
    this.varDecls.push(sym);
  }

  /**
   * isDefinition = true  -> function ... { ... }
   * isDefinition = false -> function ... ;
   */
  declareFunc(
    name: string,
    params: ParamSig[],
    ret: TypeNode,
    range: Range,
    isDefinition: boolean,
    doc?: string
  ) {
    const prev = this.funcs.get(name);

    if (!prev) {
      const sym: FuncSym = {
        kind: "func",
        name,
        params,
        ret,
        decls: isDefinition ? [] : [range],
        def: isDefinition ? range : undefined,
        primaryRange: range
      };
      
      this.funcs.set(name, sym);
      return;
    }

    if (doc && !prev.doc) prev.doc = doc;
    const sigOk = sameParams(prev.params, params) && sameType(prev.ret, ret);
    if (!sigOk) {
      this.issues.push({
        message: `Function '${name}' redeclared with different signature`,
        range
      });
      return;
    }

    if (isDefinition) {
      if (prev.def) {
        this.issues.push({ message: `Function '${name}' already defined`, range });
        return;
      }
      prev.def = range;
      prev.primaryRange = range; // definition wins for navigation
      return;
    }

    prev.decls.push(range);
    if (!prev.def && prev.decls.length === 1) prev.primaryRange = prev.decls[0];
  }

  noteCallSite(name: string, range: Range) {
    this.callSites.push({ name, range });
  }

  callFunc(name: string, argc: number, range: Range) {
    if (name === "syscall") return;

    const fn = this.funcs.get(name);
    if (!fn) {
      this.pendingCalls.push({ name, argc, range });
      return;
    }

    const minArgs = fn.params.filter(p => !p.hasDefault).length;
    const maxArgs = fn.params.length;

    if (argc < minArgs || argc > maxArgs) {
      const expected = (minArgs === maxArgs) ? `${minArgs}` : `${minArgs}..${maxArgs}`;
      this.issues.push({ message: `Call '${name}': expected ${expected} args, got ${argc}`, range });
    }
  }

  finish() {
    for (const c of this.pendingCalls) {
      const fn = this.funcs.get(c.name);
      if (!fn) {
        this.issues.push({ message: `Unknown function '${c.name}'`, range: c.range });
        continue;
      }

      const minArgs = fn.params.filter(p => !p.hasDefault).length;
      const maxArgs = fn.params.length;

      if (c.argc < minArgs || c.argc > maxArgs) {
        const expected = (minArgs === maxArgs) ? `${minArgs}` : `${minArgs}..${maxArgs}`;
        this.issues.push({ message: `Call '${c.name}': expected ${expected} args, got ${c.argc}`, range: c.range });
      }
    }
    this.pendingCalls = [];
  }
}
