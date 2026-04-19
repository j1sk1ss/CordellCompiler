function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }
    return l;
}

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

glob arr tape[64, i8];
glob arr bracketmap[64, i32];
glob arr stack[64, i32];

start(i32 argc, ptr ptr i8 argv) {
    i32 pos = 0;
    i32 stackptr = 0;
    i32 n = strlen(argv[1]);

    while pos < n; {
        @[no_fall]
        @[straight]
        switch argv[1][pos]; {
            case '['; {
                stack[stackptr] = pos;
                stackptr += 1;
            }
            case ']'; {
                if stackptr > 0; {
                    stackptr -= 1;
                    i32 m = stack[stackptr];
                    bracketmap[pos] = m;
                    bracketmap[m] = pos;
                }
            }
        }
        pos += 1;
    }

    i32 pc = 0;
    i32 pointer = 0;
    while pc < n; {
        @[no_fall]
        switch argv[1][pc]; {
            case '>'; {
                pointer += 1;
                pc += 1;
            }
            case '<'; {
                pointer -= 1;
                pc += 1;
            }
            case '+'; {
                tape[pointer] += 1;
                pc += 1;
            }
            case '-'; {
                tape[pointer] -= 1;
                pc += 1;
            }
            case '.'; {
                putc(tape[pointer]);
                pc += 1;
            }
            case '['; {
                if not tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            case ']'; {
                if tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            default {
                pc += 1;
            }
        }
    }

    exit 0;
}

:/ OUTPUT
{
    fn strlen0(i8* s) -> i32
    {
        i8s* %0 = alloc(8);
        i8s* %176 = load_arg();
        {
            i32s %1 = alloc(8);
            i32t %14 = i8n 0 as i32;
            i32s %177 = i32t %14;
            future: u64s* %179 = previous: u64s* %176;
            future: i32s %178 = previous: i32s %177;
            i32t %16 = i8n 1 as i32;
            i8t* %18 = i8n 1 as *ptr;
            lb21:
            [i32s %1] i32s %178 = phi(set [%177, bb71][%180, bb5]);
            [u64s %0] u64s* %179 = phi(set [%176, bb71][%181, bb5]);
            i8t %15 = *(i8s* %179);
            if i8t %15, goto lb22, else goto lb23;
            lb22:
            {
                i32t %17 = i32s %178 + i32t %16;
                i32s %180 = i32t %17;
                i8t* %19 = i8s* %179 + i8t* %18;
                i8s* %181 = i8t* %19;
            }
            future: i32s %178 = previous: i32s %180;
            future: u64s* %179 = previous: u64s* %181;
            goto lb21;
            lb23:
            return i32s %178;
        }
    }
    [unused]
    fn putc1(i8 c) -> i0
    {
        i8s %2 = alloc(8);
        i8s %182 = load_arg();
        {
            i8t* %20 = &(i8s %182);
            use i8t* %20;
            syscall, argc: args(i32n 33554436,i8n 1,i8t* %20,i8n 1,);
        }
    }
    arrg %3 = arr_alloc(i8n 64);
    arrg %4 = arr_alloc(i8n 64);
    arrg %5 = arr_alloc(i8n 64);
    start {
        {
            i32s %6 = alloc(8);
            i32s %183 = load_starg();
            i8s** %7 = alloc(8);
            i8s** %184 = load_starg();
            {
                i32s %8 = alloc(8);
                i32t %21 = i8n 0 as i32;
                i32s %185 = i32t %21;
                i32s %9 = alloc(8);
                i32t %22 = i8n 0 as i32;
                i32s %186 = i32t %22;
                i32s %10 = alloc(8);
                i8t** %23 = i8s** %184;
                i64t %24 = i8n 1 * cnst? 8;
                i8t** %26 = i64t %24 as *ptr;
                i8t** %25 = i8t** %23 + i8t** %26;
                i8t* %27 = *(i8t** %25);
                use i8t* %27;
                i32t %28 = call strlen0(i8* s) -> i32, argc args(i8t* %27,);
                i32s %187 = i32t %28;
                future: i32s %189 = previous: i32s %186;
                future: i32s %188 = previous: i32s %185;
                i32t* %30 = &(arrg %5);
                i32t %35 = i8n 1 as i32;
                i32t %37 = i8n 0 as i32;
                i32t %39 = i8n 1 as i32;
                i32t* %40 = &(arrg %5);
                i32t* %45 = &(arrg %4);
                i32t* %49 = &(arrg %4);
                i64t %54 = i8n 1 * cnst? 8;
                i32t %68 = i8n 1 as i32;
                i32t %69 = i8n 0 as i32;
                i32t %70 = i8n 0 as i32;
                i32t %73 = i8n 1 as i32;
                i32t %75 = i8n 1 as i32;
                i32t %77 = i8n 1 as i32;
                i32t %79 = i8n 1 as i32;
                i8t* %80 = &(arrg %3);
                i8t* %86 = &(arrg %3);
                i32t %91 = i8n 1 as i32;
                i8t* %92 = &(arrg %3);
                i8t* %98 = &(arrg %3);
                i32t %103 = i8n 1 as i32;
                i8t* %104 = &(arrg %3);
                i32t %110 = i8n 1 as i32;
                i8t* %111 = &(arrg %3);
                i32t* %117 = &(arrg %4);
                i32t %123 = i8n 1 as i32;
                i8t* %124 = &(arrg %3);
                i32t* %129 = &(arrg %4);
                i32t %135 = i8n 1 as i32;
                i32t %137 = i8n 1 as i32;
                i64t %139 = i8n 1 * cnst? 8;
            [unused] }
            i8t** %56 = i64t %54 as *ptr;
            i8t** %141 = i64t %139 as *ptr;
            lb145:
            [i32s %8] i32s %188 = phi(set [%211, bb20][%185, bb73]);
            [i32s %9] i32s %189 = phi(set [%210, bb20][%186, bb73]);
            [unused] [i32s %11]  = phi(set );
            i32t %29 = i32s %188 < i32s %187;
            if i32t %29, goto lb146, else goto lb147;
            lb146:
            {
                goto lb157;
                lb159:
                {
                    i64t %31 = i32s %189 * cnst? 4;
                    i32t* %33 = i64t %31 as *ptr;
                    i32t* %32 = i32t* %30 + i32t* %33;
                    *(i32t* %32) = i32s %188;
                    i32t %34 = i32s %189 + i32t %35;
                    i32s %216 = i32t %34;
                }
                future: i32s %210 = previous: i32s %216;
                goto lb158;
                lb187:
                {
                    i32t %36 = i32s %189 > i32t %37;
                    future: i32s %212 = previous: i32s %189;
                    if i32t %36, goto lb192, else goto lb194;
                    lb192:
                    {
                        i32t %38 = i32s %189 - i32t %39;
                        i32s %213 = i32t %38;
                        i32s %11 = alloc(8);
                        i64t %41 = i32s %213 * cnst? 4;
                        i32t* %43 = i64t %41 as *ptr;
                        i32t* %42 = i32t* %40 + i32t* %43;
                        i32t %44 = *(i32t* %42);
                        i32s %214 = i32t %44;
                        i64t %46 = i32s %188 * cnst? 4;
                        i32t* %48 = i64t %46 as *ptr;
                        i32t* %47 = i32t* %45 + i32t* %48;
                        *(i32t* %47) = i32s %214;
                        i64t %50 = i32s %214 * cnst? 4;
                        i32t* %52 = i64t %50 as *ptr;
                        i32t* %51 = i32t* %49 + i32t* %52;
                        *(i32t* %51) = i32s %188;
                    }
                    future: i32s %212 = previous: i32s %213;
                    future: i32s %215 = previous: i32s %214;
                    goto lb194;
                    lb194:
                    [i32s %9] i32s %212 = phi(set [%189, bb13][%213, bb14]);
                    [i32s %11] i32s %215 = phi(set [%214, bb14]);
                }
                future: i32s %210 = previous: i32s %212;
                goto lb158;
                [unused] goto lb158;
                lb157:
                i8t** %53 = i8s** %184;
                i8t** %55 = i8t** %53 + i8t** %56;
                i8t* %57 = *(i8t** %55);
                i8t* %58 = i8t* %57;
                i64t %59 = i32s %188 * cnst? 1;
                i8t* %61 = i64t %59 as *ptr;
                i8t* %60 = i8t* %58 + i8t* %61;
                i8t %62 = *(i8t* %60);
                i64t %64 = i8t %62;
                i8t %63 = i64t %64 == cnst? 91;
                if i8t %63, goto lb159, else goto lb279;
                lb279:
                i64t %66 = i8t %62;
                i8t %65 = i64t %66 == cnst? 93;
                if i8t %65, goto lb187, else goto lb283;
                lb283:
                future: i32s %210 = previous: i32s %189;
                lb158:
                [i32s %9] i32s %210 = phi(set [%189, bb19][%212, bb15][%216, bb12]);
                [unused] [i32s %11]  = phi(set );
                i32t %67 = i32s %188 + i32t %68;
                i32s %211 = i32t %67;
            }
            future: i32s %188 = previous: i32s %211;
            future: i32s %189 = previous: i32s %210;
            goto lb145;
            lb147:
            i32s %12 = alloc(8);
            i32s %190 = i32t %69;
            i32s %13 = alloc(8);
            i32s %191 = i32t %70;
            future: i32s %193 = previous: i32s %191;
            future: i32s %192 = previous: i32s %190;
            lb314:
            [i32s %12] i32s %192 = phi(set [%190, bb72][%196, bb68]);
            [i32s %13] i32s %193 = phi(set [%197, bb68][%191, bb72]);
            i32t %71 = i32s %192 < i32s %187;
            if i32t %71, goto lb315, else goto lb316;
            lb315:
            {
                goto lb326;
                lb328:
                {
                    i32t %72 = i32s %193 + i32t %73;
                    i32s %205 = i32t %72;
                    i32t %74 = i32s %192 + i32t %75;
                    i32s %206 = i32t %74;
                }
                future: i32s %196 = previous: i32s %206;
                future: i32s %197 = previous: i32s %205;
                goto lb327;
                lb351:
                {
                    i32t %76 = i32s %193 - i32t %77;
                    i32s %194 = i32t %76;
                    i32t %78 = i32s %192 + i32t %79;
                    i32s %195 = i32t %78;
                }
                future: i32s %196 = previous: i32s %195;
                future: i32s %197 = previous: i32s %194;
                goto lb327;
                lb374:
                {
                    i64t %81 = i32s %193 * cnst? 1;
                    i8t* %83 = i64t %81 as *ptr;
                    i8t* %82 = i8t* %80 + i8t* %83;
                    i8t %84 = *(i8t* %82);
                    i8t %85 = i8t %84 + i8n 1;
                    i64t %87 = i32s %193 * cnst? 1;
                    i8t* %89 = i64t %87 as *ptr;
                    i8t* %88 = i8t* %86 + i8t* %89;
                    *(i8t* %88) = i8t %85;
                    i32t %90 = i32s %192 + i32t %91;
                    i32s %209 = i32t %90;
                }
                future: i32s %196 = previous: i32s %209;
                future: i32s %197 = previous: i32s %193;
                goto lb327;
                lb414:
                {
                    i64t %93 = i32s %193 * cnst? 1;
                    i8t* %95 = i64t %93 as *ptr;
                    i8t* %94 = i8t* %92 + i8t* %95;
                    i8t %96 = *(i8t* %94);
                    i8t %97 = i8t %96 - i8n 1;
                    i64t %99 = i32s %193 * cnst? 1;
                    i8t* %101 = i64t %99 as *ptr;
                    i8t* %100 = i8t* %98 + i8t* %101;
                    *(i8t* %100) = i8t %97;
                    i32t %102 = i32s %192 + i32t %103;
                    i32s %207 = i32t %102;
                }
                future: i32s %196 = previous: i32s %207;
                future: i32s %197 = previous: i32s %193;
                goto lb327;
                lb454:
                {
                    i64t %105 = i32s %193 * cnst? 1;
                    i8t* %107 = i64t %105 as *ptr;
                    i8t* %106 = i8t* %104 + i8t* %107;
                    i8t %108 = *(i8t* %106);
                    use i8t %108;
                    call putc1(i8 c) -> i0, argc args(i8t %108,);
                    i32t %109 = i32s %192 + i32t %110;
                    i32s %208 = i32t %109;
                }
                future: i32s %196 = previous: i32s %208;
                future: i32s %197 = previous: i32s %193;
                goto lb327;
                lb483:
                {
                    i64t %112 = i32s %193 * cnst? 1;
                    i8t* %114 = i64t %112 as *ptr;
                    i8t* %113 = i8t* %111 + i8t* %114;
                    i8t %115 = *(i8t* %113);
                    i8t %116 = not i8t %115;
                    if i8t %116, goto lb488, else goto lb489;
                    lb488:
                    {
                        i64t %118 = i32s %192 * cnst? 4;
                        i32t* %120 = i64t %118 as *ptr;
                        i32t* %119 = i32t* %117 + i32t* %120;
                        i32t %121 = *(i32t* %119);
                        i32s %200 = i32t %121;
                    }
                    future: i32s %199 = previous: i32s %200;
                    goto lb490;
                    lb489:
                    {
                        i32t %122 = i32s %192 + i32t %123;
                        i32s %198 = i32t %122;
                    }
                    future: i32s %199 = previous: i32s %198;
                    goto lb490;
                    lb490:
                    [i32s %12] i32s %199 = phi(set [%200, bb30][%198, bb31]);
                }
                future: i32s %196 = previous: i32s %199;
                future: i32s %197 = previous: i32s %193;
                goto lb327;
                lb534:
                {
                    i64t %125 = i32s %193 * cnst? 1;
                    i8t* %127 = i64t %125 as *ptr;
                    i8t* %126 = i8t* %124 + i8t* %127;
                    i8t %128 = *(i8t* %126);
                    if i8t %128, goto lb539, else goto lb540;
                    lb539:
                    {
                        i64t %130 = i32s %192 * cnst? 4;
                        i32t* %132 = i64t %130 as *ptr;
                        i32t* %131 = i32t* %129 + i32t* %132;
                        i32t %133 = *(i32t* %131);
                        i32s %203 = i32t %133;
                    }
                    future: i32s %202 = previous: i32s %203;
                    goto lb541;
                    lb540:
                    {
                        i32t %134 = i32s %192 + i32t %135;
                        i32s %201 = i32t %134;
                    }
                    future: i32s %202 = previous: i32s %201;
                    goto lb541;
                    lb541:
                    [i32s %12] i32s %202 = phi(set [%203, bb34][%201, bb35]);
                }
                future: i32s %196 = previous: i32s %202;
                future: i32s %197 = previous: i32s %193;
                goto lb327;
                lb583:
                {
                    i32t %136 = i32s %192 + i32t %137;
                    i32s %204 = i32t %136;
                }
                future: i32s %196 = previous: i32s %204;
                future: i32s %197 = previous: i32s %193;
                goto lb327;
                [unused] goto lb327;
                lb326:
                i8t** %138 = i8s** %184;
                i8t** %140 = i8t** %138 + i8t** %141;
                i8t* %142 = *(i8t** %140);
                i8t* %143 = i8t* %142;
                i64t %144 = i32s %192 * cnst? 1;
                i8t* %146 = i64t %144 as *ptr;
                i8t* %145 = i8t* %143 + i8t* %146;
                i8t %147 = *(i8t* %145);
                i64t %149 = i8t %147;
                i8t %148 = i64t %149 < cnst? 60;
                if i8t %148, goto lb617, else goto lb618;
                lb617:
                i64t %151 = i8t %147;
                i8t %150 = i64t %151 < cnst? 45;
                if i8t %150, goto lb624, else goto lb625;
                lb624:
                i64t %153 = i8t %147;
                i8t %152 = i64t %153 < cnst? 43;
                if i8t %152, goto lb631, else goto lb632;
                lb631:
                goto lb583;
                lb632:
                i64t %155 = i8t %147;
                i8t %154 = i64t %155 > cnst? 43;
                if i8t %154, goto lb633, else goto lb634;
                lb633:
                goto lb583;
                lb634:
                goto lb374;
                lb625:
                i64t %157 = i8t %147;
                i8t %156 = i64t %157 > cnst? 45;
                if i8t %156, goto lb626, else goto lb627;
                lb626:
                i64t %159 = i8t %147;
                i8t %158 = i64t %159 < cnst? 46;
                if i8t %158, goto lb644, else goto lb645;
                lb644:
                goto lb583;
                lb645:
                i64t %161 = i8t %147;
                i8t %160 = i64t %161 > cnst? 46;
                if i8t %160, goto lb646, else goto lb647;
                lb646:
                goto lb583;
                lb647:
                goto lb454;
                lb627:
                goto lb414;
                lb618:
                i64t %163 = i8t %147;
                i8t %162 = i64t %163 > cnst? 60;
                if i8t %162, goto lb619, else goto lb620;
                lb619:
                i64t %165 = i8t %147;
                i8t %164 = i64t %165 < cnst? 91;
                if i8t %164, goto lb657, else goto lb658;
                lb657:
                i64t %167 = i8t %147;
                i8t %166 = i64t %167 < cnst? 62;
                if i8t %166, goto lb664, else goto lb665;
                lb664:
                goto lb583;
                lb665:
                i64t %169 = i8t %147;
                i8t %168 = i64t %169 > cnst? 62;
                if i8t %168, goto lb666, else goto lb667;
                lb666:
                goto lb583;
                lb667:
                goto lb328;
                lb658:
                i64t %171 = i8t %147;
                i8t %170 = i64t %171 > cnst? 91;
                if i8t %170, goto lb659, else goto lb660;
                lb659:
                i64t %173 = i8t %147;
                i8t %172 = i64t %173 < cnst? 93;
                if i8t %172, goto lb677, else goto lb678;
                lb677:
                goto lb583;
                lb678:
                i64t %175 = i8t %147;
                i8t %174 = i64t %175 > cnst? 93;
                if i8t %174, goto lb679, else goto lb680;
                lb679:
                goto lb583;
                lb680:
                goto lb534;
                lb660:
                goto lb483;
                lb620:
                goto lb351;
                lb327:
                [i32s %12] i32s %196 = phi(set [%206, bb24][%195, bb25][%199, bb32][%202, bb36][%204, bb37][%207, bb27][%209, bb26][%208, bb28]);
                [i32s %13] i32s %197 = phi(set [%193, bb28][%194, bb25][%193, bb37][%193, bb27][%193, bb36][%193, bb32][%193, bb26][%205, bb24]);
            }
            future: i32s %192 = previous: i32s %196;
            future: i32s %193 = previous: i32s %197;
            goto lb314;
            lb316:
            exit i8n 0;
        }
    }
}
/: