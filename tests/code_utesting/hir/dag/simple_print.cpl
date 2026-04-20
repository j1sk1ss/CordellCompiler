function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }

    return l;
}

start(i32 argc, ptr ptr i8 argv) {
    str msg = "Hello, World!\n";
    syscall(0x2000004, 1, ref msg, strlen(ref msg));
}

:/ OUTPUT
==== DAG DUMP ====
digraph DAG {
  rankdir=TB;
  node [shape=box, style=filled, fillcolor=lightgray];
  lb14 [label="STORE \n dupl: 1, base: i8s* %19 \n 13178888241110871610"];
  lb12 [label="STORE \n dupl: 1, base: i32s %18 \n 923226199796061841"];
  lb0 [label="FARGLD \n dupl: 1, base: i8s* %14 \n 2692132534544940458"];
  lb2 [label="HIR_TI32 \n dupl: 1, base: i32t %5 \n 13856775729930616717"];
  lb17 [label=" \n dupl: 1, base: strs %4 \n 13232826042171484514"];
  lb15 [label="STARGLD \n dupl: 1, base: i32s %20 \n 1060890402967885696"];
  lb19 [label="REF \n dupl: 1, base: i8t* %12 \n 9409173524406525006"];
  lb13 [label="iADD \n dupl: 1, base: i8t* %9 \n 5356068606571116968"];
  lb8 [label="PHI \n dupl: 1, base: i32s %17 \n 3471015483911651534"];
  lb5 [label="HIR_TI32 \n dupl: 1, base: i32t %8 \n 4674520331701620049"];
  lb7 [label="PHI \n dupl: 1, base: u64s* %16 \n 15673735951680481742"];
  lb9 [label=" \n dupl: 1, base: i8s* %16 \n 15673735951685877942"];
  lb10 [label="GDREF \n dupl: 1, base: i8t %6 \n 13462653517696327882"];
  lb16 [label="STARGLD \n dupl: 1, base: i8s** %21 \n 15434875531564022024"];
  lb1 [label=" \n dupl: 1, base: i8n 0 \n 11054685136650038474"];
  lb3 [label="STORE \n dupl: 1, base: i32s %15 \n 2211828852213350205"];
  lb18 [label="REF \n dupl: 1, base: i8t* %11 \n 16794452045597999227"];
  lb4 [label=" \n dupl: 1, base: i8n 1 \n 401222794553097464"];
  lb6 [label="HIR_TPTR \n dupl: 1, base: i8t* %10 \n 15696540768297557553"];
  lb20 [label="HIR_STORE_FCLL \n dupl: 1, base: i32t %13 \n 15923203626596397411"];
  lb11 [label="iADD \n dupl: 1, base: i32t %7 \n 13851573159292410382"];
  lb14 -> lb13 [label="farg"];
  lb12 -> lb11 [label="farg"];
  lb2 -> lb1 [label="farg"];
  lb2 -> lb3 [style=dashed, color=gray, label="user"];
  lb17 -> lb18 [style=dashed, color=gray, label="user"];
  lb17 -> lb19 [style=dashed, color=gray, label="user"];
  lb19 -> lb17 [label="farg"];
  lb13 -> lb6 [label="farg"];
  lb13 -> lb9 [label="farg"];
  lb13 -> lb14 [style=dashed, color=gray, label="user"];
  lb8 -> lb11 [style=dashed, color=gray, label="user"];
  lb5 -> lb4 [label="farg"];
  lb5 -> lb11 [style=dashed, color=gray, label="user"];
  lb9 -> lb10 [style=dashed, color=gray, label="user"];
  lb9 -> lb13 [style=dashed, color=gray, label="user"];
  lb10 -> lb9 [label="farg"];
  lb1 -> lb2 [style=dashed, color=gray, label="user"];
  lb3 -> lb2 [label="farg"];
  lb18 -> lb17 [label="farg"];
  lb4 -> lb6 [style=dashed, color=gray, label="user"];
  lb4 -> lb5 [style=dashed, color=gray, label="user"];
  lb6 -> lb4 [label="farg"];
  lb6 -> lb13 [style=dashed, color=gray, label="user"];
  lb11 -> lb8 [label="farg"];
  lb11 -> lb5 [label="farg"];
  lb11 -> lb12 [style=dashed, color=gray, label="user"];
}
==================
/: