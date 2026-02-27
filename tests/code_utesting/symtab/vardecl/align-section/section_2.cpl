{
    section(".text") {
        glob i32 r;
    }
    glob i32 a;
    section(".test") {
        glob i32 b;
        ro i32 c;
    }
    glob i32 d;
    section (".life-is-strange") {
        glob i32 t;
    }
    ro i32 y;
}

: OUTPUT
{id: 1, a, i32, s_id: 1, align: 8}
{id: 4, d, i32, s_id: 1, align: 8}
{id: 3, c, i32, s_id: 1, align: 8}
{id: 0, r, i32, s_id: 1, align: 8}
{id: 5, t, i32, s_id: 1, align: 8}
{id: 6, y, i32, s_id: 1, align: 8}
{id: 2, b, i32, s_id: 1, align: 8}
{section='.data', <<1 4>>}
{section='.rodata', 6}
{section='.test', <<3 2>>}
{section='.text', 0}
{section='.life-is-strange', 5}
: