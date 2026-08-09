function foo<T>(T a) -> T {
    T b = 10 as T;
    return b;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
      { scope, id=2 }
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [a] (GENERIC_VARIABLE_TOKEN, v_id=0, t_id=0, s_id=2)
         { scope, id=3 }
            [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=3)
               [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                     [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=3)
/: