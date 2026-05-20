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
  lb0 [label=" \n dupl: 1, base:  \n 5878661971377911259"];
  lb3 [label="HIR_TI32 \n dupl: 1, base:  \n 17317072603312265717"];
  lb5 [label="PHI \n dupl: 1, base:  \n 13168350750250561155"];
  lb4 [label="STORE \n dupl: 1, base:  \n 12787521593104867869"];
  lb2 [label="STORE \n dupl: 1, base:  \n 12874229293024032403"];
  lb6 [label="STORE \n dupl: 1, base:  \n 11072115748941720304"];
  lb1 [label="HIR_TI32 \n dupl: 1, base:  \n 12017240562302442869"];
  lb0 -> lb1 [style=dashed, color=gray, label="user"];
  lb0 -> lb3 [style=dashed, color=gray, label="user"];
  lb3 -> lb0 [label="farg"];
  lb3 -> lb4 [style=dashed, color=gray, label="user"];
  lb5 -> lb6 [style=dashed, color=gray, label="user"];
  lb4 -> lb3 [label="farg"];
  lb2 -> lb1 [label="farg"];
  lb6 -> lb5 [label="farg"];
  lb1 -> lb0 [label="farg"];
  lb1 -> lb2 [style=dashed, color=gray, label="user"];
}
==================
/: