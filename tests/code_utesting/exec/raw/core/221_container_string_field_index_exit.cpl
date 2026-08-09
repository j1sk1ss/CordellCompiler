: BLOCK_TEST :
: RUN_ASM :

container label {
    ptr i8 text;

    @[self]
    function second(ptr label self) -> i8 {
        return self.text[1];
    }
}

start() {
    label l;
    l.text = ref "A*Z";

    exit l.second() as u8;
}

:/ OUTPUT
@exit_code=42
/:
