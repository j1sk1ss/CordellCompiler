function foo(...) {
   @[poparg] i64 a;
   @[poparg] ptr f32 b;
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [...] (VAR_ARGUMENTS_TOKEN, v_id=-1, s_id=0)
         { scope, id=3 }
            [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
               [a] (I64_VARIABLE_TOKEN, v_id=0, s_id=3)
            [f32] (F32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
               [b] (F32_VARIABLE_TOKEN, ptr, v_id=1, s_id=3)
: