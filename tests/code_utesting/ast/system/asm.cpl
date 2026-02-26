{
    start() {
        asm() {
            "mov rax, 1",
            "mov rbx, 1",
            "syscall"
        }
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [asm] (ASM_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
            { scope, id=0 }
               [mov rax, 1] (STRING_VALUE_TOKEN, v_id=0, s_id=0)
               [mov rbx, 1] (STRING_VALUE_TOKEN, v_id=1, s_id=0)
               [syscall] (STRING_VALUE_TOKEN, v_id=2, s_id=0)
: