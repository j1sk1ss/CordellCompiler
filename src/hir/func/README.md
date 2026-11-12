# func

For anyone who will update the compiler with a new optimisations:

Place here any code that should work with HIR CFG **before** SSA form.

**NOTE:** ! BEFORE ! SSA form.

## Special cases

Yes - there can be a module that works with CFG after it becames SSA, and this is a special case. But in general, try to avoid this practice. 
