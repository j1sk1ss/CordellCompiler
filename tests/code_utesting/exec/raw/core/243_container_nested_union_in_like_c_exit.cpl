: BLOCK_TEST :
: RUN_ASM :

@[union]
container small_view {
    u16 half;
    arr bytes[2, u8];
}

@[like_c]
container wrapped_view {
    i8 tag;
    small_view view;

    @[self]
    function total(ptr wrapped_view self) -> i32 {
        return self.tag as i32 + self.view.bytes[0] as i32 + self.view.bytes[1] as i32;
    }
}

start() {
    wrapped_view w;
    w.tag           = 2 as i8;
    w.view.half     = 0 as u16;
    w.view.bytes[0] = 19 as u8;
    w.view.bytes[1] = 21 as u8;
    exit w.total() as u8;
}

:/ OUTPUT
@exit_code=42
/:
