: BLOCK_TEST :
: RUN_ASM :

#define SCENE_COUNT 4

glob arr seed[SCENE_COUNT, i32] = { 1, 2, 2, 3 };

container scene {
    i32 focus;
    i32 empathy;
    i32 danger;

    @[self]
    function init(ptr scene self, i32 focus, i32 empathy, i32 danger) -> i0;
}

@[inline(model)]
function scene::init(ptr scene self, i32 focus, i32 empathy, i32 danger) -> i0 {
    self.focus = focus;
    self.empathy = empathy;
    self.danger = danger;
}

container scene_book {
    arr scenes[SCENE_COUNT, scene];

    @[self]
    function init(ptr scene_book self) -> i0 {
        self.scenes[0].init(2 as i32, 0 as i32, seed[0]);
        self.scenes[1].init(0 as i32, 2 as i32, seed[1]);
        self.scenes[2].init(1 as i32, 1 as i32, seed[2]);
        self.scenes[3].init(2 as i32, 1 as i32, seed[3]);
    }

    @[self]
    function total(ptr scene_book self) -> i32 {
        return self.scenes[0].focus
            + self.scenes[1].empathy
            + self.scenes[2].danger
            + self.scenes[3].danger;
    }
}

start() {
    scene_book book;
    book.init();
    exit book.total() as u8;
}

:/ OUTPUT
@exit_code=9
/:
