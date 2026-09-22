# Containers

Containers are CPL value-layout user-defined types. Their basic memory model is still closer to C structs than to C++-style classes: declaring a local container creates ordinary function-local storage, fields are addressed by offset, and there is no hidden heap allocation, constructor, destructor, or automatic lifetime management. This remains true for ordinary containers, including containers with regular `@[self]` methods. CPL adds virtual-table machinery only when a container participates in inheritance or implements an interface, and only inherited methods use that virtual dispatch path.

```cpl
container storage {
    i32 count;
    i8  tag;
    i64 total;
}

start() {
    storage s = { 10, 5, 11 };
    exit (s.count + s.tag + s.total) as u8; :/ 26 /:
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

The rules above describe the complete layout of an ordinary container. A container does not gain hidden virtual-table state merely because it contains functions or `@[self]` methods. Compiler-managed virtual-table state is added only when the container participates in inheritance or interface implementation and therefore needs inherited methods to be dispatched virtually.

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
    arr   color[4, i8];
}

start() {
    rect r;

    r.left.x   = 3;
    r.left.y   = 5;
    r.right.x  = 7;
    r.right.y  = 11;
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
    arr    body[16, i8];
}

glob text_pair pair = { "Title", "Body" };
```

## Pointer Fields

Containers can reference values of their own type through pointers. This is useful for linked structures.

```cpl
container node {
    ptr node next;
    i32      value;
}

start() {
    node a;
    node b;

    a.value = 4;
    b.value = 8;
    a.next  = ref b;
    b.next  = 0;

    exit (a.value + a.next.value) as u8;
}
```

Field access through a pointer field loads the pointer value before applying the next field offset.

## Functions in Containers

A container may contain functions. By default, these are still regular functions with convenient namespace and call syntax. Adding functions, including `@[self]` methods, does not by itself create a virtual table or change the container's C-like value layout. Virtual behavior appears only for methods that belong to an inheritance or interface method set; inherited methods and their `@[override]` implementations use the virtual-table path, while unrelated methods remain direct calls.

```cpl
container math_box {
    function add(i32 a, i32 b) -> i32 {
        return a + b;
    }
}

start() {
    exit math_box::add(20, 22) as u8;
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
    exit math_box::add(20, 22) as u8;
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
    exit caster::pick<u8>(42) as u8;
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
    slot s = { 10 };
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
    counter c = { 10 };
    c.add(7);
    exit c.value as u8;
}
```

For an ordinary method, the call `c.add(7)` is lowered as a normal function call where `ref c` is passed as the explicit `self` argument. This is still the default `@[self]` mechanism. A `@[self]` annotation alone does not make a method virtual and does not add a vtable to the container. Only methods inherited through a container/interface relationship occupy virtual-table slots and are dispatched through the container's virtual-table linkage.

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

## Virtual Tables, Abstract Methods, and Overrides

Virtual tables are not a property of every CPL container. They are used only for inheritance/interface dispatch. If a container does not inherit or implement anything, its layout and method calls remain the same as before: visible fields are stored directly, and ordinary methods are direct calls with explicit `self` rewriting where needed.

Likewise, not every method of a derived container becomes virtual. The vtable contains the inherited method set: methods declared by a base/interface for inheritance and the corresponding implementations supplied with `@[override]`. Methods that are unrelated to that inherited set remain ordinary direct-call container functions.

A method marked `@[abstract]` in a base container declares such an inherited method slot. A container with an abstract method cannot be instantiated directly.

```cpl
container base {
    @[abstract] @[self]
    function init(ptr base self) -> i0;
}
```

A derived container uses `::` and provides a method marked `@[override]` for the inherited virtual-table slot:

```cpl
container implementation::base {
    i32 body;

    @[override]
    function init(ptr implementation self) -> i0;
}

function implementation::init(ptr implementation self) -> i0 {
    self.body = 0 as i32;
}
```

Virtual-table methods have strict slot indices. The same inherited method therefore occupies the same position for compatible child containers. This lets code accept a pointer to the base container and dispatch to the implementation associated with the actual object:

```cpl
container base {
    @[abstract] @[self]
    function do(ptr base self) -> i0;
}

container first::base {
    @[override]
    function do(ptr first self) -> i0 {
    }
}

container second::base {
    @[override]
    function do(ptr second self) -> i0 {
    }
}

glob first f;
glob second s;

function easy(ptr base b) -> i0 {
    b.do();
}

start() {
    easy(ref f); :/ first::do /:
    easy(ref s); :/ second::do /:
}
```

The virtual table is compiler-managed, but it exists only for the inherited dispatch surface. Unlike ordinary `@[self]` call rewriting, a call to an inherited virtual method depends on the object's virtual-table linkage and the strict slot assigned to that inherited method. Other methods of the same container can still be ordinary direct calls.

This part of CPL is newer than the basic container model. The changelog still describes inheritance and virtual-table support as work in progress, so code that depends on these features should not assume that every object-model edge case is already stable.

## Interfaces

`interface` declares a common virtual method contract. Functions declared in an interface are abstract by default, so they do not need an explicit `@[abstract]` annotation.

```cpl
interface base {
    @[self]
    function init(ptr base self) -> i0;
}

container implementation::base {
    @[override]
    function init(ptr implementation self) -> i0;
}

function implementation::init(ptr implementation self) -> i0 {
}
```

The `container implementation::base` syntax associates the container with the inherited base/interface method layout. An overriding method occupies the corresponding virtual-table slot. Methods defined only on `implementation` and not inherited from `base` remain ordinary methods and do not need vtable entries. This provides a narrow common dispatch surface without turning the whole container into a managed class.

## Placement in Manually Allocated Memory

For an ordinary container with no inherited virtual methods, manually allocated storage does not require any special vtable initialization because no vtable exists. The `place` operation matters only for a container that participates in inheritance/interface dispatch. A normally declared instance of such a container gives the compiler a point at which it can initialize the required virtual-table linkage. Raw memory returned by an allocator does not. Casting that allocation to `ptr <container>` changes the pointer type, but does not initialize the virtual-table link required by inherited virtual dispatch:

```cpl
interface a {
    @[self]
    function foo(ptr a self) -> i0;
}

container b::a {
    @[override]
    function foo(ptr b self) -> i0;
}

function create_raw() -> ptr b {
    malloc(sizeof(b)) as ptr b; :/ Raw memory: vtable link is not initialized /:
}
```

Use `place(pointer, type)` when a container with inherited virtual methods must be placed into storage that was allocated manually:

```cpl
function create() -> ptr b {
    place(malloc(sizeof(b)), b);
}
```

`place` does **not** allocate memory and is not CPL's equivalent of `new`. It accepts already available storage and performs the placement work required for the inherited virtual-dispatch state, including linking the object to the appropriate virtual table. An ordinary container that has no such inherited method set keeps the old C-like model and does not need `place` merely because it has fields or methods. Allocation policy therefore remains explicit and under the programmer's control.

## What Containers Still Do Not Provide

Even with the semi-OOP additions, containers are not C++- or Java-style managed classes. They do not provide:

- constructors or destructors;
- implicit heap allocation;
- access modifiers;
- automatic lifetime management;
- ownership or borrowing checks.
