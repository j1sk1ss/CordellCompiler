import { Range } from "vscode-languageserver/node";

export type MacroValue =
  | { kind: "string"; value: string }
  | { kind: "number"; value: number }
  | { kind: "raw"; text: string };

export type MacroCondition = {
  name: string;
  isDefined: boolean;
};

export type MacroSym = {
  kind: "macro";
  name: string;
  value: MacroValue;
  range: Range;
  valueRange: Range;
  filePath?: string;
  doc?: string;
  conditions?: MacroCondition[];
};

export type TypeNode =
  | { kind: "prim"; name: string }
  | { kind: "ptr"; to: TypeNode }
  | { kind: "arr"; len: number | null; elem: TypeNode }
  | { kind: "func"; params: TypeNode[]; ret: TypeNode }
  | { kind: "container"; name: string }
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
    case "func":
      return `(${t.params.map(formatType).join(", ")}) => ${formatType(t.ret)}`;
    case "container":
      return t.name;
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
  filePath?: string;
  readonly?: boolean;
};

export type FuncOverloadSym = {
  kind: "func";
  name: string;
  containerName?: string;
  isSelfMethod?: boolean;
  typeParams?: string[];
  params: ParamSig[];
  ret: TypeNode;
  decls: Range[];
  declFiles?: (string | undefined)[];
  def?: Range;
  defFilePath?: string;
  primaryRange: Range;
  primaryFilePath?: string;
  doc?: string;
};

export type ContainerSym = {
  kind: "container";
  name: string;
  fields: Map<string, VarSym>;
  methods: Map<string, FuncOverloadSym[]>;
  range: Range;
  filePath?: string;
  doc?: string;
};

export type ContainerUse = {
  name: string;
  range: Range;
  filePath?: string;
  targetRange?: Range;
  targetFilePath?: string;
};

export type CallSite = {
  name: string;
  argc: number;
  range: Range;
  filePath?: string;
  resolution?: {
    status: "resolved" | "unknown" | "no_match" | "ambiguous";
    candidates: FuncOverloadSym[];
    selected?: FuncOverloadSym;
  };
};

export type FuncValueUse = {
  name: string;
  range: Range;
  filePath?: string;
};

export type IndirectCallSite = {
  argc: number;
  range: Range;
  filePath?: string;
  calleeType: TypeNode;
};

export type SizeofSite = {
  range: Range;
  filePath?: string;
  targetType: TypeNode;
  size: number | null;
};

export function sizeofType(t: TypeNode, opts?: { pointerSize?: number }): number | undefined {
  const pointerSize = opts?.pointerSize ?? 8;

  switch (t.kind) {
    case "prim":
      switch (t.name) {
        case "i0": return 0;
        case "i8":
        case "u8": return 1;
        case "i16":
        case "u16": return 2;
        case "i32":
        case "u32":
        case "f32": return 4;
        case "i64":
        case "u64":
        case "f64": return 8;
        case "str": return pointerSize;
        default: return undefined;
      }
    case "ptr":
      return pointerSize;
    case "arr": {
      if (t.len == null) return undefined;
      const elem = sizeofType(t.elem, opts);
      return elem == null ? undefined : t.len * elem;
    }
    case "func":
      return pointerSize;
    case "container":
      return undefined;
    case "unknown":
      return undefined;
  }
}

export function formatFunctionSignature(fn: FuncOverloadSym): string {
  const displayName = fn.containerName ? `${fn.containerName}::${fn.name}` : fn.name;
  const paramsStr = fn.params
    .map((p) => {
      if (p.isVarArgs) return "...";
      return `${formatType(p.type)} ${p.name}${p.hasDefault ? " = <default>" : ""}`;
    })
    .join(", ");

  const retStr = formatType(fn.ret);
  const typeParamsStr = fn.typeParams?.length ? `<${fn.typeParams.join(", ")}>` : "";
  return retStr === "i0"
    ? `function ${displayName}${typeParamsStr}(${paramsStr})`
    : `function ${displayName}${typeParamsStr}(${paramsStr}) -> ${retStr}`;
}

class Scope {
  vars = new Map<string, VarSym>();
  funcs = new Map<string, FuncOverloadSym[]>();
  constructor(public parent?: Scope) {}

  resolveVar(name: string): VarSym | undefined {
    for (let s: Scope | undefined = this; s; s = s.parent) {
      const hit = s.vars.get(name);
      if (hit) return hit;
    }
    return undefined;
  }

  resolveFuncs(name: string): FuncOverloadSym[] {
    for (let s: Scope | undefined = this; s; s = s.parent) {
      const hit = s.funcs.get(name);
      if (hit && hit.length) return hit;
    }
    return [];
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
    case "func": {
      const bb = b as any;
      return a.params.length === bb.params.length
        && a.params.every((p, i) => sameType(p, bb.params[i]))
        && sameType(a.ret, bb.ret);
    }
    case "container":
      return a.name === (b as any).name;
    case "unknown":
      return true;
  }
}

function isCallableType(t: TypeNode): boolean {
  return t.kind === "func"
    || (t.kind === "ptr" && t.to.kind === "prim" && t.to.name === "i0");
}

function matchesCallableArity(t: TypeNode, argc: number): boolean {
  if (t.kind === "func") return t.params.length === argc;
  return true;
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

const EXCLUSIVE_MACRO_GROUPS: string[][] = [
  ["CCPL_MACHO64", "CCPL_GNU64", "CCPL_GNUI386", "CCPL_WINDOWS64"]
];

type DefineMacroOptions = {
  doc?: string;
  conditions?: MacroCondition[];
};

function normalizeMacroConditions(conditions: MacroCondition[] = []): Map<string, boolean> | undefined {
  const result = new Map<string, boolean>();

  for (const cond of conditions) {
    const prev = result.get(cond.name);
    if (prev !== undefined && prev !== cond.isDefined) return undefined;
    result.set(cond.name, cond.isDefined);
  }

  for (const group of EXCLUSIVE_MACRO_GROUPS) {
    let requiredDefined = 0;
    for (const name of group) {
      if (result.get(name) === true) requiredDefined++;
    }
    if (requiredDefined > 1) return undefined;
  }

  return result;
}

function macroConditionsCompatible(a: MacroCondition[] = [], b: MacroCondition[] = []): boolean {
  return normalizeMacroConditions([...a, ...b]) !== undefined;
}

function macroConditionsSatisfiable(conditions: MacroCondition[] = []): boolean {
  return normalizeMacroConditions(conditions) !== undefined;
}

export class SemanticContext {
  issues: Issue[] = [];

  funcs = new Map<string, FuncOverloadSym[]>();
  globals = new Map<string, VarSym>();
  containers = new Map<string, ContainerSym>();
  containerDecls: ContainerSym[] = [];
  containerUses: ContainerUse[] = [];

  macros = new Map<string, MacroSym>();
  macroDecls: MacroSym[] = [];
  macroUses: { name: string; range: Range; filePath?: string }[] = [];

  varDecls: VarSym[] = [];
  varUses: { name: string; type: TypeNode; range: Range; filePath?: string; targetRange?: Range; targetFilePath?: string }[] = [];
  funcValueUses: FuncValueUse[] = [];
  callSites: CallSite[] = [];
  indirectCallSites: IndirectCallSite[] = [];
  sizeofSites: SizeofSite[] = [];

  private currentFilePath: string | undefined;
  private scope: Scope = new Scope();
  private pendingCalls: { name: string; argc: number; range: Range; filePath?: string; scope: Scope }[] = [];
  private pendingAssociatedCalls: { containerName: string; name: string; argc: number; range: Range; filePath?: string }[] = [];

  setCurrentFilePath(filePath?: string): string | undefined {
    const prev = this.currentFilePath;
    this.currentFilePath = filePath;
    return prev;
  }

  getCurrentFilePath(): string | undefined {
    return this.currentFilePath;
  }

  hasContainer(name: string): boolean {
    return this.containers.has(name);
  }

  getContainer(name: string): ContainerSym | undefined {
    return this.containers.get(name);
  }

  useContainer(name: string, range: Range) {
    const c = this.containers.get(name);
    if (!c) return;

    this.containerUses.push({
      name,
      range,
      filePath: this.currentFilePath,
      targetRange: c.range,
      targetFilePath: c.filePath
    });
  }

  containerTypeForName(name: string): TypeNode {
    return this.containers.has(name) ? { kind: "container", name } : { kind: "prim", name };
  }

  private qualifiedMethodName(containerName: string, name: string): string {
    return `${containerName}::${name}`;
  }

  private isInstanceMethod(fn: FuncOverloadSym): boolean {
    return !!fn.isSelfMethod || fn.params[0]?.name === "self";
  }

  private isAssociatedMethod(fn: FuncOverloadSym): boolean {
    return !this.isInstanceMethod(fn);
  }

  private functionTypeFromMethod(fn: FuncOverloadSym, omitSelf: boolean): TypeNode {
    const hasExplicitSelfParam = fn.params[0]?.name === "self";
    const params = omitSelf && hasExplicitSelfParam ? fn.params.slice(1) : fn.params;
    return {
      kind: "func",
      params: params.filter((p) => !p.isVarArgs).map((p) => p.type),
      ret: fn.ret
    };
  }

  private containerNameFromType(t: TypeNode): string | undefined {
    if (t.kind === "container") return t.name;
    if (t.kind === "prim" && this.containers.has(t.name)) return t.name;

    if (t.kind === "ptr") {
      const inner = this.containerNameFromType(t.to);
      if (inner) return inner;
    }

    return undefined;
  }

  declareContainer(name: string, range: Range, doc?: string) {
    if (this.containers.has(name)) {
      this.issues.push({ message: `Container '${name}' already declared`, range });
      return;
    }

    const sym: ContainerSym = {
      kind: "container",
      name,
      fields: new Map<string, VarSym>(),
      methods: new Map<string, FuncOverloadSym[]>(),
      range,
      filePath: this.currentFilePath,
      doc
    };

    this.containers.set(name, sym);
    this.containerDecls.push(sym);
  }

  declareContainerField(containerName: string, name: string, type: TypeNode, range: Range, opts?: { readonly?: boolean }) {
    const container = this.containers.get(containerName);
    if (!container) {
      this.issues.push({ message: `Unknown container '${containerName}'`, range });
      return;
    }

    if (container.fields.has(name)) {
      this.issues.push({ message: `Field '${containerName}.${name}' already declared`, range });
      return;
    }

    const sym: VarSym = { kind: "var", name, type, range, filePath: this.currentFilePath, readonly: opts?.readonly };
    container.fields.set(name, sym);
    this.varDecls.push(sym);
  }

  declareContainerMethod(
    containerName: string,
    name: string,
    params: ParamSig[],
    ret: TypeNode,
    range: Range,
    isDefinition: boolean,
    doc?: string,
    typeParams?: string[],
    opts?: { self?: boolean }
  ) {
    const container = this.containers.get(containerName);
    if (!container) {
      this.issues.push({ message: `Unknown container '${containerName}'`, range });
      return;
    }

    const local = container.methods.get(name) ?? [];
    const exact = local.find((f) => sameParamIdentity(f.params, params));

    if (!exact) {
      const sameTypesDifferentDefaults = local.find(
        (f) => sameParamTypesOnly(f.params, params) && !sameParamIdentity(f.params, params)
      );
      if (sameTypesDifferentDefaults) {
        this.issues.push({
          message: `Method '${containerName}.${name}' overload differs only by default arguments`,
          range
        });
      }

      const sym: FuncOverloadSym = {
        kind: "func",
        name,
        containerName,
        isSelfMethod: opts?.self || params[0]?.name === "self",
        typeParams,
        params,
        ret,
        decls: isDefinition ? [] : [range],
        declFiles: isDefinition ? [] : [this.currentFilePath],
        def: isDefinition ? range : undefined,
        defFilePath: isDefinition ? this.currentFilePath : undefined,
        primaryRange: range,
        primaryFilePath: this.currentFilePath,
        doc
      };

      local.push(sym);
      container.methods.set(name, local);

      const qualifiedName = this.qualifiedMethodName(containerName, name);
      const all = this.funcs.get(qualifiedName) ?? [];
      all.push(sym);
      this.funcs.set(qualifiedName, all);
      return;
    }

    if (!sameType(exact.ret, ret)) {
      this.issues.push({
        message: `Method '${containerName}.${name}' overload with same parameters has different return type`,
        range
      });
      return;
    }

    if (doc && !exact.doc) exact.doc = doc;
    if (opts?.self) exact.isSelfMethod = true;

    if (isDefinition) {
      if (exact.def) {
        this.issues.push({ message: `Method '${containerName}.${name}' overload already defined`, range });
        return;
      }
      exact.def = range;
      exact.defFilePath = this.currentFilePath;
      exact.primaryRange = range;
      exact.primaryFilePath = this.currentFilePath;
      return;
    }

    exact.decls.push(range);
    (exact.declFiles ??= []).push(this.currentFilePath);
    if (!exact.def && exact.decls.length === 1) {
      exact.primaryRange = exact.decls[0];
      exact.primaryFilePath = this.currentFilePath;
    }
  }

  getContainerMemberType(baseType: TypeNode, memberName: string, range: Range): TypeNode {
    const containerName = this.containerNameFromType(baseType);
    if (!containerName) {
      if (baseType.kind !== "unknown") {
        this.issues.push({ message: `Type '${formatType(baseType)}' has no members`, range });
      }
      return { kind: "unknown" };
    }

    const container = this.containers.get(containerName);
    if (!container) return { kind: "unknown" };

    const field = container.fields.get(memberName);
    if (field) {
      this.varUses.push({
        name: `${containerName}.${memberName}`,
        type: field.type,
        range,
        filePath: this.currentFilePath,
        targetRange: field.range,
        targetFilePath: field.filePath
      });
      return field.type;
    }

    const methods = container.methods.get(memberName) ?? [];
    if (methods.length === 1) {
      const fn = methods[0];
      return this.functionTypeFromMethod(fn, this.isInstanceMethod(fn));
    }

    if (methods.length > 1) {
      return { kind: "ptr", to: { kind: "prim", name: "i0" } };
    }

    this.issues.push({ message: `Unknown member '${containerName}.${memberName}'`, range });
    return { kind: "unknown" };
  }

  getAssociatedMemberType(containerName: string, memberName: string, range: Range): TypeNode {
    const container = this.containers.get(containerName);
    if (!container) {
      this.issues.push({ message: `Unknown container '${containerName}'`, range });
      return { kind: "unknown" };
    }

    const methods = container.methods.get(memberName) ?? [];
    const associatedMethods = methods.filter((fn) => this.isAssociatedMethod(fn));

    if (associatedMethods.length === 1) {
      return this.functionTypeFromMethod(associatedMethods[0], false);
    }

    if (associatedMethods.length > 1) {
      return { kind: "ptr", to: { kind: "prim", name: "i0" } };
    }

    return { kind: "unknown" };
  }

  defineMacro(
    name: string,
    value: MacroValue,
    nameRange: Range,
    valueRange: Range,
    optsOrDoc?: string | DefineMacroOptions
  ) {
    const opts: DefineMacroOptions =
      typeof optsOrDoc === "string" ? { doc: optsOrDoc } : (optsOrDoc ?? {});
    const conditions = opts.conditions ?? [];

    if (!macroConditionsSatisfiable(conditions)) return;

    const compatiblePrevious = this.macroDecls.find((m) =>
      m.name === name && macroConditionsCompatible(m.conditions, conditions)
    );

    if (compatiblePrevious) {
      this.issues.push({ message: `Macro '${name}' already defined`, range: nameRange });
      return;
    }

    const sym: MacroSym = {
      kind: "macro",
      name,
      value,
      range: nameRange,
      valueRange,
      filePath: this.currentFilePath,
      doc: opts.doc,
      conditions: conditions.length ? [...conditions] : undefined
    };

    const current = this.macros.get(name);
    if (!current || ((current.conditions?.length ?? 0) > 0 && conditions.length === 0)) {
      this.macros.set(name, sym);
    }

    this.macroDecls.push(sym);
  }

  useMacro(name: string, range: Range) {
    this.macroUses.push({ name, range, filePath: this.currentFilePath });
  }

  getVarType(name: string): TypeNode | undefined {
    const v = this.scope.resolveVar(name) ?? this.globals.get(name);
    return v?.type;
  }

  hasFunctionNamed(name: string): boolean {
    const list = this.scope.resolveFuncs(name);
    return list.length > 0;
  }

  getFunctions(name: string): FuncOverloadSym[] {
    return this.scope.resolveFuncs(name);
  }

  getFunctionValueType(name: string): TypeNode | undefined {
    const overloads = this.getFunctions(name);
    if (overloads.length === 1) {
      return {
        kind: "func",
        params: overloads[0].params.filter((p) => !p.isVarArgs).map((p) => p.type),
        ret: overloads[0].ret
      };
    }
    if (overloads.length > 1) {
      return { kind: "ptr", to: { kind: "prim", name: "i0" } };
    }
    return undefined;
  }

  useVar(name: string, range: Range) {
    const v = this.scope.resolveVar(name) ?? this.globals.get(name);
    if (v) {
      this.varUses.push({
        name,
        type: v.type,
        range,
        filePath: this.currentFilePath,
        targetRange: v.range,
        targetFilePath: v.filePath
      });
      return;
    }

    const m = this.macros.get(name);
    if (m) {
      this.useMacro(name, range);
      return;
    }

    const overloads = this.scope.resolveFuncs(name);
    if (overloads.length > 0) {
      this.funcValueUses.push({ name, range, filePath: this.currentFilePath });
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

  declareExternGlobalVar(
    name: string,
    type: TypeNode,
    range: Range,
    opts?: { readonly?: boolean }
  ) {
    const existing = this.globals.get(name);
    if (existing) {
      if (!existing.filePath && this.currentFilePath) existing.filePath = this.currentFilePath;
      return;
    }

    const sym: VarSym = { kind: "var", name, type, range, filePath: this.currentFilePath, readonly: opts?.readonly };
    this.globals.set(name, sym);
    this.varDecls.push(sym);
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
    const sym: VarSym = { kind: "var", name, type, range, filePath: this.currentFilePath, readonly: opts?.readonly };
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
    const sym: VarSym = { kind: "var", name, type, range, filePath: this.currentFilePath, readonly: opts?.readonly };
    this.scope.vars.set(name, sym);
    this.varDecls.push(sym);
  }

  declareFunc(
    name: string,
    params: ParamSig[],
    ret: TypeNode,
    range: Range,
    isDefinition: boolean,
    doc?: string,
    typeParams?: string[]
  ) {
    const local = this.scope.funcs.get(name) ?? [];

    const exact = local.find((f) => sameParamIdentity(f.params, params));
    if (!exact) {
      const sameTypesDifferentDefaults = local.find(
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
        typeParams,
        params,
        ret,
        decls: isDefinition ? [] : [range],
        declFiles: isDefinition ? [] : [this.currentFilePath],
        def: isDefinition ? range : undefined,
        defFilePath: isDefinition ? this.currentFilePath : undefined,
        primaryRange: range,
        primaryFilePath: this.currentFilePath,
        doc
      };

      local.push(sym);
      this.scope.funcs.set(name, local);
      const all = this.funcs.get(name) ?? [];
      all.push(sym);
      this.funcs.set(name, all);
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
      exact.defFilePath = this.currentFilePath;
      exact.primaryRange = range;
      exact.primaryFilePath = this.currentFilePath;
      return;
    }

    exact.decls.push(range);
    (exact.declFiles ??= []).push(this.currentFilePath);
    if (!exact.def && exact.decls.length === 1) {
      exact.primaryRange = exact.decls[0];
      exact.primaryFilePath = this.currentFilePath;
    }
  }

  noteCallSite(name: string, range: Range) {
    const existing = this.callSites.find(
      (c) =>
        c.name === name &&
        c.range.start.line === range.start.line &&
        c.range.start.character === range.start.character &&
        c.range.end.line === range.end.line &&
        c.range.end.character === range.end.character &&
        c.filePath === this.currentFilePath
    );

    if (!existing) {
      this.callSites.push({ name, argc: -1, range, filePath: this.currentFilePath });
    }
  }

  private upsertCallSite(
    name: string,
    argc: number,
    range: Range,
    resolution?: CallSite["resolution"],
    filePath: string | undefined = this.currentFilePath
  ) {
    const site = this.callSites.find(
      (c) =>
        c.name === name &&
        c.range.start.line === range.start.line &&
        c.range.start.character === range.start.character &&
        c.range.end.line === range.end.line &&
        c.range.end.character === range.end.character &&
        c.filePath === filePath
    );

    if (site) {
      site.argc = argc;
      site.filePath = filePath;
      if (resolution) site.resolution = resolution;
      return;
    }

    this.callSites.push({ name, argc, range, filePath, resolution });
  }

  private resolveCall(name: string, argc: number, scope: Scope = this.scope): CallSite["resolution"] {
    const overloads = scope.resolveFuncs(name);
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

  private resolveAssociatedCall(containerName: string, name: string, argc: number): CallSite["resolution"] {
    const container = this.containers.get(containerName);
    if (!container) {
      return { status: "unknown", candidates: [] };
    }

    const allMethods = container.methods.get(name) ?? [];
    const overloads = allMethods.filter((fn) => this.isAssociatedMethod(fn));
    if (overloads.length === 0) {
      return allMethods.length > 0
        ? { status: "no_match", candidates: allMethods }
        : { status: "unknown", candidates: [] };
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
    this.pendingCalls.push({ name, argc, range, filePath: this.currentFilePath, scope: this.scope });
    this.upsertCallSite(name, argc, range, this.resolveCall(name, argc, this.scope));
  }

  callNamedOrValue(name: string, argc: number, range: Range) {
    if (name === "syscall") return;

    if (this.hasFunctionNamed(name)) {
      this.callFunc(name, argc, range);
      return;
    }

    const vt = this.getVarType(name);
    if (vt) {
      if (isCallableType(vt)) {
        if (!matchesCallableArity(vt, argc)) {
          this.issues.push({
            message: `Expression '${name}' is callable, but expects ${vt.kind === "func" ? vt.params.length : "compatible"} args`,
            range
          });
          return;
        }
        this.indirectCallSites.push({ argc, range, filePath: this.currentFilePath, calleeType: vt });
        return;
      }
      this.issues.push({
        message: `Expression '${name}' is not callable`,
        range
      });
      return;
    }

    this.callFunc(name, argc, range);
  }

  callAssociatedMethod(containerName: string, name: string, argc: number, range: Range) {
    const qualifiedName = this.qualifiedMethodName(containerName, name);
    const resolution = this.resolveAssociatedCall(containerName, name, argc);
    this.pendingAssociatedCalls.push({ containerName, name, argc, range, filePath: this.currentFilePath });
    this.upsertCallSite(qualifiedName, argc, range, resolution);
  }

  noteSizeof(range: Range, targetType: TypeNode) {
    this.sizeofSites.push({
      range,
      filePath: this.currentFilePath,
      targetType,
      size: sizeofType(targetType) ?? null
    });
  }

  callIndirectExpr(calleeType: TypeNode, argc: number, range: Range) {
    if (calleeType.kind === "unknown") return;

    if (isCallableType(calleeType)) {
      if (!matchesCallableArity(calleeType, argc)) {
        this.issues.push({
          message: `Expression is callable, but expects ${calleeType.kind === "func" ? calleeType.params.length : "compatible"} args`,
          range
        });
        return;
      }
      this.indirectCallSites.push({ argc, range, filePath: this.currentFilePath, calleeType });
      return;
    }

    this.issues.push({
      message: `Expression is not callable`,
      range
    });
  }

  finish() {
    for (const c of this.pendingAssociatedCalls) {
      const qualifiedName = this.qualifiedMethodName(c.containerName, c.name);
      const resolution = this.resolveAssociatedCall(c.containerName, c.name, c.argc);
      if (resolution == undefined) continue;
      this.upsertCallSite(qualifiedName, c.argc, c.range, resolution, c.filePath);

      if (resolution.status === "unknown") {
        const container = this.containers.get(c.containerName);
        this.issues.push({
          message: container
            ? `Unknown associated method '${qualifiedName}'`
            : `Unknown container '${c.containerName}'`,
          range: c.range
        });
        continue;
      }

      if (resolution.status === "no_match") {
        const onlyInstance = resolution.candidates.length > 0
          && resolution.candidates.every((fn) => this.isInstanceMethod(fn));

        if (onlyInstance) {
          this.issues.push({
            message: `Associated call '${qualifiedName}' refers to an instance method; use '.' on a value`,
            range: c.range
          });
          continue;
        }

        const expected = resolution.candidates
          .map(expectedArityString)
          .filter((v, i, a) => a.indexOf(v) === i)
          .join(" | ");

        this.issues.push({
          message: `Call '${qualifiedName}': no matching overload for ${c.argc} args (available: ${expected || "none"})`,
          range: c.range
        });
        continue;
      }
    }

    for (const c of this.pendingCalls) {
      const resolution = this.resolveCall(c.name, c.argc, c.scope);
      if (resolution == undefined) continue;
      this.upsertCallSite(c.name, c.argc, c.range, resolution, c.filePath);

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

    this.pendingAssociatedCalls = [];
    this.pendingCalls = [];
  }
}
