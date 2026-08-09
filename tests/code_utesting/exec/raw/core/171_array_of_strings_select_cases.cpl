: BLOCK_TEST :
: RUN_ASM[args="0"|args="1"|args="2"] :

function strlen(ptr i8 s) -> i64 {
    i64 n = 0;
    while dref s; {
        n += 1;
        s += 1;
    }

    return n;
}

start(i64 argc, ptr ptr i8 argv) {
    arr names[3, ptr i8] = { ref "cat", ref "kozyava", ref "ox" };
    i64 index = (argv[1][0] - '0') as i64;
    exit strlen(names[index]) as u8;
}

:/ OUTPUT
@case_index=0
@exit_code=3
---
@case_index=1
@exit_code=7
---
@case_index=2
@exit_code=2
---
/:
