extern function printf(ptr i8 fmt, ...) -> i32;
extern ptr i8 BUFFER;
start() {
    printf(BUFFER, 10);
}

:/ OUTPUT
{
    (fun) extern csu8 0;
    (var) extern csu8 1;
    start {
        {
            {
                use i8g* %1;
                excall , argc printf0(i8* fmt, ...) -> i32args(i8g* %1,i8n 10,);
            }
        }
    }
}
/: