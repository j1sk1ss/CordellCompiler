{
    start() {
        ptr i32 a;
        a[((10 as i32) + 10) as i8];
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
         [[]]
            [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
            [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
               [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
               [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                  [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                     [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
                     [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: