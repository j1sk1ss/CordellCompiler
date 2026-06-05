function foo() {
   @[naked]
   @[naked] i32 b;
   @[naked]
   @[naked] glob i32 a;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         { scope, id=3 }
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
               [b] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=3)
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1, glob)
               [a] (I32_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=3, glob)
/: