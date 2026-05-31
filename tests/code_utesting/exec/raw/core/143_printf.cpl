: ASM_ARCH=x86_64_nasm_macho :
: RUN_ASM :
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        s += 1;
        l += 1;
    }

    return l;
}

@[inline(always)]
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putn(i32 a, i32 r) -> i0 {
    arr buffer[32, i8];
    i32 pos = 0;
    while a > 0; {
        buffer[pos] = ((a % r) + '0') as i8;
        a /= r;
        pos += 1;
    }

    pos -= 1;
    while pos >= 0; {
        putc(buffer[pos]);
        pos -= 1;
    }
}

function puts(ptr i8 s) -> i0 {
    while dref s; {
        putc(dref s);
        s += 1;
    }
}

function printf(ptr i8 fmt, ...) -> i0 {
    i32 index = 0;
    while index < strlen(fmt); {
        @[straight]
        @[no_fall]
        switch fmt[index]; {
            case '%'; {
                i8 mode = fmt[index + 1];
                @[straight] switch mode; {
                    case 's'; {
                        @[poparg] ptr i8 s;
                        puts(s);
                        break;
                    }
                    case 'u'; {}
                    case 'i'; {}
                    case 'l'; {}
                    case 'd'; {
                        @[poparg] i32 n;
                        putn(n, 10 as i32);
                        break;
                    }
                }

                index += 1;
            }
            default {
                putc(fmt[index]);
            }
        }

        index += 1;
    }
}

start() {
    printf(ref "Hello! %s, %i %i %i\n", ref "Oregon!", 10 as i32, 11 as i32, 12 as i32);
    exit 0;
}

:/ OUTPUT
Hello! Oregon!, 10 11 12
/: