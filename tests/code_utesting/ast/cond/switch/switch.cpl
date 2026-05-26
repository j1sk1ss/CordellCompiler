start() {
   switch 1; {
      case 1; {
      }
      case 2; {
            break;
      }
      default {
      }
   }
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [switch] (SWITCH_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=3 }
               [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=4 }
                     [break] (BREAK_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [default] (DEFAULT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=5 }
/: