export const homeExamples = {
  start: {
    label: "Start",
    file: "hello.cpl",
    exit: 0,
    code: `start() {
    exit 0;
}`
  },
  pointers: {
    label: "Memory",
    file: "memory.cpl",
    exit: 18,
    code: `start() {
    i32 value = 10;
    ptr i32 address = ref value;
    dref address = 18;
    exit value as u8;
}`
  },
  functions: {
    label: "Functions",
    file: "functions.cpl",
    exit: 15,
    code: `function add(i64 a, i64 b = 2) -> i64 {
    return a + b;
}

start() {
    exit add(13) as u8;
}`
  },
  arrays: {
    label: "Arrays",
    file: "arrays.cpl",
    exit: 29,
    code: `start() {
    arr numbers[4, i32] = { 3, 5, 8, 13 };
    i32 total = 0;
    i32 index = 0;

    while index < 4; {
        total += numbers[index];
        index += 1;
    }

    exit total as u8;
}`
  },
  generics: {
    label: "Generics",
    file: "generics.cpl",
    exit: 24,
    code: `function sum<T>(T a, T b) -> T {
    return a + b;
}

start() {
    exit sum<i32>(9, 15) as u8;
}`
  },
  containers: {
    label: "Containers",
    file: "containers.cpl",
    exit: 16,
    code: `container point {
    i32 x;
    i32 y;
}

start() {
    point p = { 6, 10 };
    exit (p.x + p.y) as u8;
}`
  },
  interfaces: {
    label: "Interfaces",
    file: "interfaces.cpl",
    exit: 27,
    code: `interface value {
    @[self] function get(ptr value self) -> i32;
}

container sample implements value {
    @[override] function get(ptr sample self) -> i32;
}

function sample::get(ptr sample self) -> i32 {
    return 27;
}

start() {
    sample a;
    ptr value v = ref a;
    exit v.get() as u8;
}`
  },
  defer: {
    label: "Defer",
    file: "defer.cpl",
    exit: 8,
    code: `function finish(ptr i32 p) -> i0 {
    dref p = 8;
}

function work(ptr i32 p) -> i0 {
    defer finish(p);
    dref p = 1;
}

start() {
    i32 value = 0;
    work(ref value);
    exit value as u8;
}`
  },
};
