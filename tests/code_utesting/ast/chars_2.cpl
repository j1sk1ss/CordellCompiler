function putc(i8 a) -> i0;
start() {
    putc('a');
    putc('\0');
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [putc] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I8_VARIABLE_TOKEN, v_id=0, s_id=2)
   [start] (START_TOKEN, v_id=1, s_id=1)
      { scope, id=3 }
         [putc] (CALL_TOKEN, v_id=0, s_id=0)
            { scope, id=0 }
               [97] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [putc] (CALL_TOKEN, v_id=0, s_id=0)
            { scope, id=0 }
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
/: