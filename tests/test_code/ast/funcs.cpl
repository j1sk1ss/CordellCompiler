{
    function foo(i32 a) {
        a += 1;
        return;
    }

    function bar(i16 a) => i32 {
        a *= 1;
        return 1;
    }

    function baz(u64 a) => i0 {
        a /= 1;
        return;
    }

    function fang(i32 a) {
        a += 1;
        return;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         { scope, id=3 }
            [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [return] (RETURN_TOKEN, v_id=0, s_id=3)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=4 }
         [i16] (I16_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I16_VARIABLE_TOKEN, v_id=1, s_id=4)
         { scope, id=5 }
            [*=] (MULASSIGN_TOKEN, v_id=-1, s_id=0)
               [a] (I16_VARIABLE_TOKEN, v_id=1, s_id=4)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [return] (RETURN_TOKEN, v_id=0, s_id=5)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [baz] (FUNC_NAME_TOKEN, v_id=2, s_id=0)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=6 }
         [u64] (U64_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (U64_VARIABLE_TOKEN, v_id=2, s_id=6)
         { scope, id=7 }
            [/=] (DIVASSIGN_TOKEN, v_id=-1, s_id=0)
               [a] (U64_VARIABLE_TOKEN, v_id=2, s_id=6)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [return] (RETURN_TOKEN, v_id=0, s_id=7)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [fang] (FUNC_NAME_TOKEN, v_id=3, s_id=0)
      { scope, id=8 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=3, s_id=8)
         { scope, id=9 }
            [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=3, s_id=8)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [return] (RETURN_TOKEN, v_id=0, s_id=9)
: