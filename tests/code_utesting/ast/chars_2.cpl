function putc(i8 a) -> i0;
start() {
    putc('a');
    putc('\0');
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [putc] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [a] (I8_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=3 }
         [()]
            [putc] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            { scope, id=-1 }
               [97] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [()]
            [putc] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            { scope, id=-1 }
               [48] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: