function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function foo() -> i0 {
    putc('G');
}

function bar() -> i0 {
    function foo() -> i0 {
        putc('L');
    }
    foo();
}

start() {
    bar();
    foo();
    exit 0;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [putc] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [c] (I8_VARIABLE_TOKEN, v_id=0, s_id=2)
         { scope, id=3 }
            [syscall] (SYSCALL_TOKEN, v_id=-1, s_id=0)
               [33554436] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [ref] (REF_TYPE_TOKEN, v_id=-1, s_id=0)
                  [c] (I8_VARIABLE_TOKEN, v_id=0, s_id=2)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=4 }
         { scope, id=5 }
            [putc] (CALL_TOKEN, v_id=0, s_id=1)
               { scope, id=0 }
                  [71] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [bar] (FUNC_NAME_TOKEN, v_id=2, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=6 }
         { scope, id=7 }
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [foo] (FUNC_NAME_TOKEN, v_id=3, s_id=7)
                  [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
               { scope, id=8 }
                  { scope, id=9 }
                     [putc] (CALL_TOKEN, v_id=0, s_id=1)
                        { scope, id=0 }
                           [76] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [foo] (CALL_TOKEN, v_id=3, s_id=7)
               { scope, id=0 }
   [start] (START_TOKEN, v_id=4, s_id=1)
      { scope, id=10 }
         [bar] (CALL_TOKEN, v_id=2, s_id=1)
            { scope, id=0 }
         [foo] (CALL_TOKEN, v_id=1, s_id=1)
            { scope, id=0 }
         [exit] (EXIT_TOKEN, v_id=-1, s_id=10)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
/: