{
    start() {
        i32 a;
        i32 b;
        i32 c;
        i32 d;
        a + b + c + d;
        a / b + (c + d);
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [d] (I32_VARIABLE_TOKEN, v_id=3, s_id=2)
         [+] (PLUS_TOKEN, v_id=-1, s_id=0)
            [+] (PLUS_TOKEN, v_id=-1, s_id=0)
               [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                  [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
                  [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=2)
               [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
            [d] (I32_VARIABLE_TOKEN, v_id=3, s_id=2)
         [+] (PLUS_TOKEN, v_id=-1, s_id=0)
            [/] (DIVIDE_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
               [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=2)
            [+] (PLUS_TOKEN, v_id=-1, s_id=0)
               [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
               [d] (I32_VARIABLE_TOKEN, v_id=3, s_id=2)
: