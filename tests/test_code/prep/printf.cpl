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
line=1, type=13, data=[{], 
line=2, type=59, data=[function], 
line=2, type=60, data=[_strlen], 
line=2, type=11, data=[(], 
line=2, type=39, data=[i8], ptr 
line=2, type=100, data=[s], ptr 
line=2, type=12, data=[)], 
line=2, type=53, data=[=>], 
line=2, type=36, data=[i64], 
line=2, type=13, data=[{], 
line=3, type=36, data=[i64], 
line=3, type=100, data=[l], 
line=3, type=86, data=[=], 
line=3, type=5, data=[0], 
line=3, type=7, data=[;], 
line=4, type=65, data=[while], 
line=4, type=15, data=[dref], 
line=4, type=100, data=[s], ptr 
line=4, type=7, data=[;], 
line=4, type=13, data=[{], 
line=5, type=100, data=[s], ptr 
line=5, type=76, data=[+=], 
line=5, type=5, data=[1], 
line=5, type=7, data=[;], 
line=6, type=100, data=[l], 
line=6, type=76, data=[+=], 
line=6, type=5, data=[1], 
line=6, type=7, data=[;], 
line=7, type=14, data=[}], 
line=9, type=51, data=[return], 
line=9, type=100, data=[l], 
line=9, type=7, data=[;], 
line=10, type=14, data=[}], 
line=12, type=59, data=[function], 
line=12, type=60, data=[_putc], 
line=12, type=11, data=[(], 
line=12, type=39, data=[i8], 
line=12, type=100, data=[c], 
line=12, type=12, data=[)], 
line=12, type=53, data=[=>], 
line=12, type=33, data=[i0], 
line=12, type=13, data=[{], 
line=13, type=56, data=[syscall], 
line=13, type=11, data=[(], 
line=13, type=5, data=[33554436], 
line=13, type=8, data=[,], 
line=13, type=5, data=[1], 
line=13, type=8, data=[,], 
line=13, type=16, data=[ref], 
line=13, type=100, data=[c], 
line=13, type=8, data=[,], 
line=13, type=5, data=[1], 
line=13, type=12, data=[)], 
line=13, type=7, data=[;], 
line=14, type=14, data=[}], 
line=16, type=59, data=[function], 
line=16, type=60, data=[_putn], 
line=16, type=11, data=[(], 
line=16, type=37, data=[i32], 
line=16, type=100, data=[a], 
line=16, type=12, data=[)], 
line=16, type=53, data=[=>], 
line=16, type=33, data=[i0], 
line=16, type=13, data=[{], 
line=17, type=65, data=[while], 
line=17, type=100, data=[a], 
line=17, type=91, data=[>], 
line=17, type=5, data=[0], 
line=17, type=7, data=[;], 
line=17, type=13, data=[{], 
line=18, type=61, data=[_putc], 
line=18, type=11, data=[(], 
line=18, type=100, data=[a], 
line=18, type=75, data=[%], 
line=18, type=5, data=[10], 
line=18, type=71, data=[+], 
line=18, type=5, data=[48], 
line=18, type=12, data=[)], 
line=18, type=7, data=[;], 
line=19, type=100, data=[a], 
line=19, type=79, data=[/=], 
line=19, type=5, data=[10], 
line=19, type=7, data=[;], 
line=20, type=14, data=[}], 
line=21, type=14, data=[}], 
line=23, type=59, data=[function], 
line=23, type=60, data=[_puts], 
line=23, type=11, data=[(], 
line=23, type=39, data=[i8], ptr 
line=23, type=100, data=[s], ptr 
line=23, type=12, data=[)], 
line=23, type=53, data=[=>], 
line=23, type=33, data=[i0], 
line=23, type=13, data=[{], 
line=24, type=65, data=[while], 
line=24, type=15, data=[dref], 
line=24, type=100, data=[s], ptr 
line=24, type=7, data=[;], 
line=24, type=13, data=[{], 
line=25, type=61, data=[_putc], 
line=25, type=11, data=[(], 
line=25, type=15, data=[dref], 
line=25, type=100, data=[s], ptr 
line=25, type=12, data=[)], 
line=25, type=7, data=[;], 
line=26, type=100, data=[s], ptr 
line=26, type=76, data=[+=], 
line=26, type=5, data=[1], 
line=26, type=7, data=[;], 
line=27, type=14, data=[}], 
line=28, type=14, data=[}], 
line=30, type=59, data=[function], 
line=30, type=60, data=[printf], 
line=30, type=11, data=[(], 
line=30, type=39, data=[i8], ptr 
line=30, type=100, data=[fmt], ptr 
line=30, type=8, data=[,], 
line=30, type=21, data=[...], 
line=30, type=12, data=[)], 
line=30, type=13, data=[{], 
line=31, type=37, data=[i32], 
line=31, type=100, data=[index], 
line=31, type=86, data=[=], 
line=31, type=5, data=[0], 
line=31, type=7, data=[;], 
line=32, type=36, data=[i64], 
line=32, type=100, data=[fmt_len], 
line=32, type=86, data=[=], 
line=32, type=61, data=[_strlen], 
line=32, type=11, data=[(], 
line=32, type=100, data=[fmt], ptr 
line=32, type=12, data=[)], 
line=32, type=7, data=[;], 
line=33, type=65, data=[while], 
line=33, type=100, data=[index], 
line=33, type=89, data=[<], 
line=33, type=100, data=[fmt_len], 
line=33, type=7, data=[;], 
line=33, type=13, data=[{], 
line=34, type=62, data=[switch], 
line=34, type=100, data=[fmt], ptr 
line=34, type=9, data=[[], 
line=34, type=100, data=[index], 
line=34, type=10, data=[]], 
line=34, type=7, data=[;], 
line=34, type=13, data=[{], 
line=35, type=63, data=[case], 
line=35, type=5, data=[37], 
line=35, type=7, data=[;], 
line=35, type=13, data=[{], 
line=36, type=39, data=[i8], 
line=36, type=100, data=[mode], 
line=36, type=86, data=[=], 
line=36, type=100, data=[fmt], ptr 
line=36, type=9, data=[[], 
line=36, type=100, data=[index], 
line=36, type=76, data=[+=], 
line=36, type=5, data=[1], 
line=36, type=10, data=[]], 
line=36, type=7, data=[;], 
line=37, type=62, data=[switch], 
line=37, type=100, data=[mode], 
line=37, type=7, data=[;], 
line=37, type=13, data=[{], 
line=38, type=63, data=[case], 
line=38, type=5, data=[115], 
line=38, type=7, data=[;], 
line=38, type=13, data=[{], 
line=39, type=61, data=[_puts], 
line=39, type=11, data=[(], 
line=39, type=70, data=[poparg], 
line=39, type=46, data=[as], 
line=39, type=39, data=[i8], ptr 
line=39, type=12, data=[)], 
line=39, type=7, data=[;], 
line=40, type=67, data=[break], 
line=40, type=7, data=[;], 
line=41, type=14, data=[}], 
line=42, type=63, data=[case], 
line=42, type=5, data=[117], 
line=42, type=7, data=[;], 
line=42, type=13, data=[{], 
line=42, type=14, data=[}], 
line=43, type=63, data=[case], 
line=43, type=5, data=[105], 
line=43, type=7, data=[;], 
line=43, type=13, data=[{], 
line=43, type=14, data=[}], 
line=44, type=63, data=[case], 
line=44, type=5, data=[108], 
line=44, type=7, data=[;], 
line=44, type=13, data=[{], 
line=44, type=14, data=[}], 
line=45, type=63, data=[case], 
line=45, type=5, data=[100], 
line=45, type=7, data=[;], 
line=45, type=13, data=[{], 
line=46, type=61, data=[_putn], 
line=46, type=11, data=[(], 
line=46, type=70, data=[poparg], 
line=46, type=46, data=[as], 
line=46, type=39, data=[i8], ptr 
line=46, type=12, data=[)], 
line=46, type=7, data=[;], 
line=47, type=67, data=[break], 
line=47, type=7, data=[;], 
line=48, type=14, data=[}], 
line=49, type=14, data=[}], 
line=51, type=67, data=[break], 
line=51, type=7, data=[;], 
line=52, type=14, data=[}], 
line=53, type=64, data=[default], 
line=53, type=13, data=[{], 
line=54, type=61, data=[_putc], 
line=54, type=11, data=[(], 
line=54, type=100, data=[fmt], ptr 
line=54, type=9, data=[[], 
line=54, type=100, data=[index], 
line=54, type=10, data=[]], 
line=54, type=12, data=[)], 
line=54, type=7, data=[;], 
line=55, type=67, data=[break], 
line=55, type=7, data=[;], 
line=56, type=14, data=[}], 
line=57, type=14, data=[}], 
line=59, type=100, data=[index], 
line=59, type=76, data=[+=], 
line=59, type=5, data=[1], 
line=59, type=7, data=[;], 
line=60, type=14, data=[}], 
line=61, type=14, data=[}], 
line=63, type=50, data=[start], 
line=63, type=11, data=[(], 
line=63, type=12, data=[)], 
line=63, type=13, data=[{], 
line=64, type=61, data=[printf], 
line=64, type=11, data=[(], 
line=64, type=113, data=[Hello %s! Im %i yo!
], 
line=64, type=8, data=[,], 
line=64, type=113, data=[Max], 
line=64, type=8, data=[,], 
line=64, type=5, data=[22], 
line=64, type=12, data=[)], 
line=64, type=7, data=[;], 
line=65, type=52, data=[exit], 
line=65, type=5, data=[0], 
line=65, type=7, data=[;], 
line=66, type=14, data=[}], 
line=67, type=14, data=[}],
: