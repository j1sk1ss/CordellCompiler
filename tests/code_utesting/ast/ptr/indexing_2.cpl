{
    start() {
        i32 addr;
        i8 a = (addr as ptr i32)[0];
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [addr] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I8_VARIABLE_TOKEN, v_id=1, s_id=2)
            [[]]
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
                  [addr] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: