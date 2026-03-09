{
    start() {
        ((0x100 + 0x100) as ptr i0)(100);
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [()]
            [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
               [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
               [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                  [256] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [256] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
               [100] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: