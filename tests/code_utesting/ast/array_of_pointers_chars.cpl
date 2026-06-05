function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i8 a = 'M';
    i8 b = 'N';
    arr ps[2, ptr i8] = { ref a, ref b };
    putc(dref ps[0]);
    putc(dref ps[1]);
    exit 0;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [putc] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [c] (I8_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
         { scope, id=3 }
            [syscall] (SYSCALL_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [33554436] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [c] (I8_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=4 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [a] (I8_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=4)
            [77] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [b] (I8_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=4)
            [78] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [arr] (ARRAY_TYPE_TOKEN, v_id=-1, t_id=5, s_id=-1)
            [ps] (ARR_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=4)
            [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=4, s_id=-1)
            [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (I8_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=4)
            [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [b] (I8_VARIABLE_TOKEN, v_id=2, t_id=3, s_id=4)
         [()]
            [putc] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            { scope, id=-1 }
               [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [[]]
                     [ps] (ARR_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=4)
                     [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [()]
            [putc] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            { scope, id=-1 }
               [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [[]]
                     [ps] (ARR_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=4)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [exit] (EXIT_TOKEN, v_id=-1, t_id=-1, s_id=4)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: