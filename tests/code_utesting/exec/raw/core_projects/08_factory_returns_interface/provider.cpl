#include "value_source.inc"

container hidden_value08 implements value_source08 {
    i32 stored;

    @[override]
    function value(ptr hidden_value08 self) -> i32;
}

glob hidden_value08 INSTANCE08;

function hidden_value08::value(ptr hidden_value08 self) -> i32 {
    return self.stored;
}

function acquire08(i32 value) -> ptr value_source08 {
    INSTANCE08.stored = value;
    return ref INSTANCE08;
}
