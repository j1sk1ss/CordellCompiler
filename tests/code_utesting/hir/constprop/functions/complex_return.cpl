function foo(i32 a, i32 b) {
    return a + b;
}

start() {
    i32 a = 10 as i32;
    i32 b = 10 as i32;
    i32 c = foo(a, b) as i32;
}

:/ OUTPUT
{id: 2, a, i32, s_id: 4}
{id: 9, tmp, i64, s_id: -1, value=20}
{id: 12, a, i32, s_id: 4, value=10}
{id: 14, c, i32, s_id: 4, value=20}
{id: 1, b, i32, s_id: 2}
{id: 4, c, i32, s_id: 4}
{id: 3, b, i32, s_id: 4}
{id: 7, tmp, i32, s_id: -1, value=10}
{id: 13, b, i32, s_id: 4, value=10}
{id: 11, b, i32, s_id: 2, value=10}
{id: 10, a, i32, s_id: 2, value=10}
{id: 0, a, i32, s_id: 2}
{id: 5, tmp, i32, s_id: -1, value=20}
{id: 6, tmp, i32, s_id: -1, value=10}
{id: 8, tmp, i32, s_id: -1, value=20}
/:
