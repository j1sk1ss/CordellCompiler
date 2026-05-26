start() {
   syscall(syscall(syscall(syscall(1, 2), 1), 1, 1, 1, 1), 1);
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [syscall] (SYSCALL_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [syscall] (SYSCALL_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [syscall] (SYSCALL_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [syscall] (SYSCALL_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/: