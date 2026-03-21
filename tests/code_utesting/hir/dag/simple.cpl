start() {
    i32 a = 10;
    i32 b = 10;
    i32 c = a + b;
    i32 d = a + b + c;
}

: OUTPUT
{X}
digraph DAG {
  rankdir=TB;
  node [shape=box, style=filled, fillcolor=lightgray];
  {lb2 [label="STORE \n dupl: 2, base: i32s %9 \n {X}"];}
  {lb9 [label="STORE \n dupl: 1, base: i32s %12 \n {X}"];}
  {lb6 [label="STORE \n dupl: 1, base: i32s %11 \n {X}"];}
  {lb8 [label="iADD \n dupl: 1, base: i32t %8 \n {X}"];}
  {lb1 [label="HIR_TI32 \n dupl: 2, base: i32t %4 \n {X}"];}
  {lb5 [label="iADD \n dupl: 2, base: i32t %6 \n {X}"];}
  {lb0 [label=" \n dupl: 1, base: num? 10 \n {X}"];}
  {lb2 -> lb1 [label="farg"];}
  {lb2 -> lb5 [style=dashed, color=gray, label="user"];}
  {lb9 -> lb8 [label="farg"];}
  {lb6 -> lb5 [label="farg"];}
  {lb6 -> lb8 [style=dashed, color=gray, label="user"];}
  {lb8 -> lb5 [label="farg"];}
  {lb8 -> lb6 [label="farg"];}
  {lb8 -> lb9 [style=dashed, color=gray, label="user"];}
  {lb1 -> lb0 [label="farg"];}
  {lb1 -> lb2 [style=dashed, color=gray, label="user"];}
  {lb5 -> lb2 [label="farg"];}
  {lb5 -> lb6 [style=dashed, color=gray, label="user"];}
  {lb5 -> lb8 [style=dashed, color=gray, label="user"];}
  {lb0 -> lb1 [style=dashed, color=gray, label="user"];}
}
{X}
: