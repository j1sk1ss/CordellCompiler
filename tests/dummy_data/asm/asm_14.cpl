{
    function bubble(ptr u32 array, i32 asize) => i32 {
        i32 i = 0;
        i32 j = 0;

        while i < asize - 1; {
            while j < asize - i - 2; {
                if array[j] > array[j + 1]; {
                    i32 tmp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = tmp;
                }

                j += 1;
            }

            i += 1;
        }

        return 1;
    }

    start(i64 argc, ptr u64 argv) {
        arr array[10, i32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        bubble(array, 10);
        exit 0;
    }
}