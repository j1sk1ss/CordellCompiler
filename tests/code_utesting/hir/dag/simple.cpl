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
  lb2 [label="STORE \n dupl: 1, base:  \n 40769291707432169"];
  lb9 [label="STORE \n dupl: 1, base:  \n 4239922092971889756"];
  lb4 [label="STORE \n dupl: 1, base:  \n 12061730331816600223"];
  lb6 [label="STORE \n dupl: 1, base:  \n 2528991139489650554"];
  lb8 [label="iADD \n dupl: 1, base:  \n 1144919062040370692"];
  lb5 [label="iADD \n dupl: 2, base:  \n 17414091545193712901"];
  lb0 [label=" \n dupl: 1, base:  \n 2958715913803752280"];
  lb3 [label="HIR_TI32 \n dupl: 1, base:  \n 12693608025852425247"];
  lb1 [label="HIR_TI32 \n dupl: 1, base:  \n 17847957473366447952"];
  lb2 -> lb1 [label="farg"];
  lb2 -> lb5 [style=dashed, color=gray, label="user"];
  lb9 -> lb8 [label="farg"];
  lb4 -> lb3 [label="farg"];
  lb4 -> lb5 [style=dashed, color=gray, label="user"];
  lb6 -> lb5 [label="farg"];
  lb6 -> lb8 [style=dashed, color=gray, label="user"];
  lb8 -> lb6 [label="farg"];
  lb8 -> lb5 [label="farg"];
  lb8 -> lb9 [style=dashed, color=gray, label="user"];
  lb5 -> lb2 [label="farg"];
  lb5 -> lb4 [label="farg"];
  lb5 -> lb8 [style=dashed, color=gray, label="user"];
  lb5 -> lb6 [style=dashed, color=gray, label="user"];
  lb0 -> lb1 [style=dashed, color=gray, label="user"];
  lb0 -> lb3 [style=dashed, color=gray, label="user"];
  lb3 -> lb0 [label="farg"];
  lb3 -> lb4 [style=dashed, color=gray, label="user"];
  lb1 -> lb0 [label="farg"];
  lb1 -> lb2 [style=dashed, color=gray, label="user"];
}
==================
/: