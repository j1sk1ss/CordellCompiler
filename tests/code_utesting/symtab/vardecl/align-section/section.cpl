{
    section(".text") {
        glob i32 a;
        glob i32 b;
        glob i32 c;
    }
}

: OUTPUT
{id: 0, a, i32, s_id: 1, align: 8}
{id: 1, b, i32, s_id: 1, align: 8}
{id: 2, c, i32, s_id: 1, align: 8}
{section='.data'}
{section='.text', <<0 1 2>>}
: