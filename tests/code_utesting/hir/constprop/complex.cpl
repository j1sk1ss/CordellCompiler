start() {
    i32 a = 10 as i32;
    i32 b = 1 as i32;
    if b; {
        a = 12 as i32;
    }
    else {
        a = 12 as i32;
    }

    i32 c = a;
}

:/ OUTPUT
id: 2, c, i32, s_id: 2
id: 9, a, i32, s_id: 2, value=12
id: 12, a, i32, s_id: 2, value=12
id: 1, b, i32, s_id: 2
id: 4, tmp, i32, s_id: -1, value=1
id: 3, tmp, i32, s_id: -1, value=10
id: 7, a, i32, s_id: 2, value=10
id: 11, c, i32, s_id: 2, value=12
id: 10, a, i32, s_id: 2, value=12
id: 0, a, i32, s_id: 2
id: 5, tmp, i32, s_id: -1, value=12
id: 6, tmp, i32, s_id: -1, value=12
id: 8, b, i32, s_id: 2, value=1
/: