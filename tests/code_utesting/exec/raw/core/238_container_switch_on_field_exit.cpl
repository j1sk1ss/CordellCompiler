: BLOCK_TEST :
: RUN_ASM :

container tagged_value {
    i32 tag;
    i32 value;
}

start() {
    tagged_value v;
    v.tag = 2;
    v.value = 40;

    i32 out = 0;
    @[no_fall]
    switch v.tag; {
        case 1; { out = v.value + 1; }
        case 2; { out = v.value + 2; }
        default { out = 99; }
    }

    exit out as u8;
}

:/ OUTPUT
@exit_code=42
/:
