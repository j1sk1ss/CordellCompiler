start() {
    i8 flags = 0x1F;
    flags &= neg 0x04;
    flags ^= 0x02;
    exit flags as u8;
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [flags] (I8_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [31] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [&=] (BITANDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [flags] (I8_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
            [neg] (NEGATIVE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [4] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [^=] (BITXORASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [flags] (I8_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
            [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [exit] (EXIT_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [u8] (U8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [flags] (I8_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
/: