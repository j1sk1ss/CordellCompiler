@[vname("__exit")] @[abi] extern function done(i32 status) -> i0;

container Stream {
    ptr i8 buf;
    u64    len;
    i64    pos;

    @[self] function set(ptr Stream self, ptr i8 buf, u64 len) -> i0;
    @[self] function next(ptr Stream self) -> i32;
}

function Stream::set(ptr Stream self, ptr i8 buf, u64 len) -> i0 {
    self.buf = buf;
    self.len = len;
    self.pos = -1 as i64;
}

function Stream::next(ptr Stream self) -> i32 {
    self.pos += 1;
    if self.pos >= self.len as i64; {
        return -1;
    }

    self.buf[self.pos] as i32
}

container Machine {
    Stream tokenizer;
    arr code[2, i8];
    i32 scope;
    arr args[10, i32];
    i32 out;

    @[self] function run(ptr Machine self) -> i0;
}

function Machine::run(ptr Machine self) -> i0 {
    self.tokenizer.set(ref self.code, 2 as u64);

    i32 curr = self.tokenizer.next();
    while curr >= 0; {
        @[no_fall]
        switch curr; {
            case '#'; {
                curr = self.tokenizer.next();
                if self.scope > 0 && curr >= '0' && curr <= '9'; {
                    self.out = self.args[curr - '0'];
                }
            }
        }

        curr = self.tokenizer.next();
    }
}

@[section(".bss")] glob Machine m;

start() {
    m.code[0] = '#';
    m.code[1] = '0';
    m.scope   = 1;
    m.args[0] = 7;
    m.out     = 0;
    m.run();
    done(m.out);
}

:/ OUTPUT
{
    (fun) extern(csu8 0);
    fn set1__Stream(u8* self, i8* buf, u64 len) -> i0
    {
        u8s* %8 = alloc;
        u8s* %8 = load_arg();
        i8s* %9 = alloc;
        i8s* %9 = load_arg();
        u64s %10 = alloc;
        u64s %10 = load_arg();
        {
            u8t** %21 = u8s* %8 + csu8 0;
            i8t** %22 = u8t** %21 as *ptr;
            *(i8t** %22) = i8s* %9;
            u8t* %23 = u8s* %8 + csu8 8;
            u64t* %24 = u8t* %23 as *ptr;
            *(u64t* %24) = u64s %10;
            i64t %25 = i8n -1 as i64;
            u8t* %26 = u8s* %8 + csu8 16;
            i64t* %27 = u8t* %26 as *ptr;
            *(i64t* %27) = i64t %25;
        }
    }
    fn next2__Stream(u8* self) -> i32
    {
        u8s* %11 = alloc;
        u8s* %11 = load_arg();
        {
            u8t* %28 = u8s* %11 + csu8 16;
            i64t %29 = *(u8t* %28);
            i64t %31 = i8n 1 as i64;
            i64t %30 = i64t %29 + i64t %31;
            u8t* %32 = u8s* %11 + csu8 16;
            i64t* %33 = u8t* %32 as *ptr;
            *(i64t* %33) = i64t %30;
            u8t* %34 = u8s* %11 + csu8 16;
            i64t %35 = *(u8t* %34);
            u8t* %37 = u8s* %11 + csu8 8;
            u64t %38 = *(u8t* %37);
            i64t %36 = u64t %38 as i64;
            i64t %39 = i64t %35 >= i64t %36;
            if i64t %39, goto lb63, else goto lb65;
            lb63:
            {
                i32t %40 = i8n -1 as i32;
                return i32t %40;
            }
            goto lb65;
            lb65:
            u8t* %42 = u8s* %11 + csu8 0;
            i8t* %43 = *(u8t* %42);
            u8t* %44 = u8s* %11 + csu8 16;
            i64t %45 = *(u8t* %44);
            i8t* %46 = i8t* %43;
            u64t %48 = i64t %45 as u64;
            u64t %47 = u64t %48 * csu8 1;
            i8t* %50 = u64t %47 as *ptr;
            i8t* %49 = i8t* %46 + i8t* %50;
            i8t %51 = *(i8t* %49);
            i32t %41 = i8t %51 as i32;
            use i32t %41;
        }
    }
    fn run3__Machine(u8* self) -> i0
    {
        u8s* %18 = alloc;
        u8s* %18 = load_arg();
        {
            u8t* %52 = u8s* %18 + csu8 0;
            use u8t* %52;
            u8t* %53 = u8s* %18 + csu8 24;
            i8t* %54 = u8t* %53 as *ptr;
            use i8t* %54;
            u64t %55 = i8n 2 as u64;
            use u64t %55;
            u8s* %8 = u8t* %52;
            i8s* %9 = i8t* %54;
            u64s %10 = u64t %55;
            u8t** %130 = u8s* %8 + csu8 0;
            i8t** %131 = u8t** %130 as *ptr;
            *(i8t** %131) = i8s* %9;
            u8t* %132 = u8s* %8 + csu8 8;
            u64t* %133 = u8t* %132 as *ptr;
            *(u64t* %133) = u64s %10;
            i64t %134 = i8n -1 as i64;
            u8t* %135 = u8s* %8 + csu8 16;
            i64t* %136 = u8t* %135 as *ptr;
            *(i64t* %136) = i64t %134;
            lb345:
            [unused] set1__Stream(u8t* %52, i8t* %54, u64t %55);
            i32s %19 = alloc;
            u8t* %56 = u8s* %18 + csu8 0;
            use u8t* %56;
            u8s* %11 = u8t* %56;
            u8t* %137 = u8s* %11 + csu8 16;
            i64t %138 = *(u8t* %137);
            i64t %139 = i8n 1 as i64;
            i64t %140 = i64t %138 + i64t %139;
            u8t* %141 = u8s* %11 + csu8 16;
            i64t* %142 = u8t* %141 as *ptr;
            *(i64t* %142) = i64t %140;
            u8t* %143 = u8s* %11 + csu8 16;
            i64t %144 = *(u8t* %143);
            u8t* %145 = u8s* %11 + csu8 8;
            u64t %146 = *(u8t* %145);
            i64t %147 = u64t %146 as i64;
            i64t %148 = i64t %144 >= i64t %147;
            if i64t %148, goto lb415, else goto lb417;
            lb415:
            {
                i32t %149 = i8n -1 as i32;
                i32t %57 = i32t %149;
                goto lb379;
            }
            goto lb417;
            lb417:
            u8t* %150 = u8s* %11 + csu8 0;
            i8t* %151 = *(u8t* %150);
            u8t* %152 = u8s* %11 + csu8 16;
            i64t %153 = *(u8t* %152);
            i8t* %154 = i8t* %151;
            u64t %155 = i64t %153 as u64;
            u64t %156 = u64t %155 * csu8 1;
            i8t* %157 = u64t %156 as *ptr;
            i8t* %158 = i8t* %154 + i8t* %157;
            i8t %159 = *(i8t* %158);
            i32t %160 = i8t %159 as i32;
            use i32t %160;
            lb379:
            [unused] i32t %57 = next2__Stream(u8t* %56);
            i32s %19 = i32t %57;
            lb138:
            i32t %59 = i8n 0 as i32;
            i32t %58 = i32s %19 >= i32t %59;
            if i32t %58, goto lb139, else goto lb140;
            lb139:
            {
                goto lb147;
                lb149:
                {
                    u8t* %60 = u8s* %18 + csu8 0;
                    use u8t* %60;
                    u8s* %11 = u8t* %60;
                    u8t* %161 = u8s* %11 + csu8 16;
                    i64t %162 = *(u8t* %161);
                    i64t %163 = i8n 1 as i64;
                    i64t %164 = i64t %162 + i64t %163;
                    u8t* %165 = u8s* %11 + csu8 16;
                    i64t* %166 = u8t* %165 as *ptr;
                    *(i64t* %166) = i64t %164;
                    u8t* %167 = u8s* %11 + csu8 16;
                    i64t %168 = *(u8t* %167);
                    u8t* %169 = u8s* %11 + csu8 8;
                    u64t %170 = *(u8t* %169);
                    i64t %171 = u64t %170 as i64;
                    i64t %172 = i64t %168 >= i64t %171;
                    if i64t %172, goto lb501, else goto lb503;
                    lb501:
                    {
                        i32t %173 = i8n -1 as i32;
                        i32t %61 = i32t %173;
                        goto lb465;
                    }
                    goto lb503;
                    lb503:
                    u8t* %174 = u8s* %11 + csu8 0;
                    i8t* %175 = *(u8t* %174);
                    u8t* %176 = u8s* %11 + csu8 16;
                    i64t %177 = *(u8t* %176);
                    i8t* %178 = i8t* %175;
                    u64t %179 = i64t %177 as u64;
                    u64t %180 = u64t %179 * csu8 1;
                    i8t* %181 = u64t %180 as *ptr;
                    i8t* %182 = i8t* %178 + i8t* %181;
                    i8t %183 = *(i8t* %182);
                    i32t %184 = i8t %183 as i32;
                    use i32t %184;
                    lb465:
                    [unused] i32t %61 = next2__Stream(u8t* %60);
                    i32s %19 = i32t %61;
                    i64s %62 = alloc;
                    i64s %63 = alloc;
                    u8t* %64 = u8s* %18 + csu8 32;
                    i32t %65 = *(u8t* %64);
                    i32t %67 = i8n 0 as i32;
                    i32t %66 = i32t %65 > i32t %67;
                    if i32t %66, goto lb175, else goto lb176;
                    lb175:
                    i32t %69 = i8n 48 as i32;
                    i32t %68 = i32s %19 >= i32t %69;
                    i64s %63 = alloc;
                    i64t %70 = i32t %68 as i64;
                    i64s %63 = i64t %70;
                    goto lb177;
                    lb176:
                    i64s %63 = csu8 0;
                    goto lb177;
                    lb177:
                    if i64s %63, goto lb186, else goto lb187;
                    lb186:
                    i32t %72 = i8n 57 as i32;
                    i32t %71 = i32s %19 <= i32t %72;
                    i64s %62 = alloc;
                    i64t %73 = i32t %71 as i64;
                    i64s %62 = i64t %73;
                    goto lb188;
                    lb187:
                    i64s %62 = csu8 0;
                    goto lb188;
                    lb188:
                    if i64s %62, goto lb163, else goto lb165;
                    lb163:
                    {
                        u8t* %74 = u8s* %18 + csu8 40;
                        i32t* %75 = u8t* %74 as *ptr;
                        i32t %77 = i8n 48 as i32;
                        i32t %76 = i32s %19 - i32t %77;
                        i32t* %78 = i32t* %75;
                        u64t %80 = i32t %76 as u64;
                        u64t %79 = u64t %80 * csu8 4;
                        i32t* %82 = u64t %79 as *ptr;
                        i32t* %81 = i32t* %78 + i32t* %82;
                        i32t %83 = *(i32t* %81);
                        u8t* %84 = u8s* %18 + csu8 80;
                        i32t* %85 = u8t* %84 as *ptr;
                        *(i32t* %85) = i32t %83;
                    }
                    goto lb165;
                    lb165:
                }
                goto lb148;
                goto lb148;
                lb147:
                i64t %86 = csu8 35 as i64;
                i64t %88 = i32s %19 as i64;
                i8t %87 = i64t %88 < i64t %86;
                if i8t %87, goto lb222, else goto lb223;
                lb222:
                goto lb148;
                lb223:
                i64t %90 = i32s %19 as i64;
                i8t %89 = i64t %90 > i64t %86;
                if i8t %89, goto lb224, else goto lb225;
                lb224:
                goto lb148;
                lb225:
                goto lb149;
                lb148:
                u8t* %91 = u8s* %18 + csu8 0;
                use u8t* %91;
                u8s* %11 = u8t* %91;
                u8t* %185 = u8s* %11 + csu8 16;
                i64t %186 = *(u8t* %185);
                i64t %187 = i8n 1 as i64;
                i64t %188 = i64t %186 + i64t %187;
                u8t* %189 = u8s* %11 + csu8 16;
                i64t* %190 = u8t* %189 as *ptr;
                *(i64t* %190) = i64t %188;
                u8t* %191 = u8s* %11 + csu8 16;
                i64t %192 = *(u8t* %191);
                u8t* %193 = u8s* %11 + csu8 8;
                u64t %194 = *(u8t* %193);
                i64t %195 = u64t %194 as i64;
                i64t %196 = i64t %192 >= i64t %195;
                if i64t %196, goto lb587, else goto lb589;
                lb587:
                {
                    i32t %197 = i8n -1 as i32;
                    i32t %92 = i32t %197;
                    goto lb551;
                }
                goto lb589;
                lb589:
                u8t* %198 = u8s* %11 + csu8 0;
                i8t* %199 = *(u8t* %198);
                u8t* %200 = u8s* %11 + csu8 16;
                i64t %201 = *(u8t* %200);
                i8t* %202 = i8t* %199;
                u64t %203 = i64t %201 as u64;
                u64t %204 = u64t %203 * csu8 1;
                i8t* %205 = u64t %204 as *ptr;
                i8t* %206 = i8t* %202 + i8t* %205;
                i8t %207 = *(i8t* %206);
                i32t %208 = i8t %207 as i32;
                use i32t %208;
                lb551:
                [unused] i32t %92 = next2__Stream(u8t* %91);
                i32s %19 = i32t %92;
            }
            goto lb138;
            lb140:
        }
    }
    u8g %20 = arr_alloc(csu8 88);
    fn _main()
    {
        {
            u8t* %93 = &(u8g %20);
            u8t* %94 = u8t* %93 + csu8 24;
            i8t* %95 = u8t* %94 as *ptr;
            i8t* %96 = i8t* %95;
            u64t %98 = i8n 0 as u64;
            u64t %97 = u64t %98 * csu8 1;
            i8t* %100 = u64t %97 as *ptr;
            i8t* %99 = i8t* %96 + i8t* %100;
            *(i8t* %99) = i8n 35;
            u8t* %101 = &(u8g %20);
            u8t* %102 = u8t* %101 + csu8 24;
            i8t* %103 = u8t* %102 as *ptr;
            i8t* %104 = i8t* %103;
            u64t %106 = i8n 1 as u64;
            u64t %105 = u64t %106 * csu8 1;
            i8t* %108 = u64t %105 as *ptr;
            i8t* %107 = i8t* %104 + i8t* %108;
            *(i8t* %107) = i8n 48;
            u8t* %109 = &(u8g %20);
            u8t* %110 = u8t* %109 + csu8 32;
            i32t* %111 = u8t* %110 as *ptr;
            i32t %112 = i8n 1 as i32;
            *(i32t* %111) = i32t %112;
            u8t* %113 = &(u8g %20);
            u8t* %114 = u8t* %113 + csu8 40;
            i32t* %115 = u8t* %114 as *ptr;
            i32t* %116 = i32t* %115;
            u64t %118 = i8n 0 as u64;
            u64t %117 = u64t %118 * csu8 4;
            i32t* %120 = u64t %117 as *ptr;
            i32t* %119 = i32t* %116 + i32t* %120;
            i32t %121 = i8n 7 as i32;
            *(i32t* %119) = i32t %121;
            u8t* %122 = &(u8g %20);
            u8t* %123 = u8t* %122 + csu8 80;
            i32t* %124 = u8t* %123 as *ptr;
            i32t %125 = i8n 0 as i32;
            *(i32t* %124) = i32t %125;
            u8t* %126 = &(u8g %20);
            use u8t* %126;
            run3__Machine(u8t* %126);
            u8t* %127 = &(u8g %20);
            u8t* %128 = u8t* %127 + csu8 80;
            i32t %129 = *(u8t* %128);
            use i32t %129;
            __exit(i32t %129);
        }
    }
}
/: