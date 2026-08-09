: BLOCK_TEST :
: RUN_ASM :

#define LIS_SCENE_COUNT   4
#define LIS_ACTION_COUNT  5
#define LIS_START_REWINDS 3

glob arr storm_seed[LIS_SCENE_COUNT, i32] = { 1, 2, 2, 3 };

container scene {
    ptr i8 title;
    ptr i8 clue;
    i32 focus;
    i32 empathy;
    i32 danger;

    @[self]
    function init(ptr scene self, ptr i8 title, ptr i8 clue, i32 focus, i32 empathy, i32 danger) -> i0;
}

@[like_c]
container timeline {
    i32 focus;
    i32 empathy;
    i32 danger;
    i32 photos;
    i32 rewinds;
    i32 scene;
}

@[align(1)]
@[union]
container omen_code {
    u16 code;
    arr bytes[2, u8];
}

container memory_note {
    ptr memory_note next;
    ptr i8 text;
    i32 weight;
}

container lis_game {
    arr scenes[LIS_SCENE_COUNT, scene];
    arr scene_done[LIS_SCENE_COUNT, i8];
    arr actions[LIS_ACTION_COUNT, ptr i0];
    timeline now;
    timeline saved;
    memory_note photo_note;
    memory_note friend_note;
    memory_note storm_note;
    ptr memory_note journal;
    ptr i8 mood;
    i8 running;
    i8 won;

    @[self] function new(ptr lis_game self) -> i0;
    @[self] function save(ptr lis_game self) -> i0;
    @[self] function score(ptr lis_game self) -> i32;
    @[self] function journal_weight(ptr lis_game self) -> i32;
    @[self] function scene_stat_sum(ptr lis_game self) -> i32;
}

function act_photo(ptr lis_game self) -> i0;
function act_talk(ptr lis_game self) -> i0;
function act_search(ptr lis_game self) -> i0;
function act_wait(ptr lis_game self) -> i0;
function act_rewind(ptr lis_game self) -> i0;

@[inline(model)]
function scene::init(ptr scene self, ptr i8 title, ptr i8 clue, i32 focus, i32 empathy, i32 danger) -> i0 {
    self.title = title;
    self.clue = clue;
    self.focus = focus;
    self.empathy = empathy;
    self.danger = danger;
}

@[inline(never)]
function copy_timeline(ptr timeline dst, ptr timeline src) -> i0 {
    dst.focus = src.focus;
    dst.empathy = src.empathy;
    dst.danger = src.danger;
    dst.photos = src.photos;
    dst.rewinds = src.rewinds;
    dst.scene = src.scene;
}

@[inline(model)]
function omen_sum() -> i32 {
    omen_code omen;
    omen.code = 0 as u16;
    omen.bytes[0] = 4 as u8;
    omen.bytes[1] = 2 as u8;
    return ((omen.code & 0xFF) + ((omen.code >> 8) & 0xFF)) as i32;
}

@[inline(model)]
function lis_game::new(ptr lis_game self) -> i0 {
    self.scenes[0].init(ref "bathroom", ref "cabinet", 2 as i32, 0 as i32, storm_seed[0]);
    self.scenes[1].init(ref "parking", ref "keychain", 0 as i32, 2 as i32, storm_seed[1]);
    self.scenes[2].init(ref "dorm", ref "poster", 1 as i32, 1 as i32, storm_seed[2]);
    self.scenes[3].init(ref "lighthouse", ref "photo", 2 as i32, 1 as i32, storm_seed[3]);

    self.scene_done[0] = 0;
    self.scene_done[1] = 0;
    self.scene_done[2] = 0;
    self.scene_done[3] = 0;

    self.actions[0] = act_photo;
    self.actions[1] = act_talk;
    self.actions[2] = act_search;
    self.actions[3] = act_wait;
    self.actions[4] = act_rewind;

    self.now.focus = 1;
    self.now.empathy = 1;
    self.now.danger = 0;
    self.now.photos = 0;
    self.now.rewinds = LIS_START_REWINDS;
    self.now.scene = 0;
    self.save();

    self.photo_note.text = ref "photo";
    self.photo_note.weight = 2;
    self.photo_note.next = ref self.friend_note;

    self.friend_note.text = ref "friend";
    self.friend_note.weight = 3;
    self.friend_note.next = ref self.storm_note;

    self.storm_note.text = ref "storm";
    self.storm_note.weight = omen_sum();
    self.storm_note.next = 0 as ptr memory_note;

    self.journal = ref self.photo_note;
    self.mood = ref "uncertain";
    self.running = 1;
    self.won = 0;
}

function lis_game::save(ptr lis_game self) -> i0 {
    copy_timeline(ref self.saved, ref self.now);
}

function lis_game::score(ptr lis_game self) -> i32 {
    return self.now.focus + self.now.empathy + self.now.photos * 2 - self.now.danger;
}

function lis_game::journal_weight(ptr lis_game self) -> i32 {
    ptr memory_note note = self.journal;
    i32 total = 0;

    while note; {
        total += note.weight;
        note = note.next;
    }

    return total;
}

function lis_game::scene_stat_sum(ptr lis_game self) -> i32 {
    return self.scenes[0].focus
        + self.scenes[1].empathy
        + self.scenes[2].danger
        + self.scenes[3].danger
        + self.scenes[2].empathy;
}

function act_photo(ptr lis_game self) -> i0 { self.now.photos += 1; }
function act_talk(ptr lis_game self) -> i0 { self.now.empathy += 1; }
function act_search(ptr lis_game self) -> i0 { self.now.focus += 1; }
function act_wait(ptr lis_game self) -> i0 { self.now.danger += 1; }
function act_rewind(ptr lis_game self) -> i0 { self.now.rewinds -= 1; }

start() {
    lis_game game;
    game.new();

    exit (
        game.score()
        + game.journal_weight()
        + game.scene_stat_sum()
        + game.now.rewinds
        + game.saved.rewinds
        + game.running as i32
    ) as u8;
}

:/ OUTPUT
@exit_code=30
/:
