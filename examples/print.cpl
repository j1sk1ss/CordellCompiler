{
    function _strlen(ptr i8 s) -> i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function _putc(i8 c) -> i0 {
        syscall(0x2000004, 1, ref c, 1);
    }

    function _putn(i32 a) -> i0 {
        while a > 0; {
            _putc(a % 10 + '0');
            a /= 10;
        }
    }

    function _puts(ptr i8 s) -> i0 {
        while dref s; {
            _putc(dref s);
            s += 1;
        }
    }

    function printf(ptr i8 fmt, ...) {
        i32 index = 0;
        i64 fmt_len = _strlen(fmt);
        while index < fmt_len; {
            switch fmt[index]; {
                case '%'; {
                    i8 mode = fmt[index += 1];
                    switch mode; {
                        case 's'; {
                            _puts(poparg as ptr i8);
                            break;
                        }
                        case 'u'; {}
                        case 'i'; {}
                        case 'l'; {}
                        case 'd'; {
                            _putn(poparg as ptr i8);
                            break;
                        }
                    }

                    break;
                }
                default {
                    _putc(fmt[index]);
                    break;
                }
            }

            index += 1;
        }
    }
}