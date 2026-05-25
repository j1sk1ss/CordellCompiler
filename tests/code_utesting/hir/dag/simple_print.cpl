function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }

    return l;
}

start(i32 argc, ptr ptr i8 argv) {
    arr msg[0, i8] = "Hello, World!\n";
    syscall(0x2000004, 1, ref msg, strlen(ref msg));
}

:/ OUTPUT
==== DAG DUMP ====
digraph DAG {
  rankdir=TB;
  node [shape=box, style=filled, fillcolor=lightgray];
  lb19 [label="HIR_STORE_FCLL \n dupl: 1, base:  \n 15923203626597846412"];
  lb10 [label="iADD \n dupl: 1, base:  \n 14603363527660092500"];
  lb17 [label="REF \n dupl: 1, base:  \n 16794452045535473284"];
  lb7 [label="PHI \n dupl: 1, base:  \n 15673735951687054741"];
  lb16 [label=" \n dupl: 1, base:  \n 13232826042170293315"];
  lb6 [label="HIR_TPTR \n dupl: 1, base:  \n 7761430001427040637"];
  lb3 [label="STORE \n dupl: 1, base:  \n 16536749750262969493"];
  lb8 [label="PHI \n dupl: 1, base:  \n 3471015483910462957"];
  lb5 [label="HIR_TI32 \n dupl: 1, base:  \n 17401885617234982391"];
  lb4 [label=" \n dupl: 1, base:  \n 10660652250176944150"];
  lb9 [label="GDREF \n dupl: 1, base:  \n 13462653517764785861"];
  lb11 [label="STORE \n dupl: 1, base:  \n 196139352338118908"];
  lb14 [label="STARGLD \n dupl: 1, base:  \n 1060890402971175587"];
  lb18 [label="REF \n dupl: 1, base:  \n 9409173524320957729"];
  lb12 [label="iADD \n dupl: 1, base:  \n 1459278873240778083"];
  lb2 [label="HIR_TI32 \n dupl: 1, base:  \n 17727717839361341678"];
  lb1 [label=" \n dupl: 1, base:  \n 7147978678840089421"];
  lb13 [label="STORE \n dupl: 1, base:  \n 18217268204209805786"];
  lb0 [label="FARGLD \n dupl: 1, base:  \n 2692132534543730825"];
  lb15 [label="STARGLD \n dupl: 1, base:  \n 15434875531563043883"];
  lb10 -> lb5 [label="farg"];
  lb10 -> lb8 [label="farg"];
  lb10 -> lb11 [style=dashed, color=gray, label="user"];
  lb17 -> lb16 [label="farg"];
  lb7 -> lb9 [style=dashed, color=gray, label="user"];
  lb7 -> lb12 [style=dashed, color=gray, label="user"];
  lb16 -> lb18 [style=dashed, color=gray, label="user"];
  lb16 -> lb17 [style=dashed, color=gray, label="user"];
  lb6 -> lb4 [label="farg"];
  lb6 -> lb12 [style=dashed, color=gray, label="user"];
  lb3 -> lb2 [label="farg"];
  lb8 -> lb10 [style=dashed, color=gray, label="user"];
  lb5 -> lb4 [label="farg"];
  lb5 -> lb10 [style=dashed, color=gray, label="user"];
  lb4 -> lb6 [style=dashed, color=gray, label="user"];
  lb4 -> lb5 [style=dashed, color=gray, label="user"];
  lb9 -> lb7 [label="farg"];
  lb11 -> lb10 [label="farg"];
  lb18 -> lb16 [label="farg"];
  lb12 -> lb6 [label="farg"];
  lb12 -> lb7 [label="farg"];
  lb12 -> lb13 [style=dashed, color=gray, label="user"];
  lb2 -> lb1 [label="farg"];
  lb2 -> lb3 [style=dashed, color=gray, label="user"];
  lb1 -> lb2 [style=dashed, color=gray, label="user"];
  lb13 -> lb12 [label="farg"];
}
==================
/: