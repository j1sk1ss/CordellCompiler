: RUN_ASM :

container stats {
    arr values[3, i32];

    @[self]
    function total(ptr stats self) -> i32;
}

function stats::total(ptr stats self) -> i32 {
    return self.values[0] + self.values[1] + self.values[2];
}

container report {
    stats data;
    i32 bonus;

    @[self]
    function score(ptr report self) -> i32;
}

function report::score(ptr report self) -> i32 {
    return self.data.total() + self.bonus;
}

start() {
    report r;

    r.data.values[0] = 5;
    r.data.values[1] = 8;
    r.data.values[2] = 13;
    r.bonus = 21;

    exit r.score() as u8;
}

:/ OUTPUT
@exit_code=47
/:
