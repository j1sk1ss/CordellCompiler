{
    function foo(...) {
        i64 a = poparg as i64;
        ptr f32 b = poparg as ptr f32;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [...] (VAR_ARGUMENTS_TOKEN, v_id=-1, s_id=0)
         { scope, id=3 }
            [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
               [a] (I64_VARIABLE_TOKEN, v_id=0, s_id=3)
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
                  [poparg] (, v_id=-1, s_id=0)
            [f32] (F32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
               [b] (F32_VARIABLE_TOKEN, ptr, v_id=1, s_id=3)
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [f32] (F32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
                  [poparg] (, v_id=-1, s_id=0)
: