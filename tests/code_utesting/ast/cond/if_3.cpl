{
    start() {
        if 1; exit 1;
        
        i32 a;
        if 2; a = 1;
        else a = 1;
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [if] (IF_TOKEN, v_id=0, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=3 }
               [exit] (EXIT_TOKEN, v_id=0, s_id=3)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         [if] (IF_TOKEN, v_id=0, s_id=2)
            [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=4 }
               [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
                  [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: