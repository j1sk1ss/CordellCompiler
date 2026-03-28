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

: OUTPUT
{X}
digraph DAG {
  rankdir=TB;
  node [shape=box, style=filled, fillcolor=lightgray];
  lb3 [label="HIR_TI32 \n dupl: 1, base: i32t %3 \n {X}"];
  lb2 [label="STORE \n dupl: 1, base: i32s %7 \n {X}"];
  lb1 [label="HIR_TI32 \n dupl: 1, base: i32t %2 \n {X}"];
  lb0 [label=" \n dupl: 1, base: num? 12 \n {X}"];
  lb5 [label="PHI \n dupl: 1, base: i32s %5 \n {X}"];
  lb6 [label="STORE \n dupl: 1, base: i32s %6 \n {X}"];
  lb4 [label="STORE \n dupl: 1, base: i32s %4 \n {X}"];
  lb3 -> lb0 [label="farg"];
  lb3 -> lb4 [style=dashed, color=gray, label="user"];
  lb2 -> lb1 [label="farg"];
  lb1 -> lb0 [label="farg"];
  lb1 -> lb2 [style=dashed, color=gray, label="user"];
  lb0 -> lb1 [style=dashed, color=gray, label="user"];
  lb0 -> lb3 [style=dashed, color=gray, label="user"];
  lb5 -> lb6 [style=dashed, color=gray, label="user"];
  lb6 -> lb5 [label="farg"];
  lb4 -> lb3 [label="farg"];
}
{X}
: