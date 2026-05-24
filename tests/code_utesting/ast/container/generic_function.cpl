container math {
    @[static]
    function sum<T>(T a, T b) -> T {
        return a + b;
    }
}

start() {
    math m;
    i32 a = m.sum<i32>(1, 1);
}

:/ OUTPUT
{ scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [math] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=1 }
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=4 }
         [math] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [m] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=4)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=3, t_id=-1, s_id=4)
            [()]
               [sum] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=-1)
                  [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               { scope, id=-1 }
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=2, s_id=-1)
      [sum] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
      { scope, id=2 }
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (GENERIC_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
         [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=1, s_id=2)
         { scope, id=3 }
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [a] (GENERIC_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
                  [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=1, s_id=2)
   [function] (FUNC_TOKEN, v_id=-1, t_id=2, s_id=-1)
      [sum] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
         [T] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
      { scope, id=2 }
         [T] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=5, t_id=1, s_id=2)
         [T] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=4, t_id=1, s_id=2)
         { scope, id=3 }
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [a] (I32_VARIABLE_TOKEN, v_id=5, t_id=1, s_id=2)
                  [b] (I32_VARIABLE_TOKEN, v_id=4, t_id=1, s_id=2)
/: