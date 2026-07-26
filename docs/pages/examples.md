# Examples

The examples below are based on files from `tests/code_utesting/**/*.cpl`.

## Arithmetic and casts

```cpl
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 x = (1 + 2) * 3 - 2;
    put_digit(x);
    exit 0;
}
```

## If / else chain

```cpl
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i64 x = 2;
    if x == 1; putc('A');
    else if x == 2; putc('B');
    else putc('C');
    exit 0;
}
```

## Arrays

```cpl
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    arr a[3, i8] = { 'A', 'B', 'C' };
    putc(a[1]);
    exit 0;
}
```

## Default arguments

```cpl
function add(i64 a, i64 b = 2) -> i64;

function add(i64 a, i64 b = 2) -> i64 {
    return a + b;
}

start() {
    exit add(3) as u8;
}
```

## Generic function

```cpl
function sum<U, T>(U a, T b) -> T {
    return a as T + b;
}

start() {
    exit sum<i8, u8>(1, 1);
}
```

## Function pointer

```cpl
function inc(i64 a) -> i64 {
    return a + 1;
}

start() {
    ptr i0 f = inc;
    exit f(4) as u8;
}
```

## Lambda

```cpl
start() {
    ptr i0 add = (i64 a, i64 b) => a + b;
    exit add(2, 3) as u8;
}
```

## Direct syscall write

```cpl
start() {
    i8 c = 'S';
    syscall(0x2000004, 1, ref c, 1);
    exit 0;
}
```

## String length

```cpl
function strlen(ptr i8 s) -> i64 {
    i64 l = 0;
    while dref s; {
        s += 1;
        l += 1;
    }

    return l;
}

start() {
    exit strlen(ref "abc") as u8;
}
```

## Header-style organization

Header:

```cpl
#ifndef PRINT_H_
#define PRINT_H_ 0
function print(ptr i8 s) -> i0;
#endif
```

Implementation:

```cpl
#include "print_h.cpl"

function print(ptr i8 s) -> i0 {
    syscall(0x2000004, 1, s, strlen(s));
}
```

## HTTP HTML page

`examples/small/http_page.cpl` starts a small localhost HTTP server with
`cpllib/http`, serves `examples/small/http_page.html` from `/`, and exposes the
diagram image through a static route.

```cpl
#include <http_h.cpl>

glob http_server server;

function index(ptr http_request req, ptr http_response res) -> i0 {
    res.html_file(ref "examples/small/http_page.html");
}

function stop(ptr http_request req, ptr http_response res) -> i0 {
    res.text(200 as i32, ref "stopped\n");
    server.stop();
}

start() {
    server.init(ref "127.0.0.1", 18083);
    if not server.get(ref "/", index); exit 1;
    if not server.get(ref "/stop", stop); exit 2;
    if not server.static(ref "/assets/", ref "examples/small/"); exit 3;
    exit server.listen() as u8;
}
```

Build and run:

```bash
builds/darwin-x86_64/cplc --output /private/tmp/cpl_http_page_example examples/small/http_page.cpl
/private/tmp/cpl_http_page_example
```

Open `http://127.0.0.1:18083/`. The page button loads
`/assets/http_page_diagram.svg` from the same CPL server. Stop the example with:

```bash
curl http://127.0.0.1:18083/stop
```
