@[entry]
function foo() {
    i32 a = 1;
    if 1; {
        a = 0;
    }
    ptr i32 b = a as ptr i32;
    exit dref b;
}

: OUTPUT
{id: 2, tmp, i32, s_id: 0, value=1}
{id: 9, a, i32, s_id: 3, value=0}
{id: 1, b, ptr i32, s_id: 3}
{id: 4, tmp, i32, s_id: 0}
{id: 3, tmp, i32, s_id: 0, value=0}
{id: 7, a, i32, s_id: 3}
{id: 0, a, i32, s_id: 3}
{id: 5, tmp, i32, s_id: 0}
{id: 6, a, i32, s_id: 3, value=1}
{id: 8, b, ptr i32, s_id: 3}
: