: BLOCK_TEST :
: RUN_ASM[args="0"|args="1"] :

function strlen(ptr i8 s) -> i32 {
    i32 n = 0;
    while dref s; {
        n += 1;
        s += 1;
    }

    return n;
}

container dictionary {
    arr words[2, ptr i8];

    @[self]
    function len(ptr dictionary self, i32 index) -> i32 {
        return strlen(self.words[index]);
    }
}

start(i64 argc, ptr ptr i8 argv) {
    dictionary d;
    d.words[0] = ref "cat";
    d.words[1] = ref "kozyava";

    i32 index = (argv[1][0] - '0') as i32;
    exit d.len(index) as u8;
}

:/ OUTPUT
@case_index=0
@exit_code=3
---
@case_index=1
@exit_code=7
---
/:
