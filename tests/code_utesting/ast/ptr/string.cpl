{
    start() {
        ptr i8 msg = "Hello, World!";
        str ans = "Hello, User!";

        msg[0] = 'o';
        ans[1] = 'g';
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [msg] (I8_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
            [Hello, World!] (STRING_VALUE_TOKEN, v_id=0, s_id=0)
         [str] (STR_TYPE_TOKEN, v_id=-1, s_id=0)
            [ans] (STR_VARIABLE_TOKEN, v_id=1, s_id=2)
            [Hello, User!] (STRING_VALUE_TOKEN, v_id=1, s_id=0)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [[]]
               [msg] (I8_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [111] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [[]]
               [ans] (STR_VARIABLE_TOKEN, v_id=1, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [103] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: