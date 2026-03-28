start() {
    i32 a = 10 as i32;
    i32 b = 1 as i32;
    if b; {
        a = 12 as i32;
    }
    else {
        a = 12 as i32;
    }

    i32 c = a;
}

: OUTPUT
{id: {X}, a, i32, s_id: 2}
{id: {X}, b, i32, s_id: 2}
{id: {X}, b, i32, s_id: 2, value=1}
{id: {X}, c, i32, s_id: 2}
{id: {X}, tmp, i32, s_id: -1, value=12}
{id: {X}, c, i32, s_id: 2}
{id: {X}, tmp, i32, s_id: -1, value=10}
{id: {X}, tmp, i32, s_id: -1, value=1}
{id: {X}, a, i32, s_id: 2, value=10}
{id: {X}, a, i32, s_id: 2, value=12}
{id: {X}, a, i32, s_id: 2}
{id: {X}, tmp, i32, s_id: -1, value=12}
{id: {X}, a, i32, s_id: 2, value=12}
: