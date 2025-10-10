# DAG part
With the complete `SSA` form, we can move on to the first optional optimizations. The first one requires building a `DAG` (Directed Acyclic Graph) representation of the code. In short, a `DAG` shows how every value in the program is derived. In other words, this graph illustrates how each variable obtains its value (with some exceptions for `arrays` and `strings`).
![base_dag](../../media/base_DAG.png)

Then, when we build the "basic" DAG, we check and merge all nodes that share the same hash (computed as a hash of their child nodes). If the nodes are identical and the base node is located in a dominating block, we can safely merge them.
![opt_dag](../../media/opt_DAG.png)

The result of using the DAG is optimized code with Common Subexpression Elimination applied.
![res_dag](../../media/res_DAG.png)