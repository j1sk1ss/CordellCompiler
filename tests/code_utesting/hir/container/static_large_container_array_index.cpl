container scene {
    ptr i8 title;
    ptr i8 clue;
    i32    focus;
    i32    empathy;
    i32    danger;
    i8     solved;
}

start() {
    arr scenes[7, scene];
    scenes[6].title = ref "three";
}

:/ OUTPUT
{
    fn _main()
    {
        {
            arrs %6 = arr_alloc(csi16 336);
            i8t* %7 = &(three);
            i8t* %8 = &(arrs %6);
            u64t %9 = i8n 6 * csu8 48;
            i8t* %11 = u64t %9 as *ptr;
            i8t* %10 = i8t* %8 + i8t* %11;
            u8t** %12 = i8t* %10 + csu8 0;
            i8t** %13 = u8t** %12 as *ptr;
            *(i8t** %13) = i8t* %7;
        }
    }
}
/: