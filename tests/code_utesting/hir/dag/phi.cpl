: BUG :
start() {
    i32 a;
    if 1; {
        a = 12 as i32;
    }
    else {
        a = 12 as i32;
    }
    i32 b = a;
}

:/ OUTPUT
==== DAG DUMP ====
digraph DAG {
  rankdir=TB;
  node [shape=box, style=filled, fillcolor=lightgray];
  lb3 [label="HIR_TI32 \n dupl: 1, base:  \n 9327308231932683998"];
  lb0 [label=" \n dupl: 1, base:  \n 195216416947941798"];
  lb2 [label="STORE \n dupl: 1, base:  \n 4866891423330454903"];
  lb1 [label="HIR_TI32 \n dupl: 1, base:  \n 17720108715051551582"];
  lb5 [label="PHI \n dupl: 1, base:  \n 13168350750249366432"];
  lb6 [label="STORE \n dupl: 1, base:  \n 11072115748851427508"];
  lb4 [label="STORE \n dupl: 1, base:  \n 24241213674751227"];
  lb3 -> lb0 [label="farg"];
  lb3 -> lb4 [style=dashed, color=gray, label="user"];
  lb0 -> lb1 [style=dashed, color=gray, label="user"];
  lb0 -> lb3 [style=dashed, color=gray, label="user"];
  lb2 -> lb1 [label="farg"];
  lb1 -> lb0 [label="farg"];
  lb1 -> lb2 [style=dashed, color=gray, label="user"];
  lb5 -> lb6 [style=dashed, color=gray, label="user"];
  lb6 -> lb5 [label="farg"];
  lb4 -> lb3 [label="farg"];
}
==================
/: