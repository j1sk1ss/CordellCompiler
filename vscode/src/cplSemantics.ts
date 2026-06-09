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

export type IssueSeverity = "error" | "warning" | "information" | "hint";
export type Issue = { message: string; range: Range; severity?: IssueSeverity };

export type VarSym = {
  kind: "var";
  name: string;
  type: TypeNode;
  range: Range;
  readonly?: boolean;
  annotations?: string[];
};

export type FuncOverloadSym = {
  kind: "func";
  name: string;
  typeParams?: string[];
  params: ParamSig[];
  ret: TypeNode;
  decls: Range[];
  def?: Range;
  primaryRange: Range;
  doc?: string;
  annotations?: string[];
  weak?: boolean;
  selfMethod?: boolean;
  implContainer?: string;
};

export type ContainerSym = {
  kind: "container";
  name: string;
  fields: Map<string, VarSym>;
  methods: Map<string, FuncOverloadSym[]>;
  range: Range;
  doc?: string;
  annotations?: string[];
  isUnion?: boolean;
  likeC?: boolean;
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

export type SizeofSite = {
  range: Range;
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
  const paramsStr = fn.params
    .map((p) => {
      if (p.isVarArgs) return "...";
      return `${formatType(p.type)} ${p.name}${p.hasDefault ? " = <default>" : ""}`;
    })
    .join(", ");

  const retStr = formatType(fn.ret);
  const typeParamsStr = fn.typeParams?.length ? `<${fn.typeParams.join(", ")}>` : "";
  return retStr === "i0"
    ? `function ${fn.name}${typeParamsStr}(${paramsStr})`
    : `function ${fn.name}${typeParamsStr}(${paramsStr}) -> ${retStr}`;
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


function parseAnnotationRaw(raw: string): { name: string; arg?: string } {
  const t = raw.trim();
  const m = /^([A-Za-z_]\w*)\s*(?:\((.*)\))?$/.exec(t);
  if (!m) return { name: t };
  return { name: m[1], arg: m[2]?.trim() };
}

function hasAnnotation(annotations: string[] | undefined, name: string): boolean {
  return (annotations ?? []).some((a) => parseAnnotationRaw(a).name === name);
}

export function annotationArg(annotations: string[] | undefined, name: string): string | undefined {
  const hit = (annotations ?? []).map(parseAnnotationRaw).find((a) => a.name === name);
  return hit?.arg;
}

export function formatAnnotations(annotations: string[] | undefined): string {
  return (annotations ?? []).map((a) => `@[${a}]`).join(" ");
}

function mergeAnnotations(...lists: (string[] | undefined)[]): string[] | undefined {
  const out: string[] = [];
  for (const list of lists) {
    for (const a of list ?? []) {
      if (!out.includes(a)) out.push(a);
    }
  }
  return out.length ? out : undefined;
}

function stripAnnotationStringValue(value: string | undefined): string | undefined {
  if (value == null) return undefined;
  const t = value.trim();
  if (t.length >= 2 && t.startsWith('"') && t.endsWith('"')) return t.slice(1, -1);
  return t;
}

function typeMentionsContainer(t: TypeNode, containerName: string): boolean {
  if (t.kind === "container") return t.name === containerName;
  if (t.kind === "prim") return t.name === containerName;
  if (t.kind === "ptr") return typeMentionsContainer(t.to, containerName);
  return false;
}

export class SemanticContext {
  issues: Issue[] = [];

  funcs = new Map<string, FuncOverloadSym[]>();
  globals = new Map<string, VarSym>();
  containers = new Map<string, ContainerSym>();
  containerDecls: ContainerSym[] = [];

  macros = new Map<string, MacroSym>();
  macroDecls: MacroSym[] = [];
  macroUses: { name: string; range: Range }[] = [];

  varDecls: VarSym[] = [];
  varUses: { name: string; type: TypeNode; range: Range }[] = [];
  funcValueUses: FuncValueUse[] = [];
  callSites: CallSite[] = [];
  indirectCallSites: IndirectCallSite[] = [];
  sizeofSites: SizeofSite[] = [];

  private scope: Scope = new Scope();
  private pendingCalls: { name: string; argc: number; range: Range; scope: Scope }[] = [];


  addWarning(message: string, range: Range) {
    this.issues.push({ message, range, severity: "warning" });
  }

  hasGlobal(name: string): boolean {
    return this.globals.has(name);
  }

  hasMacro(name: string): boolean {
    return this.macros.has(name);
  }

  hasContainer(name: string): boolean {
    return this.containers.has(name);
  }

  containerTypeForName(name: string): TypeNode {
    return this.containers.has(name) ? { kind: "container", name } : { kind: "prim", name };
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

  declareContainer(name: string, range: Range, doc?: string, annotations?: string[]) {
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
      doc,
      annotations,
      isUnion: hasAnnotation(annotations, "union"),
      likeC: hasAnnotation(annotations, "like_c")
    };

    this.containers.set(name, sym);
    this.containerDecls.push(sym);
  }

  declareContainerField(containerName: string, name: string, type: TypeNode, range: Range, opts?: { readonly?: boolean; annotations?: string[] }) {
    const container = this.containers.get(containerName);
    if (!container) {
      this.issues.push({ message: `Unknown container '${containerName}'`, range });
      return;
    }

    if (container.fields.has(name)) {
      this.issues.push({ message: `Field '${containerName}.${name}' already declared`, range });
      return;
    }

    const sym: VarSym = { kind: "var", name, type, range, readonly: opts?.readonly, annotations: opts?.annotations };
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
    annotations?: string[],
    opts?: { fromImpl?: boolean }
  ) {
    const container = this.containers.get(containerName);
    if (!container) {
      this.issues.push({
        message: opts?.fromImpl
          ? `@[impl(${containerName})]: container '${containerName}' not found. Include or declare the container before this function.`
          : `Unknown container '${containerName}'`,
        range
      });
      return;
    }

    const local = container.methods.get(name) ?? [];
    const exact = local.find((f) => sameParamIdentity(f.params, params));

    // For @[impl(Container)] the header/container prototype is the source of truth.
    // The implementation inherits annotations from the matching container method,
    // so it does not need to repeat @[self], @[inline(...)], @[weak], etc.
    const inheritedAnnotations = opts?.fromImpl && exact
      ? mergeAnnotations(exact.annotations, annotations)
      : annotations;

    const hasSelf = hasAnnotation(inheritedAnnotations, "self") || (opts?.fromImpl && exact?.selfMethod === true);
    const first = params[0];
    if (hasSelf) {
      if (!first || first.name !== "self") {
        this.addWarning(`@[self] method '${containerName}.${name}' should have first parameter named 'self'`, range);
      } else if (!typeMentionsContainer(first.type, containerName)) {
        this.addWarning(`@[self] receiver '${first.name}' should have type '${containerName}' or 'ptr ${containerName}'`, first.range);
      }
    } else if (first?.name === "self" && !opts?.fromImpl) {
      this.addWarning(`Method '${containerName}.${name}' has receiver 'self'. Add @[self] if object calls must pass the receiver automatically.`, first.range);
    }

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

      const implContainer = opts?.fromImpl ? containerName : stripAnnotationStringValue(annotationArg(inheritedAnnotations, "impl"));
      const sym: FuncOverloadSym = {
        kind: "func",
        name,
        typeParams,
        params,
        ret,
        decls: isDefinition ? [] : [range],
        def: isDefinition ? range : undefined,
        primaryRange: range,
        doc,
        annotations: inheritedAnnotations,
        weak: hasAnnotation(inheritedAnnotations, "weak"),
        selfMethod: hasAnnotation(inheritedAnnotations, "self") || params[0]?.name === "self",
        implContainer
      };

      local.push(sym);
      container.methods.set(name, local);
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

    if (inheritedAnnotations?.length) {
      exact.annotations = mergeAnnotations(exact.annotations, inheritedAnnotations);
      exact.weak = exact.weak || hasAnnotation(inheritedAnnotations, "weak");
      exact.selfMethod = exact.selfMethod || hasAnnotation(inheritedAnnotations, "self") || exact.params[0]?.name === "self";
      exact.implContainer = exact.implContainer ?? (opts?.fromImpl ? containerName : stripAnnotationStringValue(annotationArg(inheritedAnnotations, "impl")));
    } else if (opts?.fromImpl) {
      exact.implContainer = exact.implContainer ?? containerName;
    }

    if (isDefinition) {
      if (exact.def) {
        this.issues.push({ message: `Method '${containerName}.${name}' overload already defined`, range });
        return;
      }
      exact.def = range;
      exact.primaryRange = range;
      return;
    }


    exact.decls.push(range);
    if (!exact.def && exact.decls.length === 1) exact.primaryRange = exact.decls[0];
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
      this.varUses.push({ name: `${containerName}.${memberName}`, type: field.type, range });
      return field.type;
    }

    const methods = container.methods.get(memberName) ?? [];
    if (methods.length === 1) {
      const fn = methods[0];
      const params = (fn.selfMethod || fn.params[0]?.name === "self") ? fn.params.slice(1) : fn.params;
      return { kind: "func", params: params.filter((p) => !p.isVarArgs).map((p) => p.type), ret: fn.ret };
    }

    if (methods.length > 1) {
      return { kind: "ptr", to: { kind: "prim", name: "i0" } };
    }

    this.issues.push({ message: `Unknown member '${containerName}.${memberName}'`, range });
    return { kind: "unknown" };
  }

  getContainerMembersForType(baseType: TypeNode): { containerName: string; fields: VarSym[]; methods: FuncOverloadSym[] } | undefined {
    const containerName = this.containerNameFromType(baseType);
    if (!containerName) return undefined;
    const container = this.containers.get(containerName);
    if (!container) return undefined;
    return {
      containerName,
      fields: [...container.fields.values()],
      methods: [...container.methods.values()].flat()
    };
  }

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
      this.varUses.push({ name, type: v.type, range });
      return;
    }

    const m = this.macros.get(name);
    if (m) {
      this.useMacro(name, range);
      return;
    }

    const overloads = this.scope.resolveFuncs(name);
    if (overloads.length > 0) {
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
    opts?: { readonly?: boolean; annotations?: string[] }
  ) {
    if (this.globals.has(name)) {
      this.issues.push({ message: `Global '${name}' already declared`, range });
    }
    const sym: VarSym = { kind: "var", name, type, range, readonly: opts?.readonly, annotations: opts?.annotations };
    this.globals.set(name, sym);
    this.varDecls.push(sym);
  }

  declareLocalVar(
    name: string,
    type: TypeNode,
    range: Range,
    opts?: { readonly?: boolean; annotations?: string[] }
  ) {
    if (this.scope.vars.has(name)) {
      this.issues.push({ message: `Variable '${name}' already declared in this scope`, range });
    }
    const sym: VarSym = { kind: "var", name, type, range, readonly: opts?.readonly, annotations: opts?.annotations };
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
    typeParams?: string[],
    annotations?: string[]
  ) {
    const local = this.scope.funcs.get(name) ?? [];

    const implContainer = stripAnnotationStringValue(annotationArg(annotations, "impl"));
    if (hasAnnotation(annotations, "self") && !implContainer) {
      this.addWarning(`@[self] on top-level function '${name}' has no container target. Use @[impl(Container)] or move the function into a container.`, range);
    }

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
        def: isDefinition ? range : undefined,
        primaryRange: range,
        doc,
        annotations,
        weak: hasAnnotation(annotations, "weak"),
        selfMethod: hasAnnotation(annotations, "self"),
        implContainer
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
    if (annotations?.length) {
      exact.annotations = [...(exact.annotations ?? []), ...annotations].filter((v, i, a) => a.indexOf(v) === i);
      exact.weak = exact.weak || hasAnnotation(annotations, "weak");
      exact.selfMethod = exact.selfMethod || hasAnnotation(annotations, "self") || exact.params[0]?.name === "self";
      exact.implContainer = exact.implContainer ?? stripAnnotationStringValue(annotationArg(annotations, "impl"));
    }

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

  callFunc(name: string, argc: number, range: Range) {
    if (name === "syscall") return;
    this.pendingCalls.push({ name, argc, range, scope: this.scope });
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
        this.indirectCallSites.push({ argc, range, calleeType: vt });
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

  noteSizeof(range: Range, targetType: TypeNode) {
    this.sizeofSites.push({
      range,
      targetType,
      size: sizeofType(targetType) ?? null
    });
  }

  callIndirectExpr(calleeType: TypeNode, argc: number, range: Range) {
    if (isCallableType(calleeType)) {
      if (!matchesCallableArity(calleeType, argc)) {
        this.issues.push({
          message: `Expression is callable, but expects ${calleeType.kind === "func" ? calleeType.params.length : "compatible"} args`,
          range
        });
        return;
      }
      this.indirectCallSites.push({ argc, range, calleeType });
      return;
    }

    this.issues.push({
      message: `Expression is not callable`,
      range
    });
  }

  finish() {
    for (const c of this.pendingCalls) {
      const resolution = this.resolveCall(c.name, c.argc, c.scope);
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
