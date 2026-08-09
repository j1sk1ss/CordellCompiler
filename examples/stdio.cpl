container std {
    function putc(i8 c) -> i64 {
        return syscall(1, 1, ref c, 1) as i64;
    }

    function gets(ptr i8 buffer, i64 size) -> i64 {
        return syscall(0, 0, buffer, size) as i64;
    }

    function open(ptr i8 path, i32 flags, i32 mode) -> i64 {
        return syscall(2, path, flags, mode) as i64;
    }

    function fwrite(i32 fd, ptr u8 buffer, i32 size) -> i64 {
        return syscall(1, fd, buffer, size) as i64;
    }

    function fread(i32 fd, ptr u8 buffer, i32 size) -> i64 {
        return syscall(0, fd, buffer, size) as i64;
    }

    function close(i32 fd) -> i8 {
        return syscall(3, fd) as i8;
    }
}