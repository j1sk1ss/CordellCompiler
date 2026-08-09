: BLOCK_TEST :
: RUN_ASM :

@[align(1)]
container tiny_header {
    i8  tag;
    i16 code;
    i32 value;
}

container packet {
    tiny_header head;
    i8          tail;
}

start() {
    packet p;
    p.head.tag   = 2;
    p.head.code  = 10;
    p.head.value = 25;
    p.tail       = 5;
    exit (p.head.tag + p.head.code + p.head.value + p.tail) as u8;
}

:/ OUTPUT
@exit_code=42
/:
