{
    extern function printf(ptr i8 fmt, ...);
    extern function sum(i32 a = 1, i32 b = 2) => i32;
    start() {
        sum();
        exit 1;
    }
}

: OUTPUT
{ scope, id=1 }
   [extern] (EXTERN_TOKEN, v_id=-1, s_id=0)
      [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
         [printf] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
         { scope, id=2 }
            [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
               [fmt] (I8_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
            [...] (VAR_ARGUMENTS_TOKEN, v_id=-1, s_id=0)
   [extern] (EXTERN_TOKEN, v_id=-1, s_id=0)
      [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
         [sum] (FUNC_NAME_TOKEN, v_id=1, s_id=0)
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
         { scope, id=3 }
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [b] (I32_VARIABLE_TOKEN, v_id=2, s_id=3)
               [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [start] (START_TOKEN, v_id=2, s_id=0)
      { scope, id=4 }
         [sum] (CALL_TOKEN, v_id=1, s_id=0)
            { scope, id=0 }
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [exit] (EXIT_TOKEN, v_id=0, s_id=4)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: