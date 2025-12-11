# Summary
CordellCompiler' peephole optimization bases on patterns. These patterns can be produced with special domain-specific programming language `PTRN`. The main idea is to implement a complex, easy-to-use tool for a peephole pattern generation.

# Formatting
- `;` - Is a comment symbol.
- Patters separate with a `/` symbol.

## Basics
Supported commands:
- mov
- add
- sub
- mul
- shl
- shr
- lea
- cmp
- test
- xor
- or
- and
- delete (Reserved command)

Supported objects:
- const `val`
- aconst (Any constant)
- reg `reg` (rax/rdx/rbx/.../r12)
- areg (Any register)
- mem
- obj (Any object)

# Examples
1. For instance, let's consider the next pattern:
```ptrn
mov areg_1, const 0
->
xor areg_1, areg_1
/
```

Here we replace the mov operation with the xor operation. According to docs, xor operation is more optimized for 'zeroing' purpose.

2. PTRN supports multiple lines as a single pattern. This allows us to build a complex logic:
```ptrn
mov areg_1, areg_2
mov areg_2, areg_1
->
mov areg_1, areg_2
/
```

3. PTRN also supports simple math and basic if-else statements. This makes the next code a valid construction:
```ptrn
add areg_1, aconst_1 [if:mod2] ; If aconst_1 is a module of 2, we proceed further.
->
shr areg_1, aconst_1 [act:sqrt2] ; Past here a result of the sqrt2(aconst_1) function.
/
```

4. `Delete` is pretty easy to use. Just put this command below the `->` identifier, and it will remove all lines that are satisfies to the choosen patter:
```ptrn
add obj, const 0
->
delete ; Delete the redundant line
/
```
