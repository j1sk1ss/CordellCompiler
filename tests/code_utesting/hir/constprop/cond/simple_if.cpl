@[entry]
function foo() {
    ptr i32 a = 1;
    ptr i32 b = 1;
    if 1; {
        a = 0;
        return dref a;
    }

    return 0;
}

:/ OUTPUT
{id: 8, a, ptr i32, s_id: 3, value=0}
{id: 1, b, ptr i32, s_id: 3}
{id: 4, tmp, ptr i32, s_id: -1, value=0}
{id: 3, tmp, ptr i32, s_id: -1}
{id: 0, a, ptr i32, s_id: 3}
{id: 7, b, ptr i32, s_id: 3}
{id: 5, tmp, i32, s_id: -1}
{id: 6, a, ptr i32, s_id: 3, value=1}
{id: 2, tmp, ptr i32, s_id: -1, value=1}
/:
