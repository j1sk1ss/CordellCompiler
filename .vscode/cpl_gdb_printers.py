import gdb
import gdb.printing
import os
import sys


sys.path.insert(0, os.path.dirname(__file__))
from cpl_container_hints import is_pointer_type, pointee_type_name, pointer_depth, value_hint


MAX_CHILDREN = 256


def _unqualified(typ):
    try:
        return typ.unqualified()
    except Exception:
        return typ


def _named_type(typ):
    typ = _unqualified(typ)
    try:
        if typ.code == gdb.TYPE_CODE_REF:
            typ = _unqualified(typ.target())
    except Exception:
        pass
    try:
        if typ.code == gdb.TYPE_CODE_PTR:
            typ = _unqualified(typ.target())
    except Exception:
        pass
    return typ


def _type_matches(val, type_name):
    typ = _named_type(val.type)
    names = [
        getattr(typ, "name", None),
        getattr(typ, "tag", None),
        str(typ),
    ]
    return type_name in names


def _as_object(val):
    typ = _unqualified(val.type)
    try:
        if typ.code == gdb.TYPE_CODE_REF:
            return val.referenced_value()
        if typ.code == gdb.TYPE_CODE_PTR:
            if int(val) == 0:
                return None
            return val.dereference()
    except Exception:
        return None
    return val


def _to_int(val, default=0):
    try:
        return int(val)
    except Exception:
        return default


def _field(val, name):
    try:
        return val[name]
    except Exception:
        return None


def _ptr_text(val):
    if val is None:
        return "NULL"
    try:
        addr = int(val)
        return "NULL" if addr == 0 else "0x%x" % addr
    except Exception:
        return str(val)


def _is_null_ptr(val):
    if val is None:
        return True
    try:
        return int(val) == 0
    except Exception:
        return True


def _name_from_value(val):
    try:
        return val.name
    except Exception:
        return None


def _lookup_type(type_name):
    type_name = type_name.strip()
    depth = pointer_depth(type_name)
    base_name = pointee_type_name(type_name) if depth else type_name

    try:
        typ = gdb.lookup_type(base_name)
    except Exception:
        return None

    for _ in range(depth):
        typ = typ.pointer()

    return typ


def _typed_child_from_void(value, type_name):
    if not type_name or value is None:
        return value

    typ = _lookup_type(type_name)
    if typ is None:
        return value

    try:
        if not is_pointer_type(type_name):
            return gdb.Value(_to_int(value)).cast(typ)

        typed_pointer = value.cast(typ)
        if pointer_depth(type_name) == 1 and int(value) != 0:
            return typed_pointer.dereference()
        return typed_pointer
    except Exception:
        return value


def _map_entries(map_value):
    capacity = max(_to_int(_field(map_value, "capacity")), 0)
    entries = _field(map_value, "entries")
    if capacity == 0 or _is_null_ptr(entries):
        return

    shown = 0
    for slot in range(capacity):
        if shown >= MAX_CHILDREN:
            break
        try:
            entry = (entries + slot).dereference()
        except Exception:
            break

        if _to_int(_field(entry, "used")):
            yield shown, slot, entry
            shown += 1


class ListPrinter(object):
    def __init__(self, val):
        self.original = val
        self.val = _as_object(val)

    def to_string(self):
        if self.val is None:
            return "list_t NULL"

        size = _to_int(_field(self.val, "s"))
        head = _ptr_text(_field(self.val, "h"))
        tail = _ptr_text(_field(self.val, "t"))
        suffix = "" if size <= MAX_CHILDREN else ", showing first %d" % MAX_CHILDREN
        hint = value_hint("list", _name_from_value(self.original), None)
        hint_suffix = "" if not hint else " items=%s" % hint
        return "list_t size=%d%s%s head=%s tail=%s" % (size, suffix, hint_suffix, head, tail)

    def children(self):
        if self.val is None:
            return

        expected = max(_to_int(_field(self.val, "s")), 0)
        node = _field(self.val, "h")
        seen = set()

        for index in range(min(expected, MAX_CHILDREN)):
            if _is_null_ptr(node):
                break

            addr = _to_int(node)
            if addr in seen:
                break
            seen.add(addr)

            try:
                current = node.dereference()
            except Exception:
                break

            hint = value_hint("list", _name_from_value(self.original), None)
            yield "[%d]" % index, _typed_child_from_void(_field(current, "data"), hint)
            node = _field(current, "n")

    def display_hint(self):
        return "array"


class MapPrinter(object):
    def __init__(self, val):
        self.original = val
        self.val = _as_object(val)

    def to_string(self):
        if self.val is None:
            return "map_t NULL"

        size = _to_int(_field(self.val, "size"))
        capacity = _to_int(_field(self.val, "capacity"))
        compressed = _to_int(_field(self.val, "compr"))
        cmp_flag = _to_int(_field(self.val, "cmp"))
        suffix = "" if size <= MAX_CHILDREN else ", showing first %d" % MAX_CHILDREN
        hint = value_hint("map", _name_from_value(self.original), None)
        hint_suffix = "" if not hint else " values=%s" % hint
        return "map_t size=%d%s%s capacity=%d compressed=%d cmp=%d" % (
            size,
            suffix,
            hint_suffix,
            capacity,
            compressed,
            cmp_flag,
        )

    def children(self):
        if self.val is None:
            return

        for index, slot, entry in _map_entries(self.val):
            key = _to_int(_field(entry, "key"))
            hint = value_hint("map", _name_from_value(self.original), None)
            yield "[%d] key=%d slot=%d" % (index, key, slot), _typed_child_from_void(_field(entry, "value"), hint)

    def display_hint(self):
        return "array"


class SetPrinter(object):
    def __init__(self, val):
        self.original = val
        self.val = _as_object(val)

    def to_string(self):
        if self.val is None:
            return "set_t NULL"

        body = _field(self.val, "body")
        size = _to_int(_field(body, "size"))
        capacity = _to_int(_field(body, "capacity"))
        compressed = _to_int(_field(body, "compr"))
        suffix = "" if size <= MAX_CHILDREN else ", showing first %d" % MAX_CHILDREN
        hint = value_hint("set", _name_from_value(self.original), None)
        hint_suffix = "" if not hint else " items=%s" % hint
        return "set_t size=%d%s%s capacity=%d compressed=%d" % (
            size,
            suffix,
            hint_suffix,
            capacity,
            compressed,
        )

    def children(self):
        if self.val is None:
            return

        body = _field(self.val, "body")
        for index, slot, entry in _map_entries(body):
            key = _to_int(_field(entry, "key"))
            hint = value_hint("set", _name_from_value(self.original), None)
            yield "[%d] key=%d slot=%d" % (index, key, slot), _typed_child_from_void(_field(entry, "value"), hint)

    def display_hint(self):
        return "array"


class CplPrettyPrinter(object):
    name = "cpl"
    enabled = True

    def __call__(self, val):
        if _type_matches(val, "list_t"):
            return ListPrinter(val)
        if _type_matches(val, "map_t"):
            return MapPrinter(val)
        if _type_matches(val, "set_t"):
            return SetPrinter(val)
        return None


def register_cpl_printers(objfile=None):
    target = objfile or gdb.current_objfile() or gdb
    gdb.printing.register_pretty_printer(target, CplPrettyPrinter(), replace=True)


register_cpl_printers()
