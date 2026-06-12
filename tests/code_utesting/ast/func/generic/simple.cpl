function sum<T>(T a, T b) -> T {
   T c = 1;
   return a + b + c;
}

function sub<T>(T a, T b) -> T {
   T c = 2;
   return a - b - c;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [sum] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
      { scope, id=2 }
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [a] (GENERIC_VARIABLE_TOKEN, v_id=0, t_id=0, s_id=2)
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=2)
         { scope, id=3 }
            [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               [c] (GENERIC_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=3)
               [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [a] (GENERIC_VARIABLE_TOKEN, v_id=0, t_id=0, s_id=2)
                     [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=2)
                  [c] (GENERIC_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=3)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [sub] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
      { scope, id=4 }
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (GENERIC_VARIABLE_TOKEN, v_id=3, t_id=1, s_id=4)
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [b] (GENERIC_VARIABLE_TOKEN, v_id=4, t_id=1, s_id=4)
         { scope, id=5 }
            [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
               [c] (GENERIC_VARIABLE_TOKEN, v_id=5, t_id=1, s_id=5)
               [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=5)
               [-] (MINUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [-] (MINUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [a] (GENERIC_VARIABLE_TOKEN, v_id=3, t_id=1, s_id=4)
                     [b] (GENERIC_VARIABLE_TOKEN, v_id=4, t_id=1, s_id=4)
                  [c] (GENERIC_VARIABLE_TOKEN, v_id=5, t_id=1, s_id=5)
/: