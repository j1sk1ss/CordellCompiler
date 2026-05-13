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
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [T] (GENERIC_TYPE_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [T] (GENERIC_TYPE_TOKEN, v_id=0, s_id=0)
            [a] (GENERIC_VARIABLE_TOKEN, v_id=0, s_id=2)
         { scope, id=3 }
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=3)
                  [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
               { scope, id=4 }
                  [T] (GENERIC_TYPE_TOKEN, v_id=0, s_id=0)
                     [b] (GENERIC_VARIABLE_TOKEN, v_id=1, s_id=4)
                  [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
                     [c] (GENERIC_VARIABLE_TOKEN, v_id=2, s_id=4)
                  { scope, id=5 }
                     [function] (FUNC_TOKEN, v_id=-1, s_id=0)
                        [baz] (FUNC_NAME_TOKEN, v_id=2, s_id=5)
                           [K] (GENERIC_TYPE_TOKEN, v_id=2, s_id=0)
                        { scope, id=6 }
                           [T] (GENERIC_TYPE_TOKEN, v_id=0, s_id=0)
                              [d] (GENERIC_VARIABLE_TOKEN, v_id=3, s_id=6)
                           [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
                              [k] (GENERIC_VARIABLE_TOKEN, v_id=4, s_id=6)
                           [K] (GENERIC_TYPE_TOKEN, v_id=2, s_id=0)
                              [l] (GENERIC_VARIABLE_TOKEN, v_id=5, s_id=6)
                           { scope, id=7 }
                              [return] (RETURN_TOKEN, v_id=-1, s_id=7)
                                 [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                     [()]
                        [baz] (FUNC_NAME_TOKEN, v_id=2, s_id=5)
                           [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
                        { scope, id=0 }
                           [b] (GENERIC_VARIABLE_TOKEN, v_id=1, s_id=4)
                           [c] (GENERIC_VARIABLE_TOKEN, v_id=2, s_id=4)
                           [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [()]
               [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=3)
                  [T] (GENERIC_TYPE_TOKEN, v_id=0, s_id=0)
               { scope, id=0 }
                  [a] (GENERIC_VARIABLE_TOKEN, v_id=0, s_id=2)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [start] (START_TOKEN, v_id=3, s_id=1)
      { scope, id=8 }
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=4, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=4, s_id=1)
         [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=11, s_id=2)
         { scope, id=3 }
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [bar] (FUNC_NAME_TOKEN, v_id=6, s_id=3)
                  [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
               { scope, id=4 }
                  [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
                     [b] (I32_VARIABLE_TOKEN, v_id=10, s_id=4)
                  [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
                     [c] (GENERIC_VARIABLE_TOKEN, v_id=9, s_id=4)
                  { scope, id=5 }
                     [function] (FUNC_TOKEN, v_id=-1, s_id=0)
                        [baz] (FUNC_NAME_TOKEN, v_id=5, s_id=5)
                           [K] (GENERIC_TYPE_TOKEN, v_id=2, s_id=0)
                        { scope, id=6 }
                           [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
                              [d] (I32_VARIABLE_TOKEN, v_id=8, s_id=6)
                           [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
                              [k] (GENERIC_VARIABLE_TOKEN, v_id=7, s_id=6)
                           [K] (GENERIC_TYPE_TOKEN, v_id=2, s_id=0)
                              [l] (GENERIC_VARIABLE_TOKEN, v_id=6, s_id=6)
                           { scope, id=7 }
                              [return] (RETURN_TOKEN, v_id=-1, s_id=7)
                                 [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                     [()]
                        [baz] (FUNC_NAME_TOKEN, v_id=5, s_id=5)
                           [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
                        { scope, id=0 }
                           [b] (I32_VARIABLE_TOKEN, v_id=10, s_id=4)
                           [c] (GENERIC_VARIABLE_TOKEN, v_id=9, s_id=4)
                           [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [()]
               [bar] (FUNC_NAME_TOKEN, v_id=8, s_id=-1)
                  [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
               { scope, id=0 }
                  [a] (I32_VARIABLE_TOKEN, v_id=11, s_id=2)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [bar] (FUNC_NAME_TOKEN, v_id=8, s_id=3)
         [U] (I32_TYPE_TOKEN, v_id=1, s_id=0)
      { scope, id=4 }
         [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=16, s_id=4)
         [U] (I32_TYPE_TOKEN, v_id=1, s_id=0)
            [c] (I32_VARIABLE_TOKEN, v_id=15, s_id=4)
         { scope, id=5 }
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [baz] (FUNC_NAME_TOKEN, v_id=9, s_id=5)
                  [K] (GENERIC_TYPE_TOKEN, v_id=2, s_id=0)
               { scope, id=6 }
                  [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
                     [d] (I32_VARIABLE_TOKEN, v_id=14, s_id=6)
                  [U] (I32_TYPE_TOKEN, v_id=1, s_id=0)
                     [k] (I32_VARIABLE_TOKEN, v_id=13, s_id=6)
                  [K] (GENERIC_TYPE_TOKEN, v_id=2, s_id=0)
                     [l] (GENERIC_VARIABLE_TOKEN, v_id=12, s_id=6)
                  { scope, id=7 }
                     [return] (RETURN_TOKEN, v_id=-1, s_id=7)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [()]
               [baz] (FUNC_NAME_TOKEN, v_id=10, s_id=-1)
                  [U] (I32_TYPE_TOKEN, v_id=1, s_id=0)
               { scope, id=0 }
                  [b] (I32_VARIABLE_TOKEN, v_id=16, s_id=4)
                  [c] (I32_VARIABLE_TOKEN, v_id=15, s_id=4)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [baz] (FUNC_NAME_TOKEN, v_id=10, s_id=5)
         [K] (I32_TYPE_TOKEN, v_id=2, s_id=0)
      { scope, id=6 }
         [T] (I32_TYPE_TOKEN, v_id=0, s_id=0)
            [d] (I32_VARIABLE_TOKEN, v_id=19, s_id=6)
         [U] (I32_TYPE_TOKEN, v_id=1, s_id=0)
            [k] (I32_VARIABLE_TOKEN, v_id=18, s_id=6)
         [K] (I32_TYPE_TOKEN, v_id=2, s_id=0)
            [l] (I32_VARIABLE_TOKEN, v_id=17, s_id=6)
         { scope, id=7 }
            [return] (RETURN_TOKEN, v_id=-1, s_id=7)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
/: