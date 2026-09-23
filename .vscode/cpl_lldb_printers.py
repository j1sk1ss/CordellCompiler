import re
import os
import sys

import lldb


sys.path.insert(0, os.path.dirname(__file__))
from cpl_container_hints import is_pointer_type, pointee_type_name, pointer_depth, value_hint


MAX_CHILDREN = 256

LLDB_BASIC_TYPES = {
    "char": lldb.eBasicTypeChar,
    "signed char": lldb.eBasicTypeSignedChar,
    "unsigned char": lldb.eBasicTypeUnsignedChar,
    "short": lldb.eBasicTypeShort,
    "unsigned short": lldb.eBasicTypeUnsignedShort,
    "int": lldb.eBasicTypeInt,
    "unsigned int": lldb.eBasicTypeUnsignedInt,
    "long": lldb.eBasicTypeLong,
    "unsigned long": lldb.eBasicTypeUnsignedLong,
    "long long": lldb.eBasicTypeLongLong,
    "unsigned long long": lldb.eBasicTypeUnsignedLongLong,
}

C_PRIMITIVE_TYPES = set(LLDB_BASIC_TYPES.keys())


def _raw(valobj):
    if not valobj or not valobj.IsValid():
        return None
    try:
        raw = valobj.GetNonSyntheticValue()
        return raw if raw.IsValid() else valobj
    except Exception:
        return valobj


def _is_pointer_value(valobj):
    valobj = _raw(valobj)
    try:
        return bool(valobj.GetType().GetTypeClass() & lldb.eTypeClassPointer)
    except Exception:
        return False


def _deref(valobj):
    valobj = _raw(valobj)
    if not valobj or not valobj.IsValid():
        return None
    if not _is_pointer_value(valobj):
        return valobj
    if valobj.GetValueAsUnsigned(0) == 0:
        return None
    dereferenced = valobj.Dereference()
    return dereferenced if dereferenced.IsValid() else None


def _field(valobj, name):
    valobj = _raw(valobj)
    if not valobj or not valobj.IsValid():
        return None
    child = valobj.GetChildMemberWithName(name)
    return child if child.IsValid() else None


def _int(valobj, default=0):
    if not valobj or not valobj.IsValid():
        return default
    try:
        return valobj.GetValueAsSigned(default)
    except Exception:
        return default


def _uint(valobj, default=0):
    if not valobj or not valobj.IsValid():
        return default
    try:
        return valobj.GetValueAsUnsigned(default)
    except Exception:
        return default


def _ptr_text(valobj):
    addr = _uint(valobj)
    return "NULL" if addr == 0 else "0x%x" % addr


def _clone(valobj, name):
    if not valobj or not valobj.IsValid():
        return valobj
    try:
        return valobj.Clone(name)
    except Exception:
        pass
    try:
        return valobj.CreateValueFromData(name, valobj.GetData(), valobj.GetType())
    except Exception:
        return valobj


def _expression_path(valobj):
    if not valobj or not valobj.IsValid():
        return None

    stream = lldb.SBStream()
    try:
        if valobj.GetExpressionPath(stream):
            return stream.GetData()
    except Exception:
        return None
    return None


def _value_hint(kind, valobj):
    raw = _raw(valobj)
    name = raw.GetName() if raw and raw.IsValid() else None
    path = _expression_path(raw) if raw and raw.IsValid() else None
    return value_hint(kind, name, path)


def _type_is_valid(sbtype):
    try:
        return bool(sbtype and sbtype.IsValid())
    except Exception:
        return False


def _value_is_usable(value):
    if not value or not value.IsValid():
        return False
    try:
        return value.GetError().Success()
    except Exception:
        return True


def _lookup_type(target, type_name):
    type_name = type_name.strip()
    depth = pointer_depth(type_name)
    base_name = pointee_type_name(type_name) if depth else type_name
    base_name = base_name.strip()
    if base_name.startswith("const "):
        base_name = base_name[len("const "):].strip()
    if base_name in C_PRIMITIVE_TYPES:
        return None

    sbtype = target.FindFirstType(base_name)
    if not _type_is_valid(sbtype):
        return None

    for _ in range(depth):
        sbtype = sbtype.GetPointerType()
        if not _type_is_valid(sbtype):
            return None

    return sbtype


def _typed_child_from_void(value, name, type_name):
    if not type_name or not value or not value.IsValid():
        return _clone(value, name)

    if not is_pointer_type(type_name):
        try:
            typed_value = value.CreateValueFromExpression(name, "(%s)%d" % (type_name, _uint(value)))
            return typed_value if _value_is_usable(typed_value) else _clone(value, name)
        except Exception:
            return _clone(value, name)

    base_name = pointee_type_name(type_name)
    if base_name.startswith("const "):
        base_name = base_name[len("const "):].strip()
    if base_name in C_PRIMITIVE_TYPES:
        try:
            typed_value = value.CreateValueFromExpression(name, "(%s)0x%x" % (type_name, _uint(value)))
            return typed_value if _value_is_usable(typed_value) else _clone(value, name)
        except Exception:
            return _clone(value, name)

    target_type = _lookup_type(value.GetTarget(), type_name)
    if not _type_is_valid(target_type):
        return _clone(value, name)

    raw_pointer = _clone(value, name)
    try:
        typed_pointer = raw_pointer.Cast(target_type)
    except Exception:
        typed_pointer = raw_pointer

    if pointer_depth(type_name) != 1:
        return typed_pointer

    address = _uint(value)
    if address == 0:
        return typed_pointer

    pointee_type = target_type.GetPointeeType()
    if not _type_is_valid(pointee_type) or pointee_type.GetName() in ("char", "signed char", "unsigned char"):
        return typed_pointer

    try:
        pointed_value = value.CreateValueFromAddress(name, address, pointee_type)
        return pointed_value if _value_is_usable(pointed_value) else typed_pointer
    except Exception:
        return typed_pointer


def _entry_at(entries, slot):
    if not entries or not entries.IsValid():
        return None

    address = entries.GetValueAsUnsigned(0)
    if address == 0:
        return None

    entry_type = entries.GetType().GetPointeeType()
    byte_size = entry_type.GetByteSize()
    if byte_size <= 0:
        return None

    try:
        entry = entries.CreateValueFromAddress("[%d]" % slot, address + slot * byte_size, entry_type)
        if entry.IsValid():
            return entry
    except Exception:
        pass

    try:
        entry = entries.GetChildAtIndex(slot)
        return entry if entry.IsValid() else None
    except Exception:
        return None


def _map_child_values(map_value, child_type=None):
    capacity = max(_int(_field(map_value, "capacity")), 0)
    entries = _field(map_value, "entries")
    shown = 0

    if capacity == 0 or _uint(entries) == 0:
        return []

    children = []
    for slot in range(capacity):
        if shown >= MAX_CHILDREN:
            break

        entry = _entry_at(entries, slot)
        if entry is None:
            break

        if _int(_field(entry, "used")):
            key = _int(_field(entry, "key"))
            value = _field(entry, "value")
            children.append(_typed_child_from_void(value, "[%d] key=%d slot=%d" % (shown, key, slot), child_type))
            shown += 1

    return children


def _list_child_values(list_value, child_type=None):
    expected = max(_int(_field(list_value, "s")), 0)
    node = _field(list_value, "h")
    seen = set()
    children = []

    for index in range(min(expected, MAX_CHILDREN)):
        address = _uint(node)
        if address == 0 or address in seen:
            break
        seen.add(address)

        current = node.Dereference()
        if not current.IsValid():
            break

        data = _field(current, "data")
        children.append(_typed_child_from_void(data, "[%d]" % index, child_type))
        node = _field(current, "n")

    return children


def list_summary(valobj, internal_dict):
    value = _deref(valobj)
    if value is None:
        return "NULL"

    size = _int(_field(value, "s"))
    suffix = "" if size <= MAX_CHILDREN else ", showing first %d" % MAX_CHILDREN
    hint = _value_hint("list", valobj)
    hint_suffix = "" if not hint else " items=%s" % hint
    return "size=%d%s%s head=%s tail=%s" % (
        size,
        suffix,
        hint_suffix,
        _ptr_text(_field(value, "h")),
        _ptr_text(_field(value, "t")),
    )


def map_summary(valobj, internal_dict):
    value = _deref(valobj)
    if value is None:
        return "NULL"

    size = _int(_field(value, "size"))
    suffix = "" if size <= MAX_CHILDREN else ", showing first %d" % MAX_CHILDREN
    hint = _value_hint("map", valobj)
    hint_suffix = "" if not hint else " values=%s" % hint
    return "size=%d%s%s capacity=%d compressed=%d cmp=%d" % (
        size,
        suffix,
        hint_suffix,
        _int(_field(value, "capacity")),
        _int(_field(value, "compr")),
        _int(_field(value, "cmp")),
    )


def set_summary(valobj, internal_dict):
    value = _deref(valobj)
    if value is None:
        return "NULL"

    body = _field(value, "body")
    size = _int(_field(body, "size"))
    suffix = "" if size <= MAX_CHILDREN else ", showing first %d" % MAX_CHILDREN
    hint = _value_hint("set", valobj)
    hint_suffix = "" if not hint else " items=%s" % hint
    return "size=%d%s%s capacity=%d compressed=%d" % (
        size,
        suffix,
        hint_suffix,
        _int(_field(body, "capacity")),
        _int(_field(body, "compr")),
    )


class ListSyntheticProvider(object):
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.children = []
        self.update()

    def update(self):
        value = _deref(self.valobj)
        self.children = [] if value is None else _list_child_values(value, _value_hint("list", self.valobj))

    def num_children(self):
        return len(self.children)

    def get_child_at_index(self, index):
        if index < 0 or index >= len(self.children):
            return None
        return self.children[index]

    def get_child_index(self, name):
        match = re.match(r"\[(\d+)\]", name)
        return int(match.group(1)) if match else -1

    def has_children(self):
        return bool(self.children)


class MapSyntheticProvider(object):
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.children = []
        self.update()

    def update(self):
        value = _deref(self.valobj)
        self.children = [] if value is None else _map_child_values(value, _value_hint("map", self.valobj))

    def num_children(self):
        return len(self.children)

    def get_child_at_index(self, index):
        if index < 0 or index >= len(self.children):
            return None
        return self.children[index]

    def get_child_index(self, name):
        match = re.match(r"\[(\d+)\]", name)
        return int(match.group(1)) if match else -1

    def has_children(self):
        return bool(self.children)


class SetSyntheticProvider(object):
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.children = []
        self.update()

    def update(self):
        value = _deref(self.valobj)
        body = None if value is None else _field(value, "body")
        self.children = [] if body is None else _map_child_values(body, _value_hint("set", self.valobj))

    def num_children(self):
        return len(self.children)

    def get_child_at_index(self, index):
        if index < 0 or index >= len(self.children):
            return None
        return self.children[index]

    def get_child_index(self, name):
        match = re.match(r"\[(\d+)\]", name)
        return int(match.group(1)) if match else -1

    def has_children(self):
        return bool(self.children)


def __lldb_init_module(debugger, internal_dict):
    module = __name__
    registrations = [
        ("list_t", "list_summary", "ListSyntheticProvider"),
        ("map_t", "map_summary", "MapSyntheticProvider"),
        ("set_t", "set_summary", "SetSyntheticProvider"),
    ]

    for type_name, summary, synthetic in registrations:
        pattern = r"^%s( \*)?$" % type_name
        debugger.HandleCommand(
            'type summary add -x -F %s.%s "%s"' % (module, summary, pattern)
        )
        debugger.HandleCommand(
            'type synthetic add -x -l %s.%s "%s"' % (module, synthetic, pattern)
        )

    debugger.HandleCommand("type category enable default")
