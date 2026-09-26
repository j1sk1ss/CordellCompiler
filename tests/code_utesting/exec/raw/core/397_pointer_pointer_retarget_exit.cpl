: BLOCK_TEST :
: RUN_ASM :

function retarget397(ptr ptr i32 slot, ptr i32 value) -> i0 {
    dref slot = value;
}

start() {
    i32 a = 5;
    i32 b = 42;
    ptr i32 selected = ref a;
    retarget397(ref selected, ref b);
    exit (dref selected) as u8;
}

:/ OUTPUT
@exit_code=42
/:
