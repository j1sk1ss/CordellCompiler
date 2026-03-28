function name() -> i32 {
    @[cold] if 1; {
    }
    else {
    }

    @[no_fall]
    @[straight]
    switch 1; {
        @[cold] case 1; {}
        @[cold] case 2; {}
        default {}
    }

    return 0;
}

:/ OUTPUT
@[cold] base_token=if
@[straight] @[no_fall] base_token=switch
@[cold] base_token=case
@[cold] base_token=case
/:
