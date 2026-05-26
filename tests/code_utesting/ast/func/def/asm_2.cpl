function foo() -> i32;
start() {
   asm (10 + 10, foo()) {
      "mov rax, %0",
      "mov rbx, %1",
      "syscall"
   }
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=3 }
         [asm] (ASM_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [()]
                  [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
                  { scope, id=-1 }
            { scope, id=-1 }
               [mov rax, %0] (STRING_VALUE_TOKEN, v_id=0, t_id=-1, s_id=-1)
               [mov rbx, %1] (STRING_VALUE_TOKEN, v_id=1, t_id=-1, s_id=-1)
               [syscall] (STRING_VALUE_TOKEN, v_id=2, t_id=-1, s_id=-1)
/: