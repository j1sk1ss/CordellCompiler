start() {
    syscall(0x2000004, 1, 1, 12);
    syscall(0x2000004, 1, 1 as ptr i0, 12 as u64);
}

:/ OUTPUT
{
    start {
        {
            {
                syscall, argc: args(i32n 33554436,i8n 1,i8n 1,i8n 12,);
                i0t* %0 = i8n 1 as *ptr;
                use i0t* %0;
                u64t %1 = i8n 12 as u64;
                use u64t %1;
                syscall, argc: args(i32n 33554436,i8n 1,i0t* %0,u64t %1,);
            }
        }
    }
}
/: