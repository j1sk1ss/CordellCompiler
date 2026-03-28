function foo(i32 a) -> i32 {
    i32 b = a + 10 as i32;
    return b;
}

start() {
    foo(10);
}

:/ OUTPUT
{id: 1, b, i32, s_id: 3}
{id: 4, a, i32, s_id: 2, value=10}
{id: 3, tmp, i32, s_id: -1, value=20}
{id: 0, a, i32, s_id: 2}
{id: 5, b, i32, s_id: 3, value=20}
{id: 2, tmp, i32, s_id: -1, value=10}
/:
