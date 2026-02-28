{
    section(".test") {
        glob arr a[10, i32];
    }
    section(".ro") {
        ro arr b[10, i32];
    }
    section(".text") {
        ro glob arr c[10, i32];
    }
    glob arr d[10, i32];
}

: OUTPUT
{id: 1, b, arr, s_id: 1, align: 8}
{id: 3, d, arr, s_id: 1, align: 8}
{id: 0, a, arr, s_id: 1, align: 8}
{id: 2, c, arr, s_id: 1, align: 8}
{section='.data', 3}
{section='.rodata'}
{section='.test', 0}
{section='.ro', 1}
{section='.text', 2}
: