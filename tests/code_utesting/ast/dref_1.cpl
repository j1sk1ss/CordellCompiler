{
    start() {
        i32 a;
        ptr i32 b = ref a;
        dref b = 1;
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, ptr, v_id=1, s_id=2)
            [ref] (REF_TYPE_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
               [b] (I32_VARIABLE_TOKEN, ptr, v_id=1, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: