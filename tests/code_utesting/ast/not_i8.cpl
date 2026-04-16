function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

glob arr tape[4, i8];

start(i32 argc, ptr ptr i8 argv) {
    if not tape[0]; putdigit(1);
    else putdigit(0);

    tape[0] = 1 as i8;

    if not tape[0]; putdigit(1);
    else putdigit(0);

    exit 0;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [putc] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [c] (I8_VARIABLE_TOKEN, v_id=0, s_id=2)
         { scope, id=3 }
            [syscall] (SYSCALL_TOKEN, v_id=-1, s_id=0)
               [33554436] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [ref] (REF_TYPE_TOKEN, v_id=-1, s_id=0)
                  [c] (I8_VARIABLE_TOKEN, v_id=0, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [putdigit] (FUNC_NAME_TOKEN, v_id=1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=4 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [x] (I32_VARIABLE_TOKEN, v_id=1, s_id=4)
         { scope, id=5 }
            [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
               [c] (I8_VARIABLE_TOKEN, v_id=2, s_id=5)
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
                  [x] (I32_VARIABLE_TOKEN, v_id=1, s_id=4)
            [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
               [c] (I8_VARIABLE_TOKEN, v_id=2, s_id=5)
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
                  [48] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [putc] (CALL_TOKEN, v_id=0, s_id=0)
               { scope, id=0 }
                  [c] (I8_VARIABLE_TOKEN, v_id=2, s_id=5)
   [arr] (ARRAY_TYPE_TOKEN, v_id=-1, s_id=0, glob)
      [tape] (ARR_VARIABLE_TOKEN, v_id=3, s_id=1, glob)
      [4] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
      [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
   [start] (START_TOKEN, v_id=2, s_id=1)
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
         [argc] (I32_VARIABLE_TOKEN, v_id=4, s_id=1)
      [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
         [argv] (I8_VARIABLE_TOKEN, ptr, v_id=5, s_id=1)
      { scope, id=6 }
         [if] (IF_TOKEN, v_id=-1, s_id=6)
            [not] (NEGATIVE_TOKEN, v_id=-1, s_id=0)
               [[]]
                  [tape] (ARR_VARIABLE_TOKEN, v_id=3, s_id=1, glob)
                  [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=7 }
               [putdigit] (CALL_TOKEN, v_id=1, s_id=0)
                  { scope, id=0 }
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=6 }
               [putdigit] (CALL_TOKEN, v_id=1, s_id=0)
                  { scope, id=0 }
                     [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [[]]
               [tape] (ARR_VARIABLE_TOKEN, v_id=3, s_id=1, glob)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
               [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [if] (IF_TOKEN, v_id=-1, s_id=6)
            [not] (NEGATIVE_TOKEN, v_id=-1, s_id=0)
               [[]]
                  [tape] (ARR_VARIABLE_TOKEN, v_id=3, s_id=1, glob)
                  [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=8 }
               [putdigit] (CALL_TOKEN, v_id=1, s_id=0)
                  { scope, id=0 }
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=6 }
               [putdigit] (CALL_TOKEN, v_id=1, s_id=0)
                  { scope, id=0 }
                     [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [exit] (EXIT_TOKEN, v_id=-1, s_id=6)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
/: