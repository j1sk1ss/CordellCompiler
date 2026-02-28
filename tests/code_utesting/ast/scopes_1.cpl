{
    start() {
        i32 a;
        {
            i32 b;
        }
        i8 b;
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         { scope, id=3 }
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I8_VARIABLE_TOKEN, v_id=2, s_id=2)
: