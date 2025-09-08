{
    glob function strlen(ptr i8 string) {
        i64 index = 0;
        while dref string; {
            index = index + 1;
            string = string + 1;
        }

        return index;
    }

    glob function reverse(ptr i8 string, i32 size) {
        i32 l = 0;
        i32 r = size;
        while l < r; {
            if string[r]; {
                char tmp = string[l];
                string[l] = string[r];
                string[r] = tmp;
                l = l + 1;
            }

            r = r - 1;
        }

        return 1;
    }

    glob function atoi(ptr i8 buffer, i32 bsize) {
        i32 num = 0;
        bsize = bsize - 1;

        while bsize > 0; {
            num = num * 10;
            char sym = buffer[bsize];
            num = num + sym - 48;
        }

        return num;
    }

    glob function itoa(ptr i8 buffer, i64 buffer_size, i32 num) {
        i32 index = 0;
        i32 is_negative = 0;
        if num == 0; {
            buffer[0] = '0';
            buffer[1] = 0;
            return 1;
        }

        if num < 0; {
            is_negative = 1;
            num = num * -1;
        }

        i32 tmp = 0;
        while (num > 0) && (index < buffer_size); {
            tmp = num % 10;
            buffer[index] = tmp + 48;
            index = index + 1;
            num = num / 10;
        }

        if is_negative; {
            buffer[0] = 45;
        }

        return index;
    }
}