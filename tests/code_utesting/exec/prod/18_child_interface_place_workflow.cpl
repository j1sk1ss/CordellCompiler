: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface reader18 {
    @[self] @[abstract]
    function value(ptr reader18 self) -> i32;
}

interface writer18 implements reader18 {
    @[self] @[abstract]
    function add(ptr writer18 self, i32 delta) -> i0;
}

container state18 implements writer18 {
    i32 value_field;

    @[override] function value(ptr state18 self) -> i32;
    @[override] function add(ptr state18 self, i32 delta) -> i0;
}

function state18::value(ptr state18 self) -> i32 { return self.value_field; }
function state18::add(ptr state18 self, i32 delta) -> i0 { self.value_field += delta; }

function run18(ptr writer18 state) -> i32 {
    i32 i = 0;

    while i < 7; {
        state.add(3 as i32);
        i += 1;
    }

    return state.value();
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr state18 state = place(ref storage, state18);
    state.value_field = 21;

    exit run18(state) as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:
