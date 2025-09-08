{
    glob arr _mm_head[100000, i8] =;
    glob arr _blocks_info[100000, i32] =;
    glob long _head = 0;

    glob function memset(ptr u8 buffer, i32 val, i64 size) {
        i64 index = 0;
        while index < size; {
            buffer[index] = val;
            index = index + 1;
        }

        return 1;
    }

    glob function malloc(i64 size) {
        if size > 0; {
            ptr u8 curr_mem = _mm_head;
            i32 block_index = 0;
            while block_index < 100000; {
                if _blocks_info[block_index] == 0; {
                    _blocks_info[block_index] = 1;
                    _blocks_info[block_index + 1] = size;
                    _blocks_info[block_index + 2] = curr_mem;
                    return curr_mem;
                }

                curr_mem = curr_mem + _blocks_info[block_index + 1];
                block_index = block_index + 3;
            }
        }

        return -1;
    }

    glob function free(ptr i32 mem) {
        i32 block_index = 0;
        while block_index < 100000; {
            if _blocks_info[block_index + 2] == mem; {
                _blocks_info[block_index] = 0;
                return 1;
            }

            block_index = block_index + 3;
        }

        return 1;
    }
}