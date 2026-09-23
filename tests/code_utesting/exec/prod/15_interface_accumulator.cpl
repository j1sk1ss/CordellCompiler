: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface scorer15 {
    @[self] @[abstract]
    function score(ptr scorer15 self, i32 value) -> i32;
}

container adder15 implements scorer15 {
    i32 bias;

    @[override]
    function score(ptr adder15 self, i32 value) -> i32;
}

function adder15::score(ptr adder15 self, i32 value) -> i32 {
    return value + self.bias;
}

function run15(ptr scorer15 scorer) -> i32 {
    i32 total = 0;
    i32 i = 0;

    while i < 6; {
        total += scorer.score(i);
        i += 1;
    }

    return total;
}

start() {
    adder15 scorer;
    scorer.bias = 2;

    exit run15(ref scorer) as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=27
/:
