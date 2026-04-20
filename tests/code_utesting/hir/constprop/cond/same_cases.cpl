start() {
    i32 a = 1 as i32;
    @[no_fall]
    @[straight]
    switch 1; {
        case 1; { a = 1 as i32; }
        case 2; { a = 1 as i32; }
        default { a = 1 as i32; }
    }
    i32 b = a;
}

:/ OUTPUT
id: 17, a, i32, s_id: 2, value=1
id: 9, tmp, i8, s_id: -1, value=0
id: 4, tmp, i32, s_id: -1, value=1
id: 13, a, i32, s_id: 2, value=1
id: 11, tmp, i64, s_id: -1, value=1
id: 16, a, i32, s_id: 2, value=1
id: 14, a, i32, s_id: 2, value=1
id: 5, tmp, i32, s_id: -1, value=1
id: 2, tmp, i32, s_id: -1, value=1
id: 12, a, i32, s_id: 2, value=1
id: 15, b, i32, s_id: 2, value=1
id: 6, tmp, i8, s_id: -1, value=1
id: 8, tmp, i64, s_id: -1, value=1
id: 1, b, i32, s_id: 2
id: 3, tmp, i32, s_id: -1, value=1
id: 7, tmp, i64, s_id: -1, value=1
id: 10, tmp, i64, s_id: -1, value=2
id: 0, a, i32, s_id: 2
/: