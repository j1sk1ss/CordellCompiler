#ifndef CODE_EXT_01_METRIC_INC_
#define CODE_EXT_01_METRIC_INC_ 0

interface metric01 {
    @[self] @[abstract]
    function value(ptr metric01 self) -> i32;
}

container fixed_metric01::metric01 {
    i32 base;

    @[override]
    function value(ptr fixed_metric01 self) -> i32;
}

function evaluate01(ptr metric01 item, i32 bias) -> i32;

#endif
