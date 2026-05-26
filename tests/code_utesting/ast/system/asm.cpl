start() {
    asm() {
        "mov rax, 1",
        "mov rbx, 1",
        "syscall"
    }
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [asm] (ASM_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
            { scope, id=-1 }
               [mov rax, 1] (STRING_VALUE_TOKEN, v_id=0, t_id=-1, s_id=-1)
               [mov rbx, 1] (STRING_VALUE_TOKEN, v_id=1, t_id=-1, s_id=-1)
               [syscall] (STRING_VALUE_TOKEN, v_id=2, t_id=-1, s_id=-1)
/: