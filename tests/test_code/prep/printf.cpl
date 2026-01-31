{
    function _strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function _putc(i8 c) => i0 {
        syscall(0x2000004, 1, ref c, 1);
    }

    function _putn(i32 a) => i0 {
        while a > 0; {
            _putc(a % 10 + '0');
            a /= 10;
        }
    }

    function _puts(ptr i8 s) => i0 {
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

    start() {
        printf("Hello %s! Im %i yo!\n", "Max", 22);
        exit 0;
    }
}

: OUTPUT
line=1, type=14, data=[{], 
line=2, type=60, data=[function], 
line=2, type=61, data=[_strlen], 
line=2, type=12, data=[(], 
line=2, type=40, data=[i8], ptr 
line=2, type=101, data=[s], ptr 
line=2, type=13, data=[)], 
line=2, type=54, data=[=>], 
line=2, type=37, data=[i64], 
line=2, type=14, data=[{], 
line=3, type=37, data=[i64], 
line=3, type=101, data=[l], 
line=3, type=87, data=[=], 
line=3, type=5, data=[0], 
line=3, type=8, data=[;], 
line=4, type=66, data=[while], 
line=4, type=16, data=[dref], 
line=4, type=101, data=[s], ptr 
line=4, type=8, data=[;], 
line=4, type=14, data=[{], 
line=5, type=101, data=[s], ptr 
line=5, type=77, data=[+=], 
line=5, type=5, data=[1], 
line=5, type=8, data=[;], 
line=6, type=101, data=[l], 
line=6, type=77, data=[+=], 
line=6, type=5, data=[1], 
line=6, type=8, data=[;], 
line=7, type=15, data=[}], 
line=9, type=52, data=[return], 
line=9, type=101, data=[l], 
line=9, type=8, data=[;], 
line=10, type=15, data=[}], 
line=12, type=60, data=[function], 
line=12, type=61, data=[_putc], 
line=12, type=12, data=[(], 
line=12, type=40, data=[i8], 
line=12, type=101, data=[c], 
line=12, type=13, data=[)], 
line=12, type=54, data=[=>], 
line=12, type=34, data=[i0], 
line=12, type=14, data=[{], 
line=13, type=57, data=[syscall], 
line=13, type=12, data=[(], 
line=13, type=5, data=[33554436], 
line=13, type=9, data=[,], 
line=13, type=5, data=[1], 
line=13, type=9, data=[,], 
line=13, type=17, data=[ref], 
line=13, type=101, data=[c], 
line=13, type=9, data=[,], 
line=13, type=5, data=[1], 
line=13, type=13, data=[)], 
line=13, type=8, data=[;], 
line=14, type=15, data=[}], 
line=16, type=60, data=[function], 
line=16, type=61, data=[_putn], 
line=16, type=12, data=[(], 
line=16, type=38, data=[i32], 
line=16, type=101, data=[a], 
line=16, type=13, data=[)], 
line=16, type=54, data=[=>], 
line=16, type=34, data=[i0], 
line=16, type=14, data=[{], 
line=17, type=66, data=[while], 
line=17, type=101, data=[a], 
line=17, type=92, data=[>], 
line=17, type=5, data=[0], 
line=17, type=8, data=[;], 
line=17, type=14, data=[{], 
line=18, type=62, data=[_putc], 
line=18, type=12, data=[(], 
line=18, type=101, data=[a], 
line=18, type=76, data=[%], 
line=18, type=5, data=[10], 
line=18, type=72, data=[+], 
line=18, type=5, data=[48], 
line=18, type=13, data=[)], 
line=18, type=8, data=[;], 
line=19, type=101, data=[a], 
line=19, type=80, data=[/=], 
line=19, type=5, data=[10], 
line=19, type=8, data=[;], 
line=20, type=15, data=[}], 
line=21, type=15, data=[}], 
line=23, type=60, data=[function], 
line=23, type=61, data=[_puts], 
line=23, type=12, data=[(], 
line=23, type=40, data=[i8], ptr 
line=23, type=101, data=[s], ptr 
line=23, type=13, data=[)], 
line=23, type=54, data=[=>], 
line=23, type=34, data=[i0], 
line=23, type=14, data=[{], 
line=24, type=66, data=[while], 
line=24, type=16, data=[dref], 
line=24, type=101, data=[s], ptr 
line=24, type=8, data=[;], 
line=24, type=14, data=[{], 
line=25, type=62, data=[_putc], 
line=25, type=12, data=[(], 
line=25, type=16, data=[dref], 
line=25, type=101, data=[s], ptr 
line=25, type=13, data=[)], 
line=25, type=8, data=[;], 
line=26, type=101, data=[s], ptr 
line=26, type=77, data=[+=], 
line=26, type=5, data=[1], 
line=26, type=8, data=[;], 
line=27, type=15, data=[}], 
line=28, type=15, data=[}], 
line=30, type=60, data=[function], 
line=30, type=61, data=[printf], 
line=30, type=12, data=[(], 
line=30, type=40, data=[i8], ptr 
line=30, type=101, data=[fmt], ptr 
line=30, type=9, data=[,], 
line=30, type=22, data=[...], 
line=30, type=13, data=[)], 
line=30, type=14, data=[{], 
line=31, type=38, data=[i32], 
line=31, type=101, data=[index], 
line=31, type=87, data=[=], 
line=31, type=5, data=[0], 
line=31, type=8, data=[;], 
line=32, type=37, data=[i64], 
line=32, type=101, data=[fmt_len], 
line=32, type=87, data=[=], 
line=32, type=62, data=[_strlen], 
line=32, type=12, data=[(], 
line=32, type=101, data=[fmt], ptr 
line=32, type=13, data=[)], 
line=32, type=8, data=[;], 
line=33, type=66, data=[while], 
line=33, type=101, data=[index], 
line=33, type=90, data=[<], 
line=33, type=101, data=[fmt_len], 
line=33, type=8, data=[;], 
line=33, type=14, data=[{], 
line=34, type=63, data=[switch], 
line=34, type=101, data=[fmt], ptr 
line=34, type=10, data=[[], 
line=34, type=101, data=[index], 
line=34, type=11, data=[]], 
line=34, type=8, data=[;], 
line=34, type=14, data=[{], 
line=35, type=64, data=[case], 
line=35, type=5, data=[37], 
line=35, type=8, data=[;], 
line=35, type=14, data=[{], 
line=36, type=40, data=[i8], 
line=36, type=101, data=[mode], 
line=36, type=87, data=[=], 
line=36, type=101, data=[fmt], ptr 
line=36, type=10, data=[[], 
line=36, type=101, data=[index], 
line=36, type=77, data=[+=], 
line=36, type=5, data=[1], 
line=36, type=11, data=[]], 
line=36, type=8, data=[;], 
line=37, type=63, data=[switch], 
line=37, type=101, data=[mode], 
line=37, type=8, data=[;], 
line=37, type=14, data=[{], 
line=38, type=64, data=[case], 
line=38, type=5, data=[115], 
line=38, type=8, data=[;], 
line=38, type=14, data=[{], 
line=39, type=62, data=[_puts], 
line=39, type=12, data=[(], 
line=39, type=71, data=[poparg], 
line=39, type=47, data=[as], 
line=39, type=40, data=[i8], ptr 
line=39, type=13, data=[)], 
line=39, type=8, data=[;], 
line=40, type=68, data=[break], 
line=40, type=8, data=[;], 
line=41, type=15, data=[}], 
line=42, type=64, data=[case], 
line=42, type=5, data=[117], 
line=42, type=8, data=[;], 
line=42, type=14, data=[{], 
line=42, type=15, data=[}], 
line=43, type=64, data=[case], 
line=43, type=5, data=[105], 
line=43, type=8, data=[;], 
line=43, type=14, data=[{], 
line=43, type=15, data=[}], 
line=44, type=64, data=[case], 
line=44, type=5, data=[108], 
line=44, type=8, data=[;], 
line=44, type=14, data=[{], 
line=44, type=15, data=[}], 
line=45, type=64, data=[case], 
line=45, type=5, data=[100], 
line=45, type=8, data=[;], 
line=45, type=14, data=[{], 
line=46, type=62, data=[_putn], 
line=46, type=12, data=[(], 
line=46, type=71, data=[poparg], 
line=46, type=47, data=[as], 
line=46, type=40, data=[i8], ptr 
line=46, type=13, data=[)], 
line=46, type=8, data=[;], 
line=47, type=68, data=[break], 
line=47, type=8, data=[;], 
line=48, type=15, data=[}], 
line=49, type=15, data=[}], 
line=51, type=68, data=[break], 
line=51, type=8, data=[;], 
line=52, type=15, data=[}], 
line=53, type=65, data=[default], 
line=53, type=14, data=[{], 
line=54, type=62, data=[_putc], 
line=54, type=12, data=[(], 
line=54, type=101, data=[fmt], ptr 
line=54, type=10, data=[[], 
line=54, type=101, data=[index], 
line=54, type=11, data=[]], 
line=54, type=13, data=[)], 
line=54, type=8, data=[;], 
line=55, type=68, data=[break], 
line=55, type=8, data=[;], 
line=56, type=15, data=[}], 
line=57, type=15, data=[}], 
line=59, type=101, data=[index], 
line=59, type=77, data=[+=], 
line=59, type=5, data=[1], 
line=59, type=8, data=[;], 
line=60, type=15, data=[}], 
line=61, type=15, data=[}], 
line=63, type=51, data=[start], 
line=63, type=12, data=[(], 
line=63, type=13, data=[)], 
line=63, type=14, data=[{], 
line=64, type=62, data=[printf], 
line=64, type=12, data=[(], 
line=64, type=114, data=[Hello %s! Im %i yo!
], 
line=64, type=9, data=[,], 
line=64, type=114, data=[Max], 
line=64, type=9, data=[,], 
line=64, type=5, data=[22], 
line=64, type=13, data=[)], 
line=64, type=8, data=[;], 
line=65, type=53, data=[exit], 
line=65, type=5, data=[0], 
line=65, type=8, data=[;], 
line=66, type=15, data=[}], 
line=67, type=15, data=[}],
: