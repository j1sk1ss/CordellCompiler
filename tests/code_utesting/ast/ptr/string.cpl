start() {
   ptr i8 msg = ref "Hello, World!";
   arr ans[0, i8] = "Hello, User!";

   msg[0] = 'o';
   ans[1] = 'g';
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
            [msg] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [Hello, World!] (STRING_VALUE_TOKEN, v_id=0, t_id=-1, s_id=-1)
         [str] (STR_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [ans] (STR_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
            [Hello, User!] (STRING_VALUE_TOKEN, v_id=1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [[]]
               [msg] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [111] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [[]]
               [ans] (STR_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [103] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: