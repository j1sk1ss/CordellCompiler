{
    function foo();
    function foo(i32 a);
    function foo(i32 a, i32 b);
    function foo(f32 a);
    function foo(f32 a, i32 b);
    function foo(i8 a, i8 b, i8 c);
    function foo(u64 a);
    function foo(i64 a);

    function foo() {}
    function foo(i32 a) {}
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=1, s_id=1)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=3)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=2, s_id=1)
      { scope, id=4 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=4)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=2, s_id=4)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=3, s_id=1)
      { scope, id=5 }
         [f32] (F32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (F32_VARIABLE_TOKEN, v_id=3, s_id=5)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=4, s_id=1)
      { scope, id=6 }
         [f32] (F32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (F32_VARIABLE_TOKEN, v_id=4, s_id=6)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=5, s_id=6)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=5, s_id=1)
      { scope, id=7 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I8_VARIABLE_TOKEN, v_id=6, s_id=7)
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I8_VARIABLE_TOKEN, v_id=7, s_id=7)
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [c] (I8_VARIABLE_TOKEN, v_id=8, s_id=7)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=6, s_id=1)
      { scope, id=8 }
         [u64] (U64_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (U64_VARIABLE_TOKEN, v_id=9, s_id=8)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=7, s_id=1)
      { scope, id=9 }
         [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I64_VARIABLE_TOKEN, v_id=10, s_id=9)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=10 }
         { scope, id=11 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=1, s_id=1)
      { scope, id=12 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=11, s_id=12)
         { scope, id=13 }
: