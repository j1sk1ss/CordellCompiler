function foo<T>(T a) -> T {
}
function bar<U>(U a) {
}
function baz<T>() {
}

start() {
    foo<u32>(10);
    bar<i8>(1);
    baz<f64>();
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
      [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=1)
      { scope, id=4 }
         [U] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
            [a] (GENERIC_VARIABLE_TOKEN, v_id=1, s_id=4)
         { scope, id=5 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [baz] (FUNC_NAME_TOKEN, v_id=2, s_id=1)
      { scope, id=6 }
         { scope, id=7 }
   [start] (START_TOKEN, v_id=3, s_id=1)
      { scope, id=8 }
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=6, s_id=-1)
               [u32] (U32_TYPE_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [()]
            [bar] (FUNC_NAME_TOKEN, v_id=5, s_id=-1)
               [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [()]
            [baz] (FUNC_NAME_TOKEN, v_id=4, s_id=-1)
               [f64] (F64_TYPE_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [baz] (FUNC_NAME_TOKEN, v_id=4, s_id=1)
      { scope, id=6 }
         { scope, id=7 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [bar] (FUNC_NAME_TOKEN, v_id=5, s_id=1)
      { scope, id=4 }
         [U] (I8_TYPE_TOKEN, v_id=1, s_id=0)
            [a] (I8_VARIABLE_TOKEN, v_id=2, s_id=4)
         { scope, id=5 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=6, s_id=1)
         [T] (U32_TYPE_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [T] (U32_TYPE_TOKEN, v_id=0, s_id=0)
            [a] (U32_VARIABLE_TOKEN, v_id=3, s_id=2)
         { scope, id=3 }
/: