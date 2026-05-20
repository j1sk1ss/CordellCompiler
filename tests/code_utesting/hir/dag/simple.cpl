: BUG :
start() {
    i32 a = 10 as i32;
    i32 b = 10 as i32;
    i32 c = a + b;
    i32 d = a + b + c;
}

:/ OUTPUT
==== DAG DUMP ====
digraph DAG {
  rankdir=TB;
  node [shape=box, style=filled, fillcolor=lightgray];
  lb3 [label="HIR_TI32 \n dupl: 1, base:  \n 8324906574768552945"];
  lb4 [label="STORE \n dupl: 1, base:  \n 6450169523965529022"];
  lb1 [label="HIR_TI32 \n dupl: 1, base:  \n 5264938960744257946"];
  lb2 [label="STORE \n dupl: 1, base:  \n 14119043943294948788"];
  lb0 [label=" \n dupl: 1, base:  \n 16694628038337109086"];
  lb9 [label="STORE \n dupl: 1, base:  \n 1404796809552086635"];
  lb8 [label="iADD \n dupl: 1, base:  \n 2826781958096959376"];
  lb6 [label="STORE \n dupl: 1, base:  \n 6607791618419226080"];
  lb5 [label="iADD \n dupl: 2, base:  \n 5332973327414059158"];
  lb3 -> lb0 [label="farg"];
  lb3 -> lb4 [style=dashed, color=gray, label="user"];
  lb4 -> lb3 [label="farg"];
  lb4 -> lb5 [style=dashed, color=gray, label="user"];
  lb1 -> lb0 [label="farg"];
  lb1 -> lb2 [style=dashed, color=gray, label="user"];
  lb2 -> lb1 [label="farg"];
  lb2 -> lb5 [style=dashed, color=gray, label="user"];
  lb0 -> lb1 [style=dashed, color=gray, label="user"];
  lb0 -> lb3 [style=dashed, color=gray, label="user"];
  lb9 -> lb8 [label="farg"];
  lb8 -> lb5 [label="farg"];
  lb8 -> lb6 [label="farg"];
  lb8 -> lb9 [style=dashed, color=gray, label="user"];
  lb6 -> lb5 [label="farg"];
  lb6 -> lb8 [style=dashed, color=gray, label="user"];
  lb5 -> lb2 [label="farg"];
  lb5 -> lb4 [label="farg"];
  lb5 -> lb8 [style=dashed, color=gray, label="user"];
  lb5 -> lb6 [style=dashed, color=gray, label="user"];
}
==================
/: