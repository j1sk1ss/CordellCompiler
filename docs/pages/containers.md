# Containers

Containers are CPL value-layout user-defined types. They are closer to C structs than to classes: declaring a local container creates ordinary function-local storage, fields are addressed by offset, and there is no hidden allocation, constructor, destructor, inheritance, or virtual dispatch.

```cpl
container storage {
    i32 count;
    i8  tag;
    i64 total;
}

start() {
    storage s;
    s.count = 10;
    s.tag = 5;
    s.total = 11;

    exit (s.count + s.tag + s.total) as u8;
}
```

## Layout and Alignment

Container fields are laid out with target-dependent alignment. By default the compiler uses the target's maximum bytness for field alignment. Use `@[align(N)]` to request a different container alignment:

```cpl
@[align(1)]
container packed {
    i8  a;
    i16 b;
    i32 c;
}
```

The packed example above occupies less space than the default-aligned form, but may generate less natural memory accesses for the target.

Use `@[like_c]` when the container should follow the C-style layout path used by the compiler instead of the default CPL alignment request. This is mostly useful for ABI-facing declarations that mirror libc or OS structures.

```cpl
@[like_c]
container c_pair {
    i32 left;
    i8  tag;
    i32 right;
}
```

## Union Containers

`@[union]` makes a container store all fields at offset zero. The container size is based on the largest field rather than the sum of all fields, similar to a C union.

```cpl
@[union]
container word_view {
    u32 word;
    arr bytes[4, u8];
}

start() {
    word_view v;
    v.word = 0x41424344;
    exit v.bytes[0];
}
```

Only one interpretation should be considered active at a time. CPL does not track active union members, so type punning through a union is explicit low-level code.

## Field Types

Containers may hold primitive fields, pointer fields, arrays, and other containers.

```cpl
container point {
    i32 x;
    i32 y;
}

container rect {
    point left;
    point right;
    arr color[4, i8];
}

start() {
    rect r;

    r.left.x = 3;
    r.left.y = 5;
    r.right.x = 7;
    r.right.y = 11;
    r.color[0] = 255 as i8;

    exit (r.left.x + r.left.y + r.right.x + r.right.y) as u8;
}
```

Nested field access works by composing field offsets. Array fields can be indexed directly, and using an array field as a value gives a pointer to the first element:

```cpl
container buffer {
    arr data[5, i8];
}

start() {
    buffer b;
    b.data[0] = 1;
    b.data[1] = 2;

    ptr i8 p = b.data;
    exit (dref p + b.data[1]) as u8;
}
```

The same reference rule applies to arrays whose element type is a container or another array. Indexing such an array returns a reference to the selected element storage, not a copied value. If `items` is `arr [N, item]`, then `items[0]` is already suitable for a `ptr item` parameter; writing `ref items[0]` is unnecessary.

```cpl
container item {
    i32 value;
}

function use_item(ptr item it) -> i0;

start() {
    arr items[2, item];
    use_item(items[0]);
}
```

For global container initializers, string literals are written without `ref`. The initializer stores a static string reference for `ptr i8` fields and expands the bytes for inline `arr [N, i8]` fields.

```cpl
container text_pair {
    ptr i8 title;
    arr body[16, i8];
}

glob text_pair pair = { "Title", "Body" };
```

## Pointer Fields

Containers can reference values of their own type through pointers. This is useful for linked structures.

```cpl
container node {
    ptr node next;
    i32 value;
}

start() {
    node a;
    node b;

    a.value = 4;
    b.value = 8;
    a.next = ref b;
    b.next = 0;

    exit (a.value + a.next.value) as u8;
}
```

Field access through a pointer field loads the pointer value before applying the next field offset.

## Functions in Containers

A container may contain functions. These functions are still ordinary functions after parsing and devirtualization; the container body mainly gives them a convenient namespace and call syntax.

```cpl
container math_box {
    function add(i32 a, i32 b) -> i32 {
        return a + b;
    }
}

start() {
    math_box box;
    exit box.add(20, 22) as u8;
}
```

Defining a function body directly inside a container creates an implementation wherever that container declaration is parsed. For header-style code, prefer a prototype in the container and a separate implementation marked with `<container>::<function>`:

```cpl
container math_box {
    function add(i32 a, i32 b) -> i32;
}

function math_box::add(i32 a, i32 b) -> i32 {
    return a + b;
}

start() {
    math_box box;
    exit box.add(20, 22) as u8;
}
```

Container functions can also be generic. Generic container functions use the same explicit type-argument syntax as regular generic functions:

```cpl
container caster {
    function pick<T>(T value) -> T {
        return value;
    }
}

start() {
    caster c;
    exit c.pick<u8>(42) as u8;
}
```

Generic functions may also be `@[self]` methods. The self receiver stays explicit, and the generic parameters describe the rest of the function signature:

```cpl
container slot {
    i32 total;

    @[self]
    function add<T>(ptr slot self, T value) -> i0 {
        self.total += value as i32;
    }
}

start() {
    slot s;
    s.total = 10;
    s.add<i8>(5);

    exit s.total as u8;
}
```

## Explicit Self Methods

Use `@[self]` when a container function should receive the object being called on. The function must declare an explicit first parameter for that receiver, usually `ptr <container> self`.

```cpl
container counter {
    i32 value;

    @[self]
    function add(ptr counter self, i32 delta) -> i0;
}

function counter::add(ptr counter self, i32 delta) -> i0 {
    self.value += delta;
}

start() {
    counter c;
    c.value = 10;
    c.add(7);

    exit c.value as u8;
}
```

The call `c.add(7)` is lowered as a normal function call where `ref c` is passed as the explicit `self` argument. There is no implicit object model beyond this call rewriting.

`self` is just a parameter name by convention. The important parts are:

- the function is marked with `@[self]`;
- the first parameter has a type compatible with the receiver, normally `ptr counter self`;
- a separated implementation uses `counter::add`;
- all mutation is explicit through that pointer.

Methods can also call methods on nested container fields:

```cpl
container stats {
    arr values[3, i32];

    @[self]
    function total(ptr stats self) -> i32 {
        return self.values[0] + self.values[1] + self.values[2];
    }
}

container report {
    stats data;
    i32 bonus;

    @[self]
    function score(ptr report self) -> i32 {
        return self.data.total() + self.bonus;
    }
}
```

## What Containers Are Not

Containers are not classes. They do not provide:

- constructors or destructors;
- inheritance;
- virtual methods;
- access modifiers;
- automatic lifetime management;
- ownership or borrowing checks.

Treat them as low-level value types with field layout and optional function call sugar.
