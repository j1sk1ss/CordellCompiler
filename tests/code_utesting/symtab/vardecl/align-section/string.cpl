{
    section(".test") {
        glob str a = "Hello world!";
    }
    section(".test2") {
        glob ptr i8 b = "Hello world!";
    }
}

: OUTPUT
{id: 1, b, ptr i8, s_id: 1, align: 8}
{id: 0, a, str, s_id: 1, align: 8}
{section='.test2', 1}
{section='.data'}
{section='.test', 0}
: