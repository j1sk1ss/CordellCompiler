: RUN_ASM :

function twice<T>(T value) -> T {
    return value + value;
}

start() {
    exit twice<u8>(17);
}

:/ OUTPUT
@exit_code=34
/:
