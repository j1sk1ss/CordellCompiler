# CFG part
With `3AC`, we can move on to `CFG` (Control Flow Graph) creation. There are several ways to split `3AC` into basic blocks. One approach is using `leaders`, while another is to create a block for every command. The second approach is straightforward — each `3AC` instruction becomes its own block. The `leaders` approach, described in the *Dragon Book*, defines three rules for identifying the start of a block:

- The first instruction in a function.
- The target of a JMP instruction.
- The instruction immediately following a JMP.

In this compiler, both approaches are implemented, but for the following explanations, we will use the approach of creating a block for every command.

![markup](../../media/CFG.png)

## Example of CFG
![markup](../../media/CFG_example.png)

# Dominant calculation
With the `CFG`, we can determine the dominators of each block. In simple terms, a dominator of a block `Y` is a block `X` that appears on every path from the entry block to `Y`. For example, the following figure illustrates how this works:
![dominators](../../media/dominators.png)

## Strict dominance
Strict dominance tells us which block strictly dominates another. A block `X` strictly dominates block `Y` if `X` dominates `Y` and `X` != `Y`. Why do we need this? The basic dominance relation marks all blocks that dominate a given block, but later analyses often require only the closest one. A block `X` is said to be the immediate dominator of `Y` if `X` strictly dominates `Y`, and there is no other block `Z` such that `Z` strictly dominates `Y` and is itself strictly dominated by `X`.
![sdom](../../media/strict_dominance.png)

## Dominance frontier
The dominance frontier of a block `X` is the set of blocks where the dominance of `X` ends. More precisely, it represents all the blocks that are partially influenced by `X`: `X` dominates at least one of their predecessors, but does not dominate the block itself. In other words, it marks the boundary where control flow paths from inside `X’s` dominance region meet paths coming from outside.
![fdom](../../media/dominance_frontier.png)