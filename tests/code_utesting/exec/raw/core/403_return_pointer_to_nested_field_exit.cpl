: BLOCK_TEST :
: RUN_ASM :

container inner403 { i32 value; }
container outer403 { inner403 inner; }

function field403(ptr outer403 item) -> ptr i32 {
    return ref item.inner.value;
}

start() {
    outer403 item;
    item.inner.value = 1;
    ptr i32 p = field403(ref item);
    dref p += 41;
    exit item.inner.value as u8;
}

:/ OUTPUT
@exit_code=42
/:
