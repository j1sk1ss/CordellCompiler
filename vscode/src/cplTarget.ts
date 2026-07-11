import * as os from "os";

export type CplSysType = "unknown" | "macho64" | "linux64" | "i386" | "windows64";

export type CplPredefinedMacro = {
  name: string;
  value: string;
  doc: string;
};

export function sysTypeToPredefinedMacro(sysType: CplSysType): CplPredefinedMacro | undefined {
  switch (sysType) {
    case "macho64":
      return {
        name: "CCPL_MACHO64",
        value: "1",
        doc: "Predefined by PP_perform for the Mach-O x86-64 target."
      };
    case "linux64":
      return {
        name: "CCPL_GNU64",
        value: "1",
        doc: "Predefined by PP_perform for the Linux/GNU x86-64 target."
      };
    case "i386":
      return {
        name: "CCPL_GNUI386",
        value: "1",
        doc: "Predefined by PP_perform for the Linux/GNU i386 target."
      };
    case "windows64":
      return {
        name: "CCPL_WINDOWS64",
        value: "1",
        doc: "Predefined by PP_perform for the Windows x86-64 target."
      };
    default:
      return undefined;
  }
}

export function defaultSysTypeForHost(platform = os.platform()): CplSysType {
  return platform === "linux" ? "linux64" : "macho64";
}

function hostUnameS(platform = os.platform()): string {
  switch (platform) {
    case "darwin":
      return "Darwin";
    case "linux":
      return "Linux";
    case "win32":
      return "Windows_NT";
    default:
      return platform;
  }
}

function hostUnameM(arch = os.arch()): string {
  switch (arch) {
    case "x64":
      return "x86_64";
    case "ia32":
      return "i386";
    case "arm64":
      return "arm64";
    default:
      return arch;
  }
}

export function expandMakeValue(value: string, vars: Map<string, string>, depth = 0): string {
  if (depth > 20) return value;

  const withShell = value
    .replace(/\$\(\s*shell\s+uname\s+-s\s*\)/g, hostUnameS())
    .replace(/\$\(\s*shell\s+uname\s+-m\s*\)/g, hostUnameM())
    .replace(/\$\(\s*shell\s+uname\s+-s\s*\|\s*tr\s+'\[:upper:\]'\s+'\[:lower:\]'\s*\)/g, hostUnameS().toLowerCase())
    .replace(/\$\(\s*shell\s+uname\s+-m\s*\|\s*tr\s+'\[:upper:\]'\s+'\[:lower:\]'\s*\)/g, hostUnameM().toLowerCase());

  return withShell.replace(/\$\(([^)]+)\)/g, (_full, name: string) => {
    const replacement = vars.get(name.trim());
    return replacement == null ? "" : expandMakeValue(replacement, vars, depth + 1);
  });
}

function stripInlineMakeComment(value: string): string {
  return value.replace(/\s+#.*$/, "").trim();
}

function conditionArgs(line: string): { op: "ifeq" | "ifneq"; left: string; right: string } | undefined {
  const paren = line.match(/^(ifeq|ifneq)\s*\((.*),(.*)\)\s*$/);
  if (paren) {
    return { op: paren[1] as "ifeq" | "ifneq", left: paren[2].trim(), right: paren[3].trim() };
  }

  const quoted = line.match(/^(ifeq|ifneq)\s+["']?([^"'\s]+)["']?\s+["']?([^"'\s]+)["']?\s*$/);
  if (quoted) {
    return { op: quoted[1] as "ifeq" | "ifneq", left: quoted[2].trim(), right: quoted[3].trim() };
  }

  return undefined;
}

export function parseMakefileVarsText(text: string): Map<string, string> {
  const vars = new Map<string, string>([
    ["UNAME_S", hostUnameS()],
    ["UNAME_M", hostUnameM()]
  ]);

  type MakeConditionFrame = { parentActive: boolean; condition: boolean; active: boolean };
  const frames: MakeConditionFrame[] = [];
  let active = true;

  for (const rawLine of text.split(/\r?\n/)) {
    const line = rawLine.trim();
    if (!line || line.startsWith("#")) continue;

    const cond = conditionArgs(line);
    if (cond) {
      const left = expandMakeValue(cond.left, vars);
      const right = expandMakeValue(cond.right, vars);
      const equal = left === right;
      const condition = cond.op === "ifeq" ? equal : !equal;
      const parentActive = active;
      const frame: MakeConditionFrame = { parentActive, condition, active: parentActive && condition };
      frames.push(frame);
      active = frame.active;
      continue;
    }

    if (line === "else") {
      const frame = frames[frames.length - 1];
      if (frame) {
        frame.active = frame.parentActive && !frame.condition;
        active = frame.active;
      }
      continue;
    }

    if (line === "endif") {
      frames.pop();
      active = frames.length ? frames[frames.length - 1].active : true;
      continue;
    }

    if (!active) continue;

    const m = rawLine.match(/^\s*([A-Za-z_][A-Za-z0-9_]*)\s*(\?=|:=|=)\s*(.*)$/);
    if (!m) continue;

    const name = m[1];
    const op = m[2];
    const rawValue = stripInlineMakeComment(m[3]);
    if (op === "?=" && vars.has(name)) continue;

    vars.set(name, op === ":=" ? expandMakeValue(rawValue, vars) : rawValue);
  }

  return vars;
}

export function inferSysTypeFromCompilerArgs(args: string): CplSysType | undefined {
  const sysTypeMatches = [...args.matchAll(/(?:^|\s)--sys-type(?:=|\s+)(unknown|macho64|linux64|i386|windows64)(?=\s|$)/g)];
  if (sysTypeMatches.length) return sysTypeMatches[sysTypeMatches.length - 1][1] as CplSysType;

  const archMatches = [...args.matchAll(/(?:^|\s)--arch(?:=|\s+)(x86_64|amd64|x86|i386|ia32)(?=\s|$)/g)];
  if (!archMatches.length) return undefined;

  const arch = archMatches[archMatches.length - 1][1];
  if (arch === "x86" || arch === "i386" || arch === "ia32") return "i386";
  return undefined;
}
