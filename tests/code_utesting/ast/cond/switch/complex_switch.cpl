start() {
   i32 a;
   switch a; {
      case 1; {}
      case 2; {}
      case 3; {} case 4; {}
      default {}
   }

   switch a; {}
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
         [switch] (SWITCH_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            { scope, id=-1 }
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=3 }
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=4 }
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [3] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=5 }
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [4] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=6 }
               [default] (DEFAULT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=7 }
         [switch] (SWITCH_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            { scope, id=-1 }
/: