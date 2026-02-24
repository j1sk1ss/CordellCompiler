{
    extern function printf(ptr i8 fmt, ...);
    start(i64 argc, ptr u64 argv) {
        
        i32 num = 5;
        i32 factorio = 1;


        while num > 0; {
            factorio * num;
            num -= 1;
        }


        printf("Hello! %s\n", "String!");
        printf("%d", factorio);
        exit 0;
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
   [start] (START_TOKEN, v_id=1, s_id=0)
      [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
         [argc] (I64_VARIABLE_TOKEN, v_id=1, s_id=1)
      [u64] (U64_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
         [argv] (U64_VARIABLE_TOKEN, ptr, v_id=2, s_id=1)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [num] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
            [5] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [factorio] (I32_VARIABLE_TOKEN, v_id=4, s_id=3)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [while] (WHILE_TOKEN, v_id=0, s_id=3)
            [>] (LARGER_TOKEN, v_id=-1, s_id=0)
               [num] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=4 }
               [*] (MULTIPLY_TOKEN, v_id=-1, s_id=0)
                  [factorio] (I32_VARIABLE_TOKEN, v_id=4, s_id=3)
                  [num] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
               [-=] (SUBASSIGN_TOKEN, v_id=-1, s_id=0)
                  [num] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [printf] (CALL_TOKEN, v_id=0, s_id=0)
            { scope, id=0 }
               [Hello! %s
] (STRING_VALUE_TOKEN, v_id=0, s_id=0)
               [String!] (STRING_VALUE_TOKEN, v_id=1, s_id=0)
         [printf] (CALL_TOKEN, v_id=0, s_id=0)
            { scope, id=0 }
               [%d] (STRING_VALUE_TOKEN, v_id=2, s_id=0)
               [factorio] (I32_VARIABLE_TOKEN, v_id=4, s_id=3)
         [exit] (EXIT_TOKEN, v_id=0, s_id=3)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
: