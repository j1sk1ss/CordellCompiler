start() {
    i32 a = 5;
    ptr i32 b = ref a;
    i32 c = dref b;
}

:/ OUTPUT
{id: 8, c, i32, s_id: 2, value=5}
{id: 1, b, ptr i32, s_id: 2}
{id: 4, tmp, i32, s_id: -1}
{id: 3, tmp, i32, s_id: -1, value=5}
{id: 0, a, i32, s_id: 2}
{id: 7, b, ptr i32, s_id: 2}
{id: 5, tmp, i32, s_id: -1, value=5}
{id: 6, a, i32, s_id: 2, value=5}
{id: 2, c, i32, s_id: 2}
/: