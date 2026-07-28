import { Range } from "vscode-languageserver/node";

export type MacroValue =
  | { kind: "string"; value: string }
  | { kind: "char"; value: string }
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
  annotations?: string[];
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
  annotations?: string[];
};

export type FuncOverloadSym = {
  kind: "func";
  name: string;
  containerName?: string;
  isSelfMethod?: boolean;
  isGlobal?: boolean;
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
  annotations?: string[];
};

export type ContainerSym = {
  kind: "container";
  name: string;
  fields: Map<string, VarSym>;
  methods: Map<string, FuncOverloadSym[]>;
  range: Range;
  filePath?: string;
  doc?: string;
  annotations?: string[];
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

export type DocCommentLink = {
  range: Range;
  filePath?: string;
  targetName: string;
  targetRange: Range;
  targetFilePath?: string;
  doc: string;
};

export function hasAnnotation(annotations: readonly string[] | undefined, name: string): boolean {
  return annotations?.some((annotation) => {
    const text = annotation.trim();
    const open = text.indexOf("(");
    return (open < 0 ? text : text.slice(0, open).trim()) === name;
  }) ?? false;
}

export function annotationArgument(
  annotations: readonly string[] | undefined,
  name: string
): string | undefined {
  for (const annotation of annotations ?? []) {
    const text = annotation.trim();
    const open = text.indexOf("(");
    if (open < 0 || !text.endsWith(")")) continue;
    if (text.slice(0, open).trim() !== name) continue;
    return text.slice(open + 1, -1).trim();
  }
  return undefined;
}

export function formatAnnotations(annotations: readonly string[] | undefined): string[] {
  return (annotations ?? []).map((annotation) => `@[${annotation}]`);
}

function mergeAnnotations(
  existing: readonly string[] | undefined,
  incoming: readonly string[] | undefined
): string[] | undefined {
  const merged = [...(existing ?? [])];
  for (const annotation of incoming ?? []) {
    if (!merged.includes(annotation)) merged.push(annotation);
  }
  return merged.length ? merged : undefined;
}

function mergeParamAnnotations(existing: ParamSig[], incoming: ParamSig[]) {
  for (let i = 0; i < Math.min(existing.length, incoming.length); i++) {
    existing[i].annotations = mergeAnnotations(existing[i].annotations, incoming[i].annotations);
  }
}

export type SizeofOptions = {
  pointerSize?: number;
  containers?: ReadonlyMap<string, ContainerSym>;
  resolveAnnotationInt?: (annotations: readonly string[] | undefined, name: string) => number | undefined;
};

type TypeLayout = { size: number; alignment: number };

function alignUp(value: number, alignment: number): number {
  if (!Number.isFinite(alignment) || alignment <= 1) return value;
  return Math.ceil(value / alignment) * alignment;
}

function parseAnnotationInt(
  annotations: readonly string[] | undefined,
  name: string
): number | undefined {
  const argument = annotationArgument(annotations, name);
  if (argument == null) return undefined;
  if (/^0x[0-9a-f]+$/i.test(argument)) return Number.parseInt(argument.slice(2), 16);
  if (/^0b[01]+$/i.test(argument)) return Number.parseInt(argument.slice(2), 2);
  if (/^\d+$/.test(argument)) return Number.parseInt(argument, 10);
  return undefined;
}

function layoutOfType(
  t: TypeNode,
  opts: SizeofOptions,
  visiting: Set<string>
): TypeLayout | undefined {
  const pointerSize = opts.pointerSize ?? 8;

  switch (t.kind) {
    case "prim": {
      let size: number | undefined;
      switch (t.name) {
        case "i0": size = 0; break;
        case "i8":
        case "u8": size = 1; break;
        case "i16":
        case "u16": size = 2; break;
        case "i32":
        case "u32":
        case "f32": size = 4; break;
        case "i64":
        case "u64":
        case "f64": size = 8; break;
        case "str": size = pointerSize; break;
        default:
          if (opts.containers?.has(t.name)) {
            return layoutOfType({ kind: "container", name: t.name }, opts, visiting);
          }
          return undefined;
      }
      return { size, alignment: Math.max(1, Math.min(size || 1, pointerSize)) };
    }
    case "ptr":
    case "func":
      return { size: pointerSize, alignment: pointerSize };
    case "arr": {
      if (t.len == null) return undefined;
      const elem = layoutOfType(t.elem, opts, visiting);
      return elem ? { size: t.len * elem.size, alignment: elem.alignment } : undefined;
    }
    case "container": {
      const container = opts.containers?.get(t.name);
      if (!container || visiting.has(t.name)) return undefined;

      visiting.add(t.name);
      try {
        const resolveInt = opts.resolveAnnotationInt ?? parseAnnotationInt;
        const explicitContainerAlignment = resolveInt(container.annotations, "align");
        const defaultAlignment = Math.max(1, explicitContainerAlignment ?? pointerSize);
        const likeC = hasAnnotation(container.annotations, "like_c");
        const isUnion = hasAnnotation(container.annotations, "union");
        const fields = [...container.fields.values()];

        const fieldLayouts: { field: VarSym; layout: TypeLayout; alignment: number }[] = [];
        for (const field of fields) {
          const layout = layoutOfType(field.type, opts, visiting);
          if (!layout) return undefined;
          const explicitAlignment = resolveInt(field.annotations, "align");
          const alignment = Math.max(1, explicitAlignment ?? (likeC ? layout.alignment : defaultAlignment));
          fieldLayouts.push({ field, layout, alignment });
        }

        if (isUnion) {
          const largest = fieldLayouts.reduce((max, item) => Math.max(max, item.layout.size), 0);
          const baseAlignment = likeC ? (explicitContainerAlignment ?? 1) : defaultAlignment;
          const alignment = fieldLayouts.reduce(
            (max, item) => Math.max(max, item.alignment),
            Math.max(1, baseAlignment)
          );
          return { size: alignUp(largest, alignment), alignment };
        }

        let offset = 0;
        let containerAlignment = likeC
          ? Math.max(1, explicitContainerAlignment ?? 1)
          : defaultAlignment;
        for (const item of fieldLayouts) {
          offset = alignUp(offset, item.alignment);
          offset += item.layout.size;
          containerAlignment = Math.max(containerAlignment, item.alignment);
        }

        return {
          size: alignUp(offset, containerAlignment),
          alignment: containerAlignment
        };
      } finally {
        visiting.delete(t.name);
      }
    }
    case "unknown":
      return undefined;
  }
}

export function sizeofType(t: TypeNode, opts: SizeofOptions = {}): number | undefined {
  return layoutOfType(t, opts, new Set<string>())?.size;
}

export function formatFunctionSignature(fn: FuncOverloadSym): string {
  const displayName = fn.containerName ? `${fn.containerName}::${fn.name}` : fn.name;
  const paramsStr = fn.params
    .map((p) => {
      const annotations = formatAnnotations(p.annotations).join(" ");
      if (p.isVarArgs) return `${annotations ? annotations + " " : ""}...`;
      return `${annotations ? annotations + " " : ""}${formatType(p.type)} ${p.name}${p.hasDefault ? " = <default>" : ""}`;
    })
    .join(", ");

  const retStr = formatType(fn.ret);
  const typeParamsStr = fn.typeParams?.length ? `<${fn.typeParams.join(", ")}>` : "";
  const prefix = fn.isGlobal ? "glob " : "";
  return retStr === "i0"
    ? `${prefix}function ${displayName}${typeParamsStr}(${paramsStr})`
    : `${prefix}function ${displayName}${typeParamsStr}(${paramsStr}) -> ${retStr}`;
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

function hasTypeParams(typeParams: readonly string[] | undefined): boolean {
  return !!typeParams?.length;
}

function isTypeNamedContainer(t: TypeNode, containerName: string): boolean {
  return (t.kind === "container" || t.kind === "prim") && t.name === containerName;
}

function isSelfRefParam(param: ParamSig | undefined, containerName: string): boolean {
  return !!param && param.type.kind === "ptr" && isTypeNamedContainer(param.type.to, containerName);
}

function hasExplicitAnnotationArgument(
  annotations: readonly string[] | undefined,
  name: string
): boolean {
  const arg = annotationArgument(annotations, name);
  return arg != null && arg.trim().length > 0;
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

function instanceCallParams(fn: FuncOverloadSym): ParamSig[] {
  return fn.params[0]?.name === "self" ? fn.params.slice(1) : fn.params;
}

function matchesInstanceArity(fn: FuncOverloadSym, argc: number): boolean {
  const { minArgs, maxArgs } = arityBounds(instanceCallParams(fn));
  return argc >= minArgs && argc <= maxArgs;
}

function expectedInstanceArityString(fn: FuncOverloadSym): string {
  const { minArgs, maxArgs } = arityBounds(instanceCallParams(fn));
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

export type SemanticContextOptions = {
  pointerSize?: number;
};

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
  varUses: { name: string; type: TypeNode; range: Range; filePath?: string; targetRange?: Range; targetFilePath?: string; readonly?: boolean; annotations?: string[] }[] = [];
  funcValueUses: FuncValueUse[] = [];
  callSites: CallSite[] = [];
  indirectCallSites: IndirectCallSite[] = [];
  sizeofSites: SizeofSite[] = [];
  docLinks: DocCommentLink[] = [];

  private currentFilePath: string | undefined;
  private scope: Scope = new Scope();
  private pendingCalls: { name: string; argc: number; range: Range; filePath?: string; scope: Scope }[] = [];
  private pendingAssociatedCalls: { containerName: string; name: string; argc: number; range: Range; filePath?: string }[] = [];
  private pendingInstanceCalls: { containerName: string; name: string; argc: number; range: Range; filePath?: string }[] = [];

  constructor(private readonly options: SemanticContextOptions = {}) {}

  getPointerSize(): number {
    return this.options.pointerSize ?? 8;
  }

  private resolveAnnotationInt = (
    annotations: readonly string[] | undefined,
    name: string
  ): number | undefined => {
    const argument = annotationArgument(annotations, name);
    if (argument == null) return undefined;
    if (/^0x[0-9a-f]+$/i.test(argument)) return Number.parseInt(argument.slice(2), 16);
    if (/^0b[01]+$/i.test(argument)) return Number.parseInt(argument.slice(2), 2);
    if (/^\d+$/.test(argument)) return Number.parseInt(argument, 10);

    const macro = this.macros.get(argument);
    return macro?.value.kind === "number" ? macro.value.value : undefined;
  };

  sizeofType(type: TypeNode): number | undefined {
    return sizeofType(type, {
      pointerSize: this.getPointerSize(),
      containers: this.containers,
      resolveAnnotationInt: this.resolveAnnotationInt
    });
  }

  setCurrentFilePath(filePath?: string): string | undefined {
    const prev = this.currentFilePath;
    this.currentFilePath = filePath;
    return prev;
  }

  getCurrentFilePath(): string | undefined {
    return this.currentFilePath;
  }

  linkDocComment(range: Range, doc: string, targetName: string, targetRange: Range, targetFilePath?: string) {
    this.docLinks.push({
      range,
      filePath: this.currentFilePath,
      targetName,
      targetRange,
      targetFilePath: targetFilePath ?? this.currentFilePath,
      doc
    });
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

  declareContainer(name: string, range: Range, doc?: string, opts?: { annotations?: string[] }) {
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
      doc,
      annotations: opts?.annotations?.length ? [...opts.annotations] : undefined
    };

    this.containers.set(name, sym);
    this.containerDecls.push(sym);
  }

  declareContainerField(
    containerName: string,
    name: string,
    type: TypeNode,
    range: Range,
    opts?: { readonly?: boolean; annotations?: string[] }
  ) {
    const container = this.containers.get(containerName);
    if (!container) {
      this.issues.push({ message: `Unknown container '${containerName}'`, range });
      return;
    }

    if (container.fields.has(name)) {
      this.issues.push({ message: `Field '${containerName}.${name}' already declared`, range });
      return;
    }

    const sym: VarSym = {
      kind: "var",
      name,
      type,
      range,
      filePath: this.currentFilePath,
      readonly: opts?.readonly,
      annotations: opts?.annotations?.length ? [...opts.annotations] : undefined
    };
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
    opts?: { self?: boolean; global?: boolean; annotations?: string[] }
  ) {
    const container = this.containers.get(containerName);
    if (!container) {
      this.issues.push({ message: `Unknown container '${containerName}'`, range });
      return;
    }

    const local = container.methods.get(name) ?? [];
    const exact = local.find((f) => sameParamIdentity(f.params, params));

    if (opts?.self && !isSelfRefParam(params[0], containerName)) {
      this.issues.push({
        message: `@[self] method '${containerName}.${name}' must start with a reference to '${containerName}' (expected 'ptr ${containerName} self')`,
        range: params[0]?.range ?? range
      });
    }

    if (
      opts?.global &&
      !hasExplicitAnnotationArgument(opts?.annotations, "vname") &&
      !hasExplicitAnnotationArgument(exact?.annotations, "vname")
    ) {
      this.issues.push({
        message: `Global container method '${containerName}.${name}' must declare an exported name with @[vname("...")]`,
        range
      });
    }

    if (!exact && hasTypeParams(typeParams) && local.some((f) => hasTypeParams(f.typeParams))) {
      this.issues.push({
        message: `Generic method '${containerName}.${name}' cannot have another generic overload`,
        range
      });
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

      const sym: FuncOverloadSym = {
        kind: "func",
        name,
        containerName,
        isSelfMethod: opts?.self || params[0]?.name === "self",
        isGlobal: opts?.global,
        typeParams,
        params,
        ret,
        decls: isDefinition ? [] : [range],
        declFiles: isDefinition ? [] : [this.currentFilePath],
        def: isDefinition ? range : undefined,
        defFilePath: isDefinition ? this.currentFilePath : undefined,
        primaryRange: range,
        primaryFilePath: this.currentFilePath,
        doc,
        annotations: opts?.annotations?.length ? [...opts.annotations] : undefined
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
    if (opts?.global) exact.isGlobal = true;
    exact.annotations = mergeAnnotations(exact.annotations, opts?.annotations);
    mergeParamAnnotations(exact.params, params);

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

  getContainerMember(
    baseType: TypeNode,
    memberName: string,
    range: Range
  ): { type: TypeNode; containerName?: string; methodName?: string } {
    const containerName = this.containerNameFromType(baseType);
    if (!containerName) {
      if (baseType.kind !== "unknown") {
        this.issues.push({ message: `Type '${formatType(baseType)}' has no members`, range });
      }
      return { type: { kind: "unknown" } };
    }

    const container = this.containers.get(containerName);
    if (!container) return { type: { kind: "unknown" } };

    const field = container.fields.get(memberName);
    if (field) {
      this.varUses.push({
        name: `${containerName}.${memberName}`,
        type: field.type,
        range,
        filePath: this.currentFilePath,
        targetRange: field.range,
        targetFilePath: field.filePath,
        readonly: field.readonly,
        annotations: field.annotations?.length ? [...field.annotations] : undefined
      });
      return { type: field.type };
    }

    const methods = container.methods.get(memberName) ?? [];
    if (methods.length === 1) {
      const fn = methods[0];
      return {
        type: this.functionTypeFromMethod(fn, this.isInstanceMethod(fn)),
        containerName,
        methodName: memberName
      };
    }

    if (methods.length > 1) {
      return {
        type: { kind: "ptr", to: { kind: "prim", name: "i0" } },
        containerName,
        methodName: memberName
      };
    }

    this.issues.push({ message: `Unknown member '${containerName}.${memberName}'`, range });
    return { type: { kind: "unknown" } };
  }

  getContainerMemberType(baseType: TypeNode, memberName: string, range: Range): TypeNode {
    return this.getContainerMember(baseType, memberName, range).type;
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

    if (this.macros.has(name)) return;

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

  undefMacro(name: string) {
    this.macros.delete(name);
  }

  isMacroDefined(name: string): boolean {
    return this.macros.has(name);
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
        targetFilePath: v.filePath,
        readonly: v.readonly,
        annotations: v.annotations?.length ? [...v.annotations] : undefined
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
    opts?: { readonly?: boolean; annotations?: string[] }
  ) {
    const existing = this.globals.get(name);
    if (existing) {
      if (!existing.filePath && this.currentFilePath) existing.filePath = this.currentFilePath;
      existing.annotations = mergeAnnotations(existing.annotations, opts?.annotations);
      if (opts?.readonly) existing.readonly = true;
      return;
    }

    const sym: VarSym = {
      kind: "var",
      name,
      type,
      range,
      filePath: this.currentFilePath,
      readonly: opts?.readonly,
      annotations: opts?.annotations?.length ? [...opts.annotations] : undefined
    };
    this.globals.set(name, sym);
    this.varDecls.push(sym);
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
    const sym: VarSym = {
      kind: "var",
      name,
      type,
      range,
      filePath: this.currentFilePath,
      readonly: opts?.readonly,
      annotations: opts?.annotations?.length ? [...opts.annotations] : undefined
    };
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
    const sym: VarSym = {
      kind: "var",
      name,
      type,
      range,
      filePath: this.currentFilePath,
      readonly: opts?.readonly,
      annotations: opts?.annotations?.length ? [...opts.annotations] : undefined
    };
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
    opts?: { global?: boolean; annotations?: string[] }
  ) {
    const local = this.scope.funcs.get(name) ?? [];

    const exact = local.find((f) => sameParamIdentity(f.params, params));
    if (!exact && hasTypeParams(typeParams) && local.some((f) => hasTypeParams(f.typeParams))) {
      this.issues.push({
        message: `Generic function '${name}' cannot have another generic overload`,
        range
      });
    }

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
        isGlobal: opts?.global,
        typeParams,
        params,
        ret,
        decls: isDefinition ? [] : [range],
        declFiles: isDefinition ? [] : [this.currentFilePath],
        def: isDefinition ? range : undefined,
        defFilePath: isDefinition ? this.currentFilePath : undefined,
        primaryRange: range,
        primaryFilePath: this.currentFilePath,
        doc,
        annotations: opts?.annotations?.length ? [...opts.annotations] : undefined
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
    if (opts?.global) exact.isGlobal = true;
    exact.annotations = mergeAnnotations(exact.annotations, opts?.annotations);
    mergeParamAnnotations(exact.params, params);

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

  private resolveInstanceCall(containerName: string, name: string, argc: number): CallSite["resolution"] {
    const container = this.containers.get(containerName);
    if (!container) {
      return { status: "unknown", candidates: [] };
    }

    const allMethods = container.methods.get(name) ?? [];
    const overloads = allMethods.filter((fn) => this.isInstanceMethod(fn));
    if (overloads.length === 0) {
      return allMethods.length > 0
        ? { status: "no_match", candidates: allMethods }
        : { status: "unknown", candidates: [] };
    }

    const arityMatches = overloads.filter((fn) => matchesInstanceArity(fn, argc));
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

  callInstanceMethod(containerName: string, name: string, argc: number, range: Range) {
    const qualifiedName = this.qualifiedMethodName(containerName, name);
    const resolution = this.resolveInstanceCall(containerName, name, argc);
    this.pendingInstanceCalls.push({ containerName, name, argc, range, filePath: this.currentFilePath });
    this.upsertCallSite(qualifiedName, argc, range, resolution);
  }

  noteSizeof(range: Range, targetType: TypeNode) {
    this.sizeofSites.push({
      range,
      filePath: this.currentFilePath,
      targetType,
      size: this.sizeofType(targetType) ?? null
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

    for (const c of this.pendingInstanceCalls) {
      const qualifiedName = this.qualifiedMethodName(c.containerName, c.name);
      const resolution = this.resolveInstanceCall(c.containerName, c.name, c.argc);
      if (resolution == undefined) continue;
      this.upsertCallSite(qualifiedName, c.argc, c.range, resolution, c.filePath);

      if (resolution.status === "unknown") {
        const container = this.containers.get(c.containerName);
        this.issues.push({
          message: container
            ? `Unknown instance method '${c.containerName}.${c.name}'`
            : `Unknown container '${c.containerName}'`,
          range: c.range
        });
        continue;
      }

      if (resolution.status === "no_match") {
        const onlyAssociated = resolution.candidates.length > 0
          && resolution.candidates.every((fn) => this.isAssociatedMethod(fn));

        if (onlyAssociated) {
          this.issues.push({
            message: `Instance call '${c.containerName}.${c.name}' refers to an associated method; use '::' on the container type`,
            range: c.range
          });
          continue;
        }

        const expected = resolution.candidates
          .map(expectedInstanceArityString)
          .filter((v, i, a) => a.indexOf(v) === i)
          .join(" | ");

        this.issues.push({
          message: `Call '${c.containerName}.${c.name}': no matching overload for ${c.argc} args (available: ${expected || "none"})`,
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
    this.pendingInstanceCalls = [];
    this.pendingCalls = [];
  }
}
