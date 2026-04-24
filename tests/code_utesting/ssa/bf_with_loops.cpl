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
        i8s* %185 = load_arg();
        {
            i32s %1 = alloc(8);
            i32t %14 = i8n 0 as i32;
            i32s %186 = i32t %14;
            future: i32s %187 = previous: i32s %186;
            future: u64s* %188 = previous: u64s* %185;
            i32t %16 = i8n 1 as i32;
            i8t* %18 = i8n 1 as *ptr;
            lb15:
            [i32s %1] i32s %187 = phi(set [%186, bb69][%189, bb5]);
            [u64s %0] u64s* %188 = phi(set [%190, bb5][%185, bb69]);
            i8t %15 = *(i8s* %188);
            if i8t %15, goto lb16, else goto lb17;
            lb16:
            {
                i32t %17 = i32s %187 + i32t %16;
                i32s %189 = i32t %17;
                i8t* %19 = i8s* %188 + i8t* %18;
                i8s* %190 = i8t* %19;
            }
            future: i32s %187 = previous: i32s %189;
            future: u64s* %188 = previous: u64s* %190;
            goto lb15;
            lb17:
            return i32s %187;
        }
    }
    [unused]
    fn putc1(i8 c) -> i0
    {
        i8s %2 = alloc(8);
        i8s %191 = load_arg();
        {
            i8t* %20 = &(i8s %191);
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
            i32s %192 = load_starg();
            i8s** %7 = alloc(8);
            i8s** %193 = load_starg();
            {
                i32s %8 = alloc(8);
                i32t %21 = i8n 0 as i32;
                i32s %194 = i32t %21;
                i32s %9 = alloc(8);
                i32t %22 = i8n 0 as i32;
                i32s %195 = i32t %22;
                i32s %10 = alloc(8);
                i8t** %23 = i8s** %193;
                i8t %24 = i8n 1 * csu8 8;
                i8t** %26 = i8t %24 as *ptr;
                i8t** %25 = i8t** %23 + i8t** %26;
                i8t* %27 = *(i8t** %25);
                use i8t* %27;
                i32t %28 = call strlen0(i8* s) -> i32, argc args(i8t* %27,);
                i32s %196 = i32t %28;
                future: i32s %197 = previous: i32s %194;
                future: i32s %198 = previous: i32s %195;
                i32t* %30 = &(arrg %5);
                i32t %35 = i8n 1 as i32;
                i32t %37 = i8n 0 as i32;
                i32t %39 = i8n 1 as i32;
                i32t* %40 = &(arrg %5);
                i32t* %45 = &(arrg %4);
                i32t* %49 = &(arrg %4);
                i8t %54 = i8n 1 * csu8 8;
                i8t** %56 = i8t %54 as *ptr;
                i64t %64 = csu8 91 as i64;
                i64t %67 = csu8 93 as i64;
                i32t %70 = i8n 1 as i32;
                i32t %71 = i8n 0 as i32;
                i32t %72 = i8n 0 as i32;
                lb103:
                [i32s %8] i32s %197 = phi(set [%194, bb71][%220, bb19]);
                [i32s %9] i32s %198 = phi(set [%195, bb71][%219, bb19]);
                [unused] [i32s %11]  = phi(set );
                i32t %29 = i32s %197 < i32s %196;
                if i32t %29, goto lb104, else goto lb105;
                lb104:
                {
                    goto lb111;
                    lb113:
                    {
                        i32t %31 = i32s %198 * csu8 4;
                        i32t* %33 = i32t %31 as *ptr;
                        i32t* %32 = i32t* %30 + i32t* %33;
                        *(i32t* %32) = i32s %197;
                        i32t %34 = i32s %198 + i32t %35;
                        i32s %225 = i32t %34;
                    }
                    future: i32s %219 = previous: i32s %225;
                    goto lb112;
                    lb131:
                    {
                        i32t %36 = i32s %198 > i32t %37;
                        future: i32s %221 = previous: i32s %198;
                        if i32t %36, goto lb134, else goto lb136;
                        lb134:
                        {
                            i32t %38 = i32s %198 - i32t %39;
                            i32s %222 = i32t %38;
                            i32s %11 = alloc(8);
                            i32t %41 = i32s %222 * csu8 4;
                            i32t* %43 = i32t %41 as *ptr;
                            i32t* %42 = i32t* %40 + i32t* %43;
                            i32t %44 = *(i32t* %42);
                            i32s %223 = i32t %44;
                            i32t %46 = i32s %197 * csu8 4;
                            i32t* %48 = i32t %46 as *ptr;
                            i32t* %47 = i32t* %45 + i32t* %48;
                            *(i32t* %47) = i32s %223;
                            i32t %50 = i32s %223 * csu8 4;
                            i32t* %52 = i32t %50 as *ptr;
                            i32t* %51 = i32t* %49 + i32t* %52;
                            *(i32t* %51) = i32s %197;
                        }
                        future: i32s %221 = previous: i32s %222;
                        future: i32s %224 = previous: i32s %223;
                        goto lb136;
                        lb136:
                        [i32s %9] i32s %221 = phi(set [%198, bb13][%222, bb14]);
                        [i32s %11] i32s %224 = phi(set [%223, bb14]);
                    }
                    future: i32s %219 = previous: i32s %221;
                    goto lb112;
                    [unused] goto lb112;
                    lb111:
                    i8t** %53 = i8s** %193;
                    i8t** %55 = i8t** %53 + i8t** %56;
                    i8t* %57 = *(i8t** %55);
                    i8t* %58 = i8t* %57;
                    i32t %59 = i32s %197 * csu8 1;
                    i8t* %61 = i32t %59 as *ptr;
                    i8t* %60 = i8t* %58 + i8t* %61;
                    i8t %62 = *(i8t* %60);
                    i64t %65 = i8t %62 as i64;
                    i8t %63 = i64t %65 == i64t %64;
                    if i8t %63, goto lb113, else goto lb195;
                    lb195:
                    i64t %68 = i8t %62 as i64;
                    i8t %66 = i64t %68 == i64t %67;
                    if i8t %66, goto lb131, else goto lb200;
                    lb200:
                    future: i32s %219 = previous: i32s %198;
                    lb112:
                    [i32s %9] i32s %219 = phi(set [%225, bb12][%221, bb15][%198, bb18]);
                    [unused] [i32s %11]  = phi(set );
                    i32t %69 = i32s %197 + i32t %70;
                    i32s %220 = i32t %69;
                }
                future: i32s %197 = previous: i32s %220;
                future: i32s %198 = previous: i32s %219;
                goto lb103;
                lb105:
                i32s %12 = alloc(8);
                i32s %199 = i32t %71;
                i32s %13 = alloc(8);
                i32s %200 = i32t %72;
                future: i32s %201 = previous: i32s %199;
                future: i32s %202 = previous: i32s %200;
                i32t %75 = i8n 1 as i32;
                i32t %77 = i8n 1 as i32;
                i32t %79 = i8n 1 as i32;
                i32t %81 = i8n 1 as i32;
                i8t* %82 = &(arrg %3);
                i8t* %88 = &(arrg %3);
                i32t %93 = i8n 1 as i32;
                i8t* %94 = &(arrg %3);
                i8t* %100 = &(arrg %3);
                i32t %105 = i8n 1 as i32;
                i8t* %106 = &(arrg %3);
                i32t %112 = i8n 1 as i32;
                i8t* %113 = &(arrg %3);
                i32t* %119 = &(arrg %4);
                i32t %125 = i8n 1 as i32;
                i8t* %126 = &(arrg %3);
                i32t* %131 = &(arrg %4);
                i32t %137 = i8n 1 as i32;
                i32t %139 = i8n 1 as i32;
                i8t %141 = i8n 1 * csu8 8;
                i8t** %143 = i8t %141 as *ptr;
                i64t %150 = csu8 60 as i64;
                i64t %153 = csu8 45 as i64;
                i64t %156 = csu8 43 as i64;
                i64t %163 = csu8 46 as i64;
                i64t %170 = csu8 91 as i64;
                i64t %173 = csu8 62 as i64;
                i64t %180 = csu8 93 as i64;
                lb223:
                [i32s %12] i32s %201 = phi(set [%199, bb70][%205, bb66]);
                [i32s %13] i32s %202 = phi(set [%200, bb70][%206, bb66]);
                i32t %73 = i32s %201 < i32s %196;
                if i32t %73, goto lb224, else goto lb225;
                lb224:
                {
                    goto lb231;
                    lb233:
                    {
                        i32t %74 = i32s %202 + i32t %75;
                        i32s %214 = i32t %74;
                        i32t %76 = i32s %201 + i32t %77;
                        i32s %215 = i32t %76;
                    }
                    future: i32s %205 = previous: i32s %215;
                    future: i32s %206 = previous: i32s %214;
                    goto lb232;
                    lb248:
                    {
                        i32t %78 = i32s %202 - i32t %79;
                        i32s %203 = i32t %78;
                        i32t %80 = i32s %201 + i32t %81;
                        i32s %204 = i32t %80;
                    }
                    future: i32s %205 = previous: i32s %204;
                    future: i32s %206 = previous: i32s %203;
                    goto lb232;
                    lb263:
                    {
                        i32t %83 = i32s %202 * csu8 1;
                        i8t* %85 = i32t %83 as *ptr;
                        i8t* %84 = i8t* %82 + i8t* %85;
                        i8t %86 = *(i8t* %84);
                        i8t %87 = i8t %86 + i8n 1;
                        i32t %89 = i32s %202 * csu8 1;
                        i8t* %91 = i32t %89 as *ptr;
                        i8t* %90 = i8t* %88 + i8t* %91;
                        *(i8t* %90) = i8t %87;
                        i32t %92 = i32s %201 + i32t %93;
                        i32s %218 = i32t %92;
                    }
                    future: i32s %205 = previous: i32s %218;
                    future: i32s %206 = previous: i32s %202;
                    goto lb232;
                    lb290:
                    {
                        i32t %95 = i32s %202 * csu8 1;
                        i8t* %97 = i32t %95 as *ptr;
                        i8t* %96 = i8t* %94 + i8t* %97;
                        i8t %98 = *(i8t* %96);
                        i8t %99 = i8t %98 - i8n 1;
                        i32t %101 = i32s %202 * csu8 1;
                        i8t* %103 = i32t %101 as *ptr;
                        i8t* %102 = i8t* %100 + i8t* %103;
                        *(i8t* %102) = i8t %99;
                        i32t %104 = i32s %201 + i32t %105;
                        i32s %216 = i32t %104;
                    }
                    future: i32s %205 = previous: i32s %216;
                    future: i32s %206 = previous: i32s %202;
                    goto lb232;
                    lb317:
                    {
                        i32t %107 = i32s %202 * csu8 1;
                        i8t* %109 = i32t %107 as *ptr;
                        i8t* %108 = i8t* %106 + i8t* %109;
                        i8t %110 = *(i8t* %108);
                        use i8t %110;
                        call putc1(i8 c) -> i0, argc args(i8t %110,);
                        i32t %111 = i32s %201 + i32t %112;
                        i32s %217 = i32t %111;
                    }
                    future: i32s %205 = previous: i32s %217;
                    future: i32s %206 = previous: i32s %202;
                    goto lb232;
                    lb338:
                    {
                        i32t %114 = i32s %202 * csu8 1;
                        i8t* %116 = i32t %114 as *ptr;
                        i8t* %115 = i8t* %113 + i8t* %116;
                        i8t %117 = *(i8t* %115);
                        i8t %118 = not i8t %117;
                        if i8t %118, goto lb341, else goto lb342;
                        lb341:
                        {
                            i32t %120 = i32s %201 * csu8 4;
                            i32t* %122 = i32t %120 as *ptr;
                            i32t* %121 = i32t* %119 + i32t* %122;
                            i32t %123 = *(i32t* %121);
                            i32s %209 = i32t %123;
                        }
                        future: i32s %208 = previous: i32s %209;
                        goto lb343;
                        lb342:
                        {
                            i32t %124 = i32s %201 + i32t %125;
                            i32s %207 = i32t %124;
                        }
                        future: i32s %208 = previous: i32s %207;
                        goto lb343;
                        lb343:
                        [i32s %12] i32s %208 = phi(set [%207, bb30][%209, bb29]);
                    }
                    future: i32s %205 = previous: i32s %208;
                    future: i32s %206 = previous: i32s %202;
                    goto lb232;
                    lb373:
                    {
                        i32t %127 = i32s %202 * csu8 1;
                        i8t* %129 = i32t %127 as *ptr;
                        i8t* %128 = i8t* %126 + i8t* %129;
                        i8t %130 = *(i8t* %128);
                        if i8t %130, goto lb376, else goto lb377;
                        lb376:
                        {
                            i32t %132 = i32s %201 * csu8 4;
                            i32t* %134 = i32t %132 as *ptr;
                            i32t* %133 = i32t* %131 + i32t* %134;
                            i32t %135 = *(i32t* %133);
                            i32s %212 = i32t %135;
                        }
                        future: i32s %211 = previous: i32s %212;
                        goto lb378;
                        lb377:
                        {
                            i32t %136 = i32s %201 + i32t %137;
                            i32s %210 = i32t %136;
                        }
                        future: i32s %211 = previous: i32s %210;
                        goto lb378;
                        lb378:
                        [i32s %12] i32s %211 = phi(set [%210, bb34][%212, bb33]);
                    }
                    future: i32s %205 = previous: i32s %211;
                    future: i32s %206 = previous: i32s %202;
                    goto lb232;
                    lb407:
                    {
                        i32t %138 = i32s %201 + i32t %139;
                        i32s %213 = i32t %138;
                    }
                    future: i32s %205 = previous: i32s %213;
                    future: i32s %206 = previous: i32s %202;
                    goto lb232;
                    [unused] goto lb232;
                    lb231:
                    i8t** %140 = i8s** %193;
                    i8t** %142 = i8t** %140 + i8t** %143;
                    i8t* %144 = *(i8t** %142);
                    i8t* %145 = i8t* %144;
                    i32t %146 = i32s %201 * csu8 1;
                    i8t* %148 = i32t %146 as *ptr;
                    i8t* %147 = i8t* %145 + i8t* %148;
                    i8t %149 = *(i8t* %147);
                    i64t %152 = i8t %149 as i64;
                    i8t %151 = i64t %152 < i64t %150;
                    if i8t %151, goto lb432, else goto lb433;
                    lb432:
                    i64t %155 = i8t %149 as i64;
                    i8t %154 = i64t %155 < i64t %153;
                    if i8t %154, goto lb440, else goto lb441;
                    lb440:
                    i64t %158 = i8t %149 as i64;
                    i8t %157 = i64t %158 < i64t %156;
                    if i8t %157, goto lb448, else goto lb449;
                    lb448:
                    goto lb407;
                    lb449:
                    i64t %160 = i8t %149 as i64;
                    i8t %159 = i64t %160 > i64t %156;
                    if i8t %159, goto lb450, else goto lb451;
                    lb450:
                    goto lb407;
                    lb451:
                    goto lb263;
                    lb441:
                    i64t %162 = i8t %149 as i64;
                    i8t %161 = i64t %162 > i64t %153;
                    if i8t %161, goto lb442, else goto lb443;
                    lb442:
                    i64t %165 = i8t %149 as i64;
                    i8t %164 = i64t %165 < i64t %163;
                    if i8t %164, goto lb460, else goto lb461;
                    lb460:
                    goto lb407;
                    lb461:
                    i64t %167 = i8t %149 as i64;
                    i8t %166 = i64t %167 > i64t %163;
                    if i8t %166, goto lb462, else goto lb463;
                    lb462:
                    goto lb407;
                    lb463:
                    goto lb317;
                    lb443:
                    goto lb290;
                    lb433:
                    i64t %169 = i8t %149 as i64;
                    i8t %168 = i64t %169 > i64t %150;
                    if i8t %168, goto lb434, else goto lb435;
                    lb434:
                    i64t %172 = i8t %149 as i64;
                    i8t %171 = i64t %172 < i64t %170;
                    if i8t %171, goto lb472, else goto lb473;
                    lb472:
                    i64t %175 = i8t %149 as i64;
                    i8t %174 = i64t %175 < i64t %173;
                    if i8t %174, goto lb480, else goto lb481;
                    lb480:
                    goto lb407;
                    lb481:
                    i64t %177 = i8t %149 as i64;
                    i8t %176 = i64t %177 > i64t %173;
                    if i8t %176, goto lb482, else goto lb483;
                    lb482:
                    goto lb407;
                    lb483:
                    goto lb233;
                    lb473:
                    i64t %179 = i8t %149 as i64;
                    i8t %178 = i64t %179 > i64t %170;
                    if i8t %178, goto lb474, else goto lb475;
                    lb474:
                    i64t %182 = i8t %149 as i64;
                    i8t %181 = i64t %182 < i64t %180;
                    if i8t %181, goto lb492, else goto lb493;
                    lb492:
                    goto lb407;
                    lb493:
                    i64t %184 = i8t %149 as i64;
                    i8t %183 = i64t %184 > i64t %180;
                    if i8t %183, goto lb494, else goto lb495;
                    lb494:
                    goto lb407;
                    lb495:
                    goto lb373;
                    lb475:
                    goto lb338;
                    lb435:
                    goto lb248;
                    lb232:
                    [i32s %12] i32s %205 = phi(set [%211, bb35][%218, bb25][%217, bb27][%213, bb36][%215, bb23][%208, bb31][%216, bb26][%204, bb24]);
                    [i32s %13] i32s %206 = phi(set [%202, bb31][%202, bb35][%202, bb26][%203, bb24][%202, bb25][%202, bb36][%214, bb23][%202, bb27]);
                }
                future: i32s %201 = previous: i32s %205;
                future: i32s %202 = previous: i32s %206;
                goto lb223;
                lb225:
                exit i8n 0;
            }
        }
    [unused] }
}
/: