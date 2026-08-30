import os
import re


WORKSPACE_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

COMMENT_HINT_MARKER = re.compile(r"@(?:pretty|items?|value|values?)\s*:\s*(.+)", re.IGNORECASE)
DECL_WITH_COMMENT = re.compile(
    r"\b(?P<kind>list_t|map_t|set_t)\s+(?P<decls>[^;]+);\s*/\*\s*(?P<comment>.*?)\s*\*/"
)
EXPLICIT_POINTER_TYPE = re.compile(
    r"\b(?P<type>(?:const\s+)?(?:(?:unsigned|signed)\s+)?[A-Za-z_][A-Za-z0-9_]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)?\s*(?:\*\s*)+)"
)

SCALAR_TYPES = (
    "symbol_id_t",
    "scope_id_t",
    "section_elem_type_t",
    "token_type_t",
    "hir_operation_t",
    "hir_subject_type_t",
    "lir_operation_t",
    "lir_subject_type_t",
    "lir_registers_t",
    "cfg_block_type_t",
    "long",
    "int",
    "char",
)

BUILTIN_VALUE_HINTS = {
    "list": {
        "tokens": "token_t *",
        "token_lists": "list_t *",
        "object_files": "char *",
        "cmd": "char *",
        "hirctx.cold.blocks": "hir_block_t *",
        "cfgctx.funcs": "cfg_func_t *",
        "cfgctx.outs.hout": "hir_block_t *",
        "cfgctx.outs.lout": "lir_block_t *",
        "callctx.entries": "symbol_id_t",
        "ret_blocks": "cfg_block_t *",
        "external_preds": "cfg_block_t *",
        "linear": "hir_block_t *",
        "fixes": "lir_block_t *",
        "post_fixes": "lir_block_t *",
        "queue": "instructions_dag_node_t *",
        "ready_list": "instructions_dag_node_t *",
        "scheduled": "instructions_dag_node_t *",
    },
    "map": {
        "colors": "lir_registers_t",
        "block_alignment": "long",
        "lexems": "token_type_t",
        "nodes": "igraph_node_t *",
        "alive_edges": "instructions_dag_node_t *",
        "fmap": "cfg_func_t *",
        "homes": "hir_block_t *",
        "defs": "hir_block_t *",
        "labels": "cfg_block_t *",
        "fcalls": "list_t *",
        "frets": "hir_subject_t *",
        "lmap": "list_t *",
        "verts": "call_graph_node_t *",
    },
    "set": {
        "v": "symbol_id_t",
        "live": "symbol_id_t",
        "use": "symbol_id_t",
        "def": "symbol_id_t",
        "curr_in": "symbol_id_t",
        "curr_out": "symbol_id_t",
        "prev_in": "symbol_id_t",
        "prev_out": "symbol_id_t",
        "copy_gen": "symbol_id_t",
        "copy_kill": "symbol_id_t",
        "addr_taken": "symbol_id_t",
        "used_colors": "lir_registers_t",
        "broken_funcs": "symbol_id_t",
        "visited_funcs": "symbol_id_t",
        "pred": "cfg_block_t *",
        "dom": "cfg_block_t *",
        "domf": "cfg_block_t *",
        "leaders": "hir_block_t *",
        "locals": "cfg_func_t *",
        "callee_restored_blocks": "cfg_block_t *",
        "loop_blocks": "cfg_block_t *",
        "loop_hir": "hir_block_t *",
        "invariant_defs": "hir_block_t *",
        "owners": "symbol_id_t",
        "delown": "symbol_id_t",
        "slaves": "symbol_id_t",
        "sources": "hir_subject_t *",
        "link": "unsigned long",
    },
}

COMMENT_SEARCH_DIRS = (
    "include",
    "src",
    "tests/code_utesting",
    "tests/std_utesting",
)

_COMMENT_HINTS = None


def normalize_type_name(type_name):
    type_name = type_name.strip()
    type_name = re.sub(r"\s+", " ", type_name)
    type_name = re.sub(r"\s*\*\s*", " *", type_name)
    return type_name.strip()


def is_pointer_type(type_name):
    return "*" in type_name


def pointer_depth(type_name):
    return type_name.count("*")


def pointee_type_name(type_name):
    return normalize_type_name(type_name.replace("*", ""))


def _container_kind(type_name):
    return {
        "list_t": "list",
        "map_t": "map",
        "set_t": "set",
    }.get(type_name)


def _declared_names(decls):
    names = []
    for decl in decls.split(","):
        decl = decl.strip()
        decl = decl.split("=")[0].strip()
        match = re.search(r"([A-Za-z_][A-Za-z0-9_]*)$", decl)
        if match:
            names.append(match.group(1))
    return names


def _type_hint_from_text(text):
    text = text.strip()
    if text.startswith(":"):
        token = text[1:].strip().split()[0]
        if token:
            return normalize_type_name(token)

    pointer = EXPLICIT_POINTER_TYPE.search(text)
    if pointer:
        return normalize_type_name(pointer.group("type"))

    for scalar in SCALAR_TYPES:
        if re.search(r"\b%s\b" % re.escape(scalar), text):
            return normalize_type_name(scalar)

    return None


def _comment_hint(comment):
    marked = COMMENT_HINT_MARKER.search(comment)
    return _type_hint_from_text(marked.group(1) if marked else comment)


def _merge_unique(target, kind, name, hint):
    current = target[kind].get(name)
    if current is None:
        target[kind][name] = hint
    elif current != hint:
        target[kind][name] = None


def _load_comment_hints():
    hints = {"list": {}, "map": {}, "set": {}}

    for rel_dir in COMMENT_SEARCH_DIRS:
        root = os.path.join(WORKSPACE_ROOT, rel_dir)
        if not os.path.isdir(root):
            continue

        for dirpath, dirnames, filenames in os.walk(root):
            dirnames[:] = [name for name in dirnames if name not in (".git", "build", "__pycache__")]
            for filename in filenames:
                if not filename.endswith((".c", ".h")):
                    continue

                path = os.path.join(dirpath, filename)
                try:
                    with open(path, "r", encoding="utf-8", errors="ignore") as source:
                        lines = source.readlines()
                except OSError:
                    continue

                for line in lines:
                    match = DECL_WITH_COMMENT.search(line)
                    if not match:
                        continue

                    kind = _container_kind(match.group("kind"))
                    hint = _comment_hint(match.group("comment"))
                    if not kind or not hint:
                        continue

                    for name in _declared_names(match.group("decls")):
                        _merge_unique(hints, kind, name, hint)

    for kind in hints:
        hints[kind] = {
            name: hint
            for name, hint in hints[kind].items()
            if hint is not None
        }

    return hints


def _comment_hints():
    global _COMMENT_HINTS
    if _COMMENT_HINTS is None:
        _COMMENT_HINTS = _load_comment_hints()
    return _COMMENT_HINTS


def _path_candidates(name, expression_path):
    candidates = []
    if expression_path:
        path = expression_path.strip()
        candidates.append(path)
        if path.startswith("&"):
            candidates.append(path[1:])
        if path.startswith("(*") and path.endswith(")"):
            candidates.append(path[2:-1])
        leaf = re.split(r"[.\]>)]", path)[-1]
        if leaf:
            candidates.append(leaf)

    if name:
        candidates.append(name.strip())

    seen = set()
    result = []
    for candidate in candidates:
        candidate = candidate.strip()
        if candidate and candidate not in seen:
            seen.add(candidate)
            result.append(candidate)
    return result


def value_hint(kind, name=None, expression_path=None):
    builtin = BUILTIN_VALUE_HINTS.get(kind, {})
    comments = _comment_hints().get(kind, {})

    for candidate in _path_candidates(name, expression_path):
        if candidate in comments:
            return comments[candidate]
        if candidate in builtin:
            return builtin[candidate]

    return None
