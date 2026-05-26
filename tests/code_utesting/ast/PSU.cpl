extern function printf(ptr i8 fmt, ...) -> i32;

@[entry("_main")]
@[naked]
@[section(".text")]
function main(i32 argc, ptr ptr i8 argv) -> i8 {
    @[counter(10)] loop {
        printf("Hello from loop which iterates 10 times!\n");
    }

    @[no_fall]
    @[straight]
    switch argc; {
        case 1; {
            printf("The first argument is %s\n", argv[0]);
        }
        default {
            printf("Too many arguments! %i != 1\n", argc);
        }
    }

    @[cold] if argc == 2; {
        printf("Cold section!\n");
    }
    else printf("Hot section!\n");
    exit 0;
}

:/ OUTPUT
{ scope, id=1 }
   [extern] (EXTERN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1, ext, glob)
         [printf] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         { scope, id=2 }
            [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
               [fmt] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            [...] (VAR_ARGUMENTS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [main] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [argc] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3)
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
            [argv] (I8_VARIABLE_TOKEN, ptr, v_id=2, t_id=-1, s_id=3)
         { scope, id=4 }
            [loop] (LOOP_TOKEN, v_id=-1, t_id=-1, s_id=4)
               { scope, id=5 }
                  [()]
                     [printf] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1, ext)
                     { scope, id=-1 }
                        [Hello from loop which iterates 10 times!
] (STRING_VALUE_TOKEN, v_id=0, t_id=-1, s_id=-1)
            [switch] (SWITCH_TOKEN, v_id=-1, t_id=-1, s_id=4)
               [argc] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3)
               { scope, id=-1 }
                  [case] (CASE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     { scope, id=6 }
                        [()]
                           [printf] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1, ext)
                           { scope, id=-1 }
                              [The first argument is %s
] (STRING_VALUE_TOKEN, v_id=1, t_id=-1, s_id=-1)
                              [[]]
                                 [argv] (I8_VARIABLE_TOKEN, ptr, v_id=2, t_id=-1, s_id=3)
                                 [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [default] (DEFAULT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     { scope, id=7 }
                        [()]
                           [printf] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1, ext)
                           { scope, id=-1 }
                              [Too many arguments! %i != 1
] (STRING_VALUE_TOKEN, v_id=2, t_id=-1, s_id=-1)
                              [argc] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3)
            [if] (IF_TOKEN, v_id=-1, t_id=-1, s_id=4)
               [==] (COMPARE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [argc] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3)
                  [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               { scope, id=8 }
                  [()]
                     [printf] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1, ext)
                     { scope, id=-1 }
                        [Cold section!
] (STRING_VALUE_TOKEN, v_id=3, t_id=-1, s_id=-1)
               { scope, id=4 }
                  [()]
                     [printf] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1, ext)
                     { scope, id=-1 }
                        [Hot section!
] (STRING_VALUE_TOKEN, v_id=4, t_id=-1, s_id=-1)
            [exit] (EXIT_TOKEN, v_id=-1, t_id=-1, s_id=4)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: