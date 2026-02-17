{
    function foo() => i32;
    start() {
        syscall(0, foo(), 0 + 0, 0);
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
   [start] (START_TOKEN, v_id=1, s_id=0)
      { scope, id=3 }
         [syscall] (SYSCALL_TOKEN, v_id=-1, s_id=0)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [foo] (CALL_TOKEN, v_id=0, s_id=0)
               { scope, id=0 }
            [+] (PLUS_TOKEN, v_id=-1, s_id=0)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: