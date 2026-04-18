import re

SRC = "macos_extracted_syscalls.text"
OUT = "source_macos_syscalls.inc"

line_re = re.compile(
    r'^\s*(\d+)\s+\S+\s+\S+\s+\{\s*(.*?)\s*\}\s*(?:\{.*)?$'
)
proto_re = re.compile(
    r'^(.*?)\s+([A-Za-z_]\w*)\s*\((.*?)\)\s*(?:NO_SYSCALL_STUB)?\s*;\s*$'
)
STR_NAMES = {
    "path", "path1", "path2",
    "fname", "attrname", "namebuf",
    "from", "to", "src", "dst",
    "reason_string", "type", "policy",
    "name1", "name2",
}
STR_LIST_NAMES = {
    "argp", "envp", "argv",
}
I32_TYPES = {
    "int", "int32_t", "pid_t", "id_t",
    "uid_t", "gid_t", "mode_t",
    "key_t", "socklen_t", "au_id_t",
    "au_asid_t", "idtype_t",
}
U32_TYPES = {
    "u_int", "u_int32_t", "uint32_t",
    "mach_port_name_t", "u_int32", "u_int16_t",
}
I64_TYPES = {
    "long", "int64_t", "off_t",
    "ssize_t", "user_ssize_t",
}
U64_TYPES = {
    "u_long", "unsigned long", "uint64_t",
    "size_t", "user_size_t",
}
U16_TYPES = {
    "uint16_t", "u_int16_t",
}
I16_TYPES = {
    "int16_t",
}
U8_TYPES = {
    "uint8_t", "unsigned char",
}
I8_TYPES = {
    "int8_t", "char", "signed char",
}
SPECIAL_ARRAY_TYPES = {
    "user_addr_t",
    "caddr_t",
    "void",
    "uuid_t",
}
SPECIAL_U32_TYPES = {
    "sigset_t",
}
SPECIAL_U64_TYPES = {
    "semun_t",
    "sae_associd_t",
    "sae_connid_t",
    "guardid_t",
}

def norm_spaces(s) -> str:
    s = s.strip()
    s = re.sub(r'(\*+)', r' \1 ', s)
    s = re.sub(r'\s+', ' ', s)
    return s.strip()

def split_args(args) -> list:
    args = args.strip()
    if args == "void" or args == "":
        return []

    parts = []
    cur = []
    depth = 0

    for ch in args:
        if ch == ',' and depth == 0:
            parts.append(''.join(cur).strip())
            cur = []
            continue
        if ch in '([{':
            depth += 1
        elif ch in ')]}':
            depth -= 1
        cur.append(ch)

    if cur:
        parts.append(''.join(cur).strip())

    return parts

def split_decl(arg) -> tuple:
    arg = norm_spaces(arg)
    if arg == "void":
        return None, None

    toks = arg.split()
    if len(toks) == 1:
        return toks[0], ""

    name = toks[-1]
    ctype = ' '.join(toks[:-1])
    return ctype, name

def strip_cv(t) -> str:
    t = re.sub(r'\bconst\b', '', t)
    t = re.sub(r'\bvolatile\b', '', t)
    t = re.sub(r'\brestrict\b', '', t)
    t = re.sub(r'\b__restrict\b', '', t)
    t = re.sub(r'\s+', ' ', t)
    return t.strip()

def fmt_et(base, extra_ptr=0) -> str:
    if extra_ptr == 0:
        return f"ARG({base})"
    if extra_ptr == 1:
        return f"PTR({base})"
    if extra_ptr == 2:
        return f"PTR2({base})"
    return f"ET({base}, {extra_ptr})"

def map_scalar_type(base) -> str:
    if base == "void":
        return "HIR_TMPVARI0"
    if base in SPECIAL_U32_TYPES:
        return "HIR_TMPVARU32"
    if base in SPECIAL_U64_TYPES:
        return "HIR_TMPVARU64"
    if base in I32_TYPES:
        return "HIR_TMPVARI32"
    if base in U32_TYPES:
        return "HIR_TMPVARU32"
    if base in I64_TYPES:
        return "HIR_TMPVARI64"
    if base in U64_TYPES:
        return "HIR_TMPVARU64"
    if base in U16_TYPES:
        return "HIR_TMPVARU16"
    if base in I16_TYPES:
        return "HIR_TMPVARI16"
    if base in U8_TYPES:
        return "HIR_TMPVARU8"
    if base in I8_TYPES:
        return "HIR_TMPVARI8"
    return None

def map_decl(ctype, name) -> str:
    ctype = norm_spaces(ctype)
    ctype = strip_cv(ctype)

    ptr_level = ctype.count('*')
    base = ctype.replace('*', '').strip()

    # char* -> STR, char** -> PTR(STR)
    if base in {"char", "signed char", "unsigned char"} and ptr_level > 0:
        if name in STR_LIST_NAMES:
            return fmt_et("HIR_TMPVARI8", ptr_level)
        if name in STR_NAMES or base == "char":
            return fmt_et("HIR_TMPVARI8", ptr_level)
        
        if "unsigned" in base:
            return fmt_et("HIR_TMPVARU8", ptr_level)
        return fmt_et("HIR_TMPVARI8", ptr_level)

    # struct foo* / void* / anything* -> ARR
    if ptr_level > 0:
        return fmt_et("HIR_TMPVARI0", ptr_level)

    # user_addr_t / caddr_t / uuid_t: address-like abstract pointer
    if base in SPECIAL_ARRAY_TYPES:
        if name in STR_LIST_NAMES:
            return fmt_et("HIR_TMPVARI8", 1)
        if name in STR_NAMES:
            return fmt_et("HIR_TMPVARI8", 1)
        return fmt_et("HIR_TMPVARI0", 1)

    # plain scalar
    s = map_scalar_type(base)
    if s is not None:
        return fmt_et(s, 0)

    # structs/unions by value: very rare, fallback to U64
    if base.startswith("struct ") or base.startswith("union "):
        return fmt_et("HIR_TMPVARU64", 0)

    # fallback
    return fmt_et("HIR_TMPVARU64", 0)

def parse_proto(proto) -> tuple:
    m = proto_re.match(proto.strip())
    if not m:
        return None

    ret_t, name, args_s = m.groups()
    args = []
    for a in split_args(args_s):
        ctype, aname = split_decl(a)
        if ctype is None:
            continue
        args.append((ctype, aname))

    return ret_t.strip(), name.strip(), args

def syscall_line(num, ret_t, name, args) -> str:
    r = map_decl(ret_t, "", is_ret=True)
    aa = [map_decl(t, n, False) for t, n in args]
    argc = len(aa)
    items = ", ".join([str(num), r] + aa)
    return f"[{num}] = SYSCALL{argc}({items}), /* {name} */"

def main() -> None:
    out = []
    max_num = 0

    with open(SRC, "r", encoding="utf-8") as f:
        for raw in f:
            s = raw.strip()

            if not s:
                continue
            if s.startswith(";"):
                continue
            if s.startswith("#include"):
                continue

            if s.startswith("#if") or s.startswith("#else") or s.startswith("#endif"):
                out.append(s)
                continue

            m = line_re.match(raw)
            if not m:
                continue

            num = int(m.group(1))
            proto = m.group(2).strip()
            max_num = max(max_num, num)

            parsed = parse_proto(proto)
            if not parsed:
                out.append(f"/* [{num}] parse failed: {proto} */")
                continue

            ret_t, name, args = parsed

            if name in {"nosys", "enosys"}:
                continue

            out.append(syscall_line(num, ret_t, name, args))

    with open(OUT, "w", encoding="utf-8") as g:
        g.write("static const syscall_t macos_syscalls[] = {\n")
        for x in out:
            g.write(f"    {x}\n")
        g.write("};\n")

    print(f"written: {OUT}")
    print(f"max syscall number: {max_num}")

if __name__ == "__main__":
    main()
