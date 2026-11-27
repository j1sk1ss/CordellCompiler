"""Generate compiler backend scaffolding without changing existing targets."""

import argparse
from dataclasses import dataclass
from pathlib import Path
import re
from typing import Optional


ROOT = Path(__file__).resolve().parents[1]


@dataclass(frozen=True)
class Function:
    result: str
    name: str
    parameters: tuple[tuple[str, str], ...]

    def signature(self, prefix: str) -> str:
        arguments = ", ".join(f"{kind} {name}" for kind, name in self.parameters)
        return f"{self.result} {prefix}_{self.name}({arguments or 'void'})"

    def stub(self, prefix: str, noop: bool = False) -> str:
        lines = [self.signature(prefix) + " {"]
        lines.extend(f"    (void){name};" for _, name in self.parameters)
        if noop:
            lines.append("    return 0;")
        else:
            lines.extend([
                f'    fprintf(stderr, "Backend stub: {prefix}_{self.name} is not implemented\\n");',
                "    abort();",
            ])
        return "\n".join(lines + ["}"])


CONTEXT = (("cfg_ctx_t*", "cctx"), ("sym_table_t*", "smt"))
ASM_FUNCTIONS = {
    "asmgen": (Function("int", "generate_asm", CONTEXT + (("FILE*", "output"),)),),
    "vargen": (Function("const char*", "format_lir_subject", (
        ("lir_subject_t*", "subject"), ("sym_table_t*", "smt"), ("int", "flag"))),),
}
SELECTOR_FUNCTIONS = {
    "inst": (Function("int", "instruction_selection", CONTEXT),),
    "mem": (
        Function("int", "memory_selection", (
            ("cfg_ctx_t*", "cctx"), ("map_t*", "colors"), ("sym_table_t*", "smt"))),
        Function("int", "memory_validation", CONTEXT),
    ),
    "caller_saving": (Function("int", "caller_saving", CONTEXT),),
    "float": (Function("int", "validate_float", (
        ("cfg_block_t*", "block"), ("lir_block_t*", "instruction"), ("sym_table_t*", "smt"))),),
    "misc": (
        Function("lir_subject_t*", "create_tmp", (
            ("lir_registers_t", "reg"), ("lir_subject_t*", "source"),
            ("sym_table_t*", "smt"), ("int", "forced_size"))),
        Function("int", "is_sign_type", (("lir_subject_t*", "subject"), ("sym_table_t*", "smt"))),
        Function("int", "is_simd_type", (("lir_subject_t*", "subject"), ("sym_table_t*", "smt"))),
        Function("lir_operation_t", "get_proper_mov", (
            ("lir_subject_t*", "destination"), ("lir_subject_t*", "source"),
            ("sym_table_t*", "smt"), ("lir_operation_t", "base"))),
    ),
}
PRECOLOR = Function("long", "precolored_reg_to_color", (("lir_registers_t", "reg"),))
PEEPHOLE = Function("int", "peephole_optimization", (("cfg_ctx_t*", "cctx"),))


def identifier(value: str) -> str:
    if not re.fullmatch(r"[a-z][a-z0-9_]*", value, flags=re.ASCII):
        raise argparse.ArgumentTypeError(
            "expected a lowercase identifier starting with a letter (letters, digits, underscores)")
    return value


def header(path: str, includes: tuple[str, ...], prefix: str,
           functions: tuple[Function, ...]) -> str:
    guard = path.upper().replace("/", "_").replace(".", "_") + "_"
    dependencies = "\n".join(f"#include <{include}>" for include in includes)
    declarations = "\n".join(function.signature(prefix) + ";" for function in functions)
    return f"#ifndef {guard}\n#define {guard}\n\n{dependencies}\n\n{declarations}\n\n#endif\n"


def source(include: str, backend: str, prefix: str,
           functions: tuple[Function, ...], noop: bool = False) -> str:
    dependencies = f"#include <{include}>\n"
    if not noop:
        dependencies += "#include <stdio.h>\n#include <stdlib.h>\n"
    definitions = "\n\n".join(function.stub(prefix, noop) for function in functions)
    return f"#ifndef CPL_DISABLE_{backend.upper()}\n{dependencies}\n{definitions}\n#endif\n"


def shared_module(root: Path, name: str, kind: str) -> None:
    if kind == "precolor":
        paths = (f"include/lir/regalloc/{name}_precolor.h", f"src/lir/regalloc/gc/{name}_precolor.c")
        symbol = f"{name}_precolored_reg_to_color"
    else:
        paths = (f"include/lir/peephole/{name}.h", f"src/lir/peephole/{name}.c")
        symbol = f"{name}_peephole_optimization"
    for path in paths:
        location = root / path
        if not location.is_file():
            raise ValueError(f"shared {kind} module is missing: {location}")
        if not re.search(rf"\b{re.escape(symbol)}\s*\(", location.read_text(encoding="utf-8")):
            raise ValueError(f"shared {kind} module does not declare/define {symbol}: {location}")


def generate(arch: str, abi: str, assembly: str, root: Path,
             precolor_from: Optional[str] = None,
             peephole_from: Optional[str] = None) -> dict[str, str]:
    backend = f"{arch}_{abi}_{assembly}"
    precolor = precolor_from or f"{arch}_{abi}"
    peephole = peephole_from or backend
    if precolor_from:
        shared_module(root, precolor_from, "precolor")
    if peephole_from:
        shared_module(root, peephole_from, "peephole")

    files = {}
    asm_header = f"asm/{backend}_asmgen.h"
    selector_header = f"lir/selector/{backend}.h"
    for include, groups, directory, includes in (
        (asm_header, ASM_FUNCTIONS, f"src/asm/{backend}", ("asm/asmgen.h",)),
        (selector_header, SELECTOR_FUNCTIONS, f"src/lir/selector/{backend}",
         ("lir/selector/instsel.h", "lir/selector/memsel.h")),
    ):
        functions = tuple(function for group in groups.values() for function in group)
        files[f"include/{include}"] = header(include, includes, backend, functions)
        for name, group in groups.items():
            files[f"{directory}/{name}.c"] = source(include, backend, backend, group)

    if not precolor_from:
        include = f"lir/regalloc/{precolor}_precolor.h"
        files[f"include/{include}"] = header(include, ("lir/lir.h",), precolor, (PRECOLOR,))
        files[f"src/lir/regalloc/gc/{precolor}_precolor.c"] = source(
            include, backend, precolor, (PRECOLOR,))
    if not peephole_from:
        include = f"lir/peephole/{peephole}.h"
        files[f"include/{include}"] = header(include, ("lir/peephole/peephole.h",), peephole, (PEEPHOLE,))
        files[f"src/lir/peephole/{peephole}.c"] = source(
            include, backend, peephole, (PEEPHOLE,), noop=True)

    return files


def check_destinations(root: Path, files: dict[str, str], backend: str) -> None:
    paths = set(files) | {f"src/asm/{backend}", f"src/lir/selector/{backend}"}
    for relative in sorted(paths):
        destination = root / relative
        if destination.exists() or destination.is_symlink():
            raise ValueError(f"refusing to overwrite existing backend path: {destination}")
        for parent in destination.parents:
            if parent.is_symlink():
                raise ValueError(f"refusing to generate through a symlink: {parent}")
            if parent.exists() and not parent.is_dir():
                raise ValueError(f"parent is not a directory: {parent}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--arch", required=True, type=identifier, help="ISA name, e.g. aarch64")
    parser.add_argument("--abi", required=True, type=identifier, help="ABI/platform variant, e.g. gnu or macho")
    parser.add_argument("--asm", required=True, type=identifier, dest="assembly", help="assembly dialect, e.g. gas")
    parser.add_argument("--output-root", type=Path, default=ROOT, help="destination repository or export directory")
    parser.add_argument("--precolor-from", type=identifier, help="reuse an existing arch_abi precolor module")
    parser.add_argument("--peephole-from", type=identifier, help="reuse an existing backend peephole module")
    parser.add_argument("--dry-run", action="store_true", help="validate and list files without writing")
    args = parser.parse_args()
    root = args.output_root.absolute()
    backend = f"{args.arch}_{args.abi}_{args.assembly}"

    try:
        files = generate(args.arch, args.abi, args.assembly, root,
                         args.precolor_from, args.peephole_from)
        check_destinations(root, files, backend)
        for relative, content in files.items():
            if not args.dry_run:
                destination = root / relative
                destination.parent.mkdir(parents=True, exist_ok=True)
                with destination.open("x", encoding="utf-8", newline="\n") as output:
                    output.write(content)
            print(f"{'Would create' if args.dry_run else 'Created'} {root / relative}")
    except (OSError, ValueError) as error:
        parser.exit(1, f"error: {error}\n")

    print(f"{'Planned' if args.dry_run else 'Generated'} {len(files)} files for {backend}.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
