start() {
    i32 a = 1;
    @[no_fall]
    switch 1; {
        case 1; { a = 1; }
        case 2; { a = 1; }
        default { a = 1; }
    }
    i32 b = a;
}

: OUTPUT
{id: 2, tmp, i32, s_id: 0, value=1}
{id: 9, tmp, i8, s_id: -1, value=1}
{id: 12, a, i32, s_id: 2, value=1}
{id: 14, a, i32, s_id: 2, value=1}
{id: 1, b, i32, s_id: 2}
{id: 4, tmp, i32, s_id: 0, value=1}
{id: 15, a, i32, s_id: 2, value=1}
{id: 3, tmp, i32, s_id: 0, value=1}
{id: 7, tmp, i8, s_id: -1, value=0}
{id: 13, b, i32, s_id: 2, value=1}
{id: 11, a, i32, s_id: 2, value=1}
{id: 10, a, i32, s_id: 2, value=1}
{id: 0, a, i32, s_id: 2}
{id: 5, tmp, i32, s_id: 0, value=1}
{id: 6, tmp, i8, s_id: -1, value=0}
{id: 8, tmp, i8, s_id: -1, value=0}
: