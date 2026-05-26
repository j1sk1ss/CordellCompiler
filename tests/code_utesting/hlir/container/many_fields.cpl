@[align(1)]
container storage {
    i8  wood;
    i16 steel;
    u64 water;
    i8  ppl;
}

start() {
    storage s;
    s.wood = 1;
    s.ppl = 0;
}

:/ OUTPUT
BB1: start
%4 = arr_alloc(12);
%5 = &(%4);
%6 = %5 + 0;
*(%6) = $1;
%7 = &(%4);
%8 = %7 + 11;
*(%8) = $0;
send
/: