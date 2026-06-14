container std {
    function putc(i8 c) {
        return syscall(1, 1, ref c, 1);
    }

    function gets(ptr i8 buffer, i64 size) {
        return syscall(0, 0, buffer, size);
    }

    function open(ptr i8 path, i32 flags, i32 mode) {
        return syscall(2, path, flags, mode);
    }

    function fwrite(i32 fd, ptr u8 buffer, i32 size) {
        return syscall(1, fd, buffer, size);
    }

    function fread(i32 fd, ptr u8 buffer, i32 size) {
        return syscall(0, fd, buffer, size);
    }

    function close(i32 fd) {
        return syscall(3, fd);
    }
}