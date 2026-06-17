: BLOCK_TEST :
: RUN_ASM :

container cell {
    i32 value;
}

function choose(ptr cell left, ptr cell right, i32 flag) -> ptr cell {
    if flag; return right;
    else     return left;
}

start() {
    cell a;
    cell b;
    a.value = 11;
    b.value = 42;

    ptr cell picked = choose(ref a, ref b, 1);
    exit picked.value as u8;
}

:/ OUTPUT
@exit_code=42
/:
