: BLOCK_TEST :
: RUN_ASM[args="x"|args="abcd"] :

function strlen(ptr i8 s) -> i64 {
    i64 n = 0;
    while dref s; {
        n += 1;
        s += 1;
    }

    return n;
}

@[entry("_main")]
function main(i64 argc, ptr ptr i8 argv) -> i0 {
    exit strlen(argv[1]) as u8;
}

:/ OUTPUT
@case_index=0
@exit_code=1
---
@case_index=1
@exit_code=4
---
/:
