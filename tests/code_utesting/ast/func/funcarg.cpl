function a() -> i32;
function b(i32 g) -> i32;
function c(i32 f = b(a()));
start() {
   c();
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [a] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [b] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [g] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=3)
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [c] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
      { scope, id=4 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [f] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=4)
            [()]
               [b] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
               { scope, id=-1 }
                  [()]
                     [a] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
                     { scope, id=-1 }
   [start] (START_TOKEN, v_id=3, t_id=-1, s_id=1)
      { scope, id=5 }
         [()]
            [c] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
            { scope, id=-1 }
/: