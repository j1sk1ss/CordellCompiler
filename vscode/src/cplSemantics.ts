import { Range } from "vscode-languageserver/node";

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
type FuncSym = { kind: "func"; name: string; params: ParamSig[]; ret: TypeNode; range: Range };

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

export class SemanticContext {
  issues: Issue[] = [];

  funcs = new Map<string, FuncSym>();
  globals = new Map<string, VarSym>();

  varDecls: VarSym[] = [];
  varUses: { name: string; type: TypeNode; range: Range }[] = [];
  callSites: { name: string; range: Range }[] = [];

  private scope: Scope = new Scope();
  private pendingCalls: { name: string; argc: number; range: Range }[] = [];

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

  declareFunc(name: string, params: ParamSig[], ret: TypeNode, range: Range) {
    if (this.funcs.has(name)) {
      this.issues.push({ message: `Function '${name}' already declared`, range });
    }
    this.funcs.set(name, { kind: "func", name, params, ret, range });
  }

  useVar(name: string, range: Range) {
    const v = this.scope.resolveVar(name) ?? this.globals.get(name);
    if (!v) {
      this.issues.push({ message: `Unknown variable '${name}'`, range });
      return;
    }
    this.varUses.push({ name, type: v.type, range });
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
      const expected =
        (minArgs === maxArgs)
          ? `${minArgs}`
          : `${minArgs}..${maxArgs}`;
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
        const expected =
          (minArgs === maxArgs)
            ? `${minArgs}`
            : `${minArgs}..${maxArgs}`;
        this.issues.push({ message: `Call '${c.name}': expected ${expected} args, got ${c.argc}`, range: c.range });
      }
    }
    this.pendingCalls = [];
  }  
}
