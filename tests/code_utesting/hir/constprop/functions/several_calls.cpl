: TEST_DEBUG :
function foo(i32 a, i32 b) {
    return a + b;
}

function bar(i32 a, i32 b) {
    return a + b;
}

start() {
    foo(33, 23);
    foo(23, 33);
    bar(20, 20);
    bar(20, 20);
}

: OUTPUT
{id: 2, a, i32, s_id: 4}
{id: 9, b, i32, s_id: 4, value=20}
{id: 1, b, i32, s_id: 2}
{id: 4, tmp, i32, s_id: 0}
{id: 3, b, i32, s_id: 4}
{id: 7, b, i32, s_id: 2}
{id: 0, a, i32, s_id: 2}
{id: 5, tmp, i32, s_id: 0, value=40}
{id: 6, a, i32, s_id: 2}
{id: 8, a, i32, s_id: 4, value=20}
: