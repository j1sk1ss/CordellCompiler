function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

function strcmp(ptr i8 a, ptr i8 b) -> i8 {
    while dref a && dref b; {
        if dref a != dref b; return 0;
        a += 1;
        b += 1;
    }
    
    return dref a == dref b;
}

function print(ptr i8 msg) -> i0 {
    syscall(0x2000004, 1, msg, strlen(msg));
}

start(i32 a, ptr ptr i8 answers) {
    print(ref "Who is better? (max/chloe)\n");
    print(ref "You have written: ");
    print(answers[1]);
    print(ref "\n");
    if not strcmp(answers[1], "max"); print(ref "Wrong!\n");
    else print(ref "Correct!\n");
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [strlen] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=1, s_id=-1)
            [s] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
         { scope, id=3 }
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
               [l] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3)
               [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [while] (WHILE_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [s] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=1, s_id=2)
               { scope, id=4 }
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [l] (I32_VARIABLE_TOKEN, v_id=1, t_id=3, s_id=3)
                     [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [s] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=1, s_id=2)
                     [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=0, s_id=-1)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [l] (I32_VARIABLE_TOKEN, v_id=1, t_id=3, s_id=3)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [strcmp] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=5 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=4, s_id=-1)
            [a] (I8_VARIABLE_TOKEN, ptr, v_id=2, t_id=0, s_id=5)
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=5, s_id=-1)
            [b] (I8_VARIABLE_TOKEN, ptr, v_id=3, t_id=0, s_id=5)
         { scope, id=6 }
            [while] (WHILE_TOKEN, v_id=-1, t_id=-1, s_id=6)
               [&&] (AND_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [a] (I8_VARIABLE_TOKEN, ptr, v_id=2, t_id=4, s_id=5)
                  [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [b] (I8_VARIABLE_TOKEN, ptr, v_id=3, t_id=5, s_id=5)
               { scope, id=7 }
                  [if] (IF_TOKEN, v_id=-1, t_id=-1, s_id=7)
                     [!=] (NCOMPARE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                           [a] (I8_VARIABLE_TOKEN, ptr, v_id=2, t_id=4, s_id=5)
                        [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                           [b] (I8_VARIABLE_TOKEN, ptr, v_id=3, t_id=5, s_id=5)
                     { scope, id=8 }
                        [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=8)
                           [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [a] (I8_VARIABLE_TOKEN, ptr, v_id=2, t_id=4, s_id=5)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [b] (I8_VARIABLE_TOKEN, ptr, v_id=3, t_id=5, s_id=5)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=6)
               [==] (COMPARE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [a] (I8_VARIABLE_TOKEN, ptr, v_id=2, t_id=4, s_id=5)
                  [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [b] (I8_VARIABLE_TOKEN, ptr, v_id=3, t_id=5, s_id=5)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [print] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=9 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=6, s_id=-1)
            [msg] (I8_VARIABLE_TOKEN, ptr, v_id=4, t_id=0, s_id=9)
         { scope, id=10 }
            [syscall] (SYSCALL_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [33554436] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [msg] (I8_VARIABLE_TOKEN, ptr, v_id=4, t_id=6, s_id=9)
               [()]
                  [strlen] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
                  { scope, id=-1 }
                     [msg] (I8_VARIABLE_TOKEN, ptr, v_id=4, t_id=6, s_id=9)
   [start] (START_TOKEN, v_id=3, t_id=-1, s_id=1)
      [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=7, s_id=-1)
         [a] (I32_VARIABLE_TOKEN, v_id=5, t_id=2, s_id=1)
      [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=9, s_id=-1)
         [answers] (I8_VARIABLE_TOKEN, ptr, v_id=6, t_id=-1, s_id=1)
      { scope, id=11 }
         [()]
            [print] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
            { scope, id=-1 }
               [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [Who is better? (max/chloe)
] (STRING_VALUE_TOKEN, v_id=0, t_id=-1, s_id=-1)
         [()]
            [print] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
            { scope, id=-1 }
               [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [You have written: ] (STRING_VALUE_TOKEN, v_id=1, t_id=-1, s_id=-1)
         [()]
            [print] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
            { scope, id=-1 }
               [[]]
                  [answers] (I8_VARIABLE_TOKEN, ptr, v_id=6, t_id=9, s_id=1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [()]
            [print] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
            { scope, id=-1 }
               [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [
] (STRING_VALUE_TOKEN, v_id=2, t_id=-1, s_id=-1)
         [if] (IF_TOKEN, v_id=-1, t_id=-1, s_id=11)
            [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [()]
                  [strcmp] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
                  { scope, id=-1 }
                     [[]]
                        [answers] (I8_VARIABLE_TOKEN, ptr, v_id=6, t_id=9, s_id=1)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [max] (STRING_VALUE_TOKEN, v_id=3, t_id=-1, s_id=-1)
            { scope, id=12 }
               [()]
                  [print] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
                  { scope, id=-1 }
                     [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [Wrong!
] (STRING_VALUE_TOKEN, v_id=4, t_id=-1, s_id=-1)
            { scope, id=11 }
               [()]
                  [print] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
                  { scope, id=-1 }
                     [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [Correct!
] (STRING_VALUE_TOKEN, v_id=5, t_id=-1, s_id=-1)
/: