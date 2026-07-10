: BLOCK_TEST :
: RUN_ASM :

container timeline {
    i32 focus;
    i32 danger;
}

container tiny_game {
    arr actions[2, ptr i0];
    timeline now;

    @[self]
    function init(ptr tiny_game self) -> i0;

    @[self]
    function step(ptr tiny_game self, i32 input) -> i0;

    @[self]
    function score(ptr tiny_game self) -> i32;
}

function act_photo(ptr tiny_game self) -> i0;
function act_wait(ptr tiny_game self) -> i0;

function tiny_game::init(ptr tiny_game self) -> i0 {
    self.actions[0] = act_photo;
    self.actions[1] = act_wait;
    self.now.focus = 10;
    self.now.danger = 1;
}

function tiny_game::step(ptr tiny_game self, i32 input) -> i0 {
    @[no_fall]
    @[straight]
    switch input; {
        case 'p'; { self.actions[0](self); }
        case 'w'; { self.actions[1](self); }
    }
}

function tiny_game::score(ptr tiny_game self) -> i32 {
    return self.now.focus - self.now.danger;
}

function act_photo(ptr tiny_game self) -> i0 {
    self.now.focus += 30;
}

function act_wait(ptr tiny_game self) -> i0 {
    self.now.danger += 5;
}

start() {
    tiny_game game;
    game.init();
    game.step(112 as i32);
    game.step(119 as i32);
    exit game.score() as u8;
}

:/ OUTPUT
@exit_code=34
/:
