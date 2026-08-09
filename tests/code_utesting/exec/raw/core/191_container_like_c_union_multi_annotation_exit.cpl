: BLOCK_TEST :
: RUN_ASM :

@[like_c]
@[union]
container device_word {
    u64 qword;
    arr bytes[8, u8];
}

start() {
    device_word d;
    d.qword = 0;
    d.bytes[0] = 40;
    d.bytes[1] = 2;

    exit ((d.qword & 0xFF) + ((d.qword >> 8) & 0xFF)) as u8;
}

:/ OUTPUT
@exit_code=42
/:
