function foo<T>(T a) -> T {
    function bar<U>(T b, U c) -> U {
        function baz<K>(T d, U k, K l) -> K {
            return 1;
        }
        baz<U>(b, c, 1);
    }
    bar<T>(a, 1);
}
start() {
    foo<i32>(1);
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
            [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [bar] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=3)
                  [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
               { scope, id=4 }
                  [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                     [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=4)
                  [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                     [c] (GENERIC_VARIABLE_TOKEN, v_id=2, t_id=1, s_id=4)
                  { scope, id=5 }
                     [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [baz] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=5)
                           [K] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                        { scope, id=6 }
                           [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                              [d] (GENERIC_VARIABLE_TOKEN, v_id=3, t_id=0, s_id=6)
                           [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                              [k] (GENERIC_VARIABLE_TOKEN, v_id=4, t_id=1, s_id=6)
                           [K] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                              [l] (GENERIC_VARIABLE_TOKEN, v_id=5, t_id=2, s_id=6)
                           { scope, id=7 }
                              [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=7)
                                 [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [()]
                        [baz] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=5)
                           [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                        { scope, id=-1 }
                           [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=4)
                           [c] (GENERIC_VARIABLE_TOKEN, v_id=2, t_id=1, s_id=4)
                           [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [()]
               [bar] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=3)
                  [T] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               { scope, id=-1 }
                  [a] (GENERIC_VARIABLE_TOKEN, v_id=0, t_id=0, s_id=2)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [start] (START_TOKEN, v_id=3, t_id=-1, s_id=1)
      { scope, id=8 }
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=4, t_id=-1, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=4, t_id=-1, s_id=1)
         [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
      { scope, id=2 }
         [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=11, t_id=0, s_id=2)
         { scope, id=3 }
            [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [bar] (FUNC_NAME_TOKEN, v_id=6, t_id=-1, s_id=3)
                  [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
               { scope, id=4 }
                  [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                     [b] (I32_VARIABLE_TOKEN, v_id=10, t_id=0, s_id=4)
                  [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                     [c] (GENERIC_VARIABLE_TOKEN, v_id=9, t_id=1, s_id=4)
                  { scope, id=5 }
                     [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [baz] (FUNC_NAME_TOKEN, v_id=5, t_id=-1, s_id=5)
                           [K] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                        { scope, id=6 }
                           [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                              [d] (I32_VARIABLE_TOKEN, v_id=8, t_id=0, s_id=6)
                           [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                              [k] (GENERIC_VARIABLE_TOKEN, v_id=7, t_id=1, s_id=6)
                           [K] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                              [l] (GENERIC_VARIABLE_TOKEN, v_id=6, t_id=2, s_id=6)
                           { scope, id=7 }
                              [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=7)
                                 [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [()]
                        [baz] (FUNC_NAME_TOKEN, v_id=5, t_id=-1, s_id=5)
                           [U] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                        { scope, id=-1 }
                           [b] (I32_VARIABLE_TOKEN, v_id=10, t_id=0, s_id=4)
                           [c] (GENERIC_VARIABLE_TOKEN, v_id=9, t_id=1, s_id=4)
                           [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [()]
               [bar] (FUNC_NAME_TOKEN, v_id=7, t_id=-1, s_id=-1)
                  [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               { scope, id=-1 }
                  [a] (I32_VARIABLE_TOKEN, v_id=11, t_id=0, s_id=2)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [bar] (FUNC_NAME_TOKEN, v_id=7, t_id=-1, s_id=3)
         [U] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
      { scope, id=4 }
         [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=16, t_id=0, s_id=4)
         [U] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [c] (I32_VARIABLE_TOKEN, v_id=15, t_id=1, s_id=4)
         { scope, id=5 }
            [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [baz] (FUNC_NAME_TOKEN, v_id=8, t_id=-1, s_id=5)
                  [K] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
               { scope, id=6 }
                  [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                     [d] (I32_VARIABLE_TOKEN, v_id=14, t_id=0, s_id=6)
                  [U] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                     [k] (I32_VARIABLE_TOKEN, v_id=13, t_id=1, s_id=6)
                  [K] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                     [l] (GENERIC_VARIABLE_TOKEN, v_id=12, t_id=2, s_id=6)
                  { scope, id=7 }
                     [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=7)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [()]
               [baz] (FUNC_NAME_TOKEN, v_id=9, t_id=-1, s_id=-1)
                  [U] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
               { scope, id=-1 }
                  [b] (I32_VARIABLE_TOKEN, v_id=16, t_id=0, s_id=4)
                  [c] (I32_VARIABLE_TOKEN, v_id=15, t_id=1, s_id=4)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [baz] (FUNC_NAME_TOKEN, v_id=9, t_id=-1, s_id=5)
         [K] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
      { scope, id=6 }
         [T] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [d] (I32_VARIABLE_TOKEN, v_id=19, t_id=0, s_id=6)
         [U] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [k] (I32_VARIABLE_TOKEN, v_id=18, t_id=1, s_id=6)
         [K] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [l] (I32_VARIABLE_TOKEN, v_id=17, t_id=2, s_id=6)
         { scope, id=7 }
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=7)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: