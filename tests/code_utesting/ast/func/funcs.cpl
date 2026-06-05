function foo(i32 a) {
   a += 1;
}

function bar(i16 a) -> i32 {
   a *= 1;
   return 1;
}

function baz(u64 a) -> i0 {
   a /= 1;
}

function fang(i32 a) {
   a += 1;
}

start() {
   foo(1);
   bar(1);
   baz(1);
   fang(1);
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
         { scope, id=3 }
            [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [bar] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
      { scope, id=4 }
         [i16] (I16_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [a] (I16_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=4)
         { scope, id=5 }
            [*=] (MULASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (I16_VARIABLE_TOKEN, v_id=1, t_id=3, s_id=4)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=5)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [baz] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=6 }
         [u64] (U64_TYPE_TOKEN, v_id=-1, t_id=5, s_id=-1)
            [a] (U64_VARIABLE_TOKEN, v_id=2, t_id=-1, s_id=6)
         { scope, id=7 }
            [/=] (DIVASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (U64_VARIABLE_TOKEN, v_id=2, t_id=5, s_id=6)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [fang] (FUNC_NAME_TOKEN, v_id=3, t_id=-1, s_id=1)
      { scope, id=8 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=6, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=3, t_id=0, s_id=8)
         { scope, id=9 }
            [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (I32_VARIABLE_TOKEN, v_id=3, t_id=6, s_id=8)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [start] (START_TOKEN, v_id=4, t_id=-1, s_id=1)
      { scope, id=10 }
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            { scope, id=-1 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [()]
            [bar] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
            { scope, id=-1 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [()]
            [baz] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
            { scope, id=-1 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [()]
            [fang] (FUNC_NAME_TOKEN, v_id=3, t_id=-1, s_id=1)
            { scope, id=-1 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: