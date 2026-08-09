function foo() {
   function bar(i32 a = 10, i32 b = 10) {
      a += 1;
      b += 1;
   }
   bar();
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         { scope, id=3 }
            [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [bar] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=3)
               { scope, id=4 }
                  [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
                     [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=4)
                     [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                     [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=4)
                     [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  { scope, id=5 }
                     [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=4)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=2, s_id=4)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [()]
               [bar] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=3)
               { scope, id=-1 }
/: