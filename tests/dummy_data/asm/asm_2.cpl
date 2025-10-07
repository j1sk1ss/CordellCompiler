{
    function clear_buffer(ptr u8 buffer, u32 size) => i32 {
        while size >= 0; {
            buffer[0] = 0;
            size -= 1;
        }

        return 1;
    }

    start(i64 argc, ptr u64 argv) {
        arr a[128, u8];
        clear_buffer(ref a, 128);
        exit 0;
    }
}