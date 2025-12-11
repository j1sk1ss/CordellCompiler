# Summary
CordellCompiler' peephole optimization bases on patterns. These patterns can be produced with special domain-specific programming language `PTRN`. The main idea is to implement a complex, easy-to-use tool for a peephole pattern generation.

# Syntax
## Formatting
- `;` - Is a comment symbol.
- Patters separate with a `/` symbol.

## Basics
`PTRN` DSL supports a simple pattern's syntax. For a pattern construction user has avaliable pattern commands such as `mov`, `add`, `sub`, `mul`, `div`, `mod`, `shl`, `shr`, `and`, ... etc. (The full list can be found in the `.\commands.json (mapping)` file). With these commands we can construct our' first pattern.

```ptrn
mov REGISTER, SAME_REGISTER
->
DELETE_LINE
/
```

Now I want to speak about objects. The `object` - is a basic LIR subject that can be represented as a basic number, a register or a memory location. For instance the register object can be typed as a `reg <REG>` or a `areg_N` keyword. Here `N` is an uniqe identifier of an abstract variable, and `<REG>` is a register name such as `RAX`, `RBX`, `SIL`, etc. Here is the full list of objects:
- `reg <REG>` - Example: RAX, RBX, AX, AL, etc.
- `areg_N` - Example: RAX, RBX, AX, AL, etc.
- `const <VAL>` - Example: 10, 0b100, 0x80, etc.
- `aconst_N` - Example: 10, 0b100, 0x80, etc.
- `mem_N` - Example: [rbp - X], [0x80], [...], etc.
- `obj` - Any object. Example: RAX, 10, [rbp - X], etc.

The reason of identifier existance is complex patterns. If we want track "variables", we need to distinguish them. For instance code below:
```ptrn
mov areg_N, areg_N
mov areg_N, areg_N
->
mov areg_N, areg_N
/
```

doesn't make any sense. But with the identifier usage:
```ptrn
mov areg_1, areg_2
mov areg_2, areg_1
->
mov areg_1, areg_2
/
```

we now can say, that this is a valid optimization pattern.

## Conditions [if:X:Y]
`PTRN` DSL supports conditions for the pattern matching. The syntax is simple:
```ptrn
mov areg_1, areg_2 [if:equals]
->
delete
```

The code above will delete all `mov` lines that include two equals arguments. For this purpose we add a `[if:X:Y]` command that will extend the final pattern with an additional check.
- `X` - Not required. Target argument (`arg1` / `arg2`).
- `Y` - Required. Operation type (`mod2`, `power2`, `zero`, `one`, `equals`).

## Actions [do:X:Y]
Additionally, `PTRN` supports a `do` command that will tell generator to extend the final pattern with some action. The syntax of this command quite similar with the `if` command. The main difference here is a different list of avaliable parameters: (`sqrt2`). Let's consider next example:
```ptrn
add areg_1, aconst_2 [if:arg2:mod2]
->
shr areg_1, aconst_2 [do:arg2:sqrt2]
/
```

If we've met an `add` command with the `mod2` (`X % 2 == 0`) second argument, we convert this line to `shr` and replace constant with the `sqrt2`.  

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
add areg_1, aconst_2 [if:arg2:mod2] ; If aconst_1 is a module of 2, we proceed further.
->
shr areg_1, aconst_2 [act:arg2:sqrt2] ; Past here a result of the sqrt2(aconst_1) function.
/
```

4. `Delete` is pretty easy to use. Just put this command below the `->` identifier, and it will remove all lines that are satisfies to the choosen patter:
```ptrn
add obj, const 0
->
delete ; Delete the redundant line
/
```

# How to use it?
Just use the next command:
```bash
python3 main.py <xxx.ptrn>
```

It will produces a `peephole_geenrated.c` file with all essentials.
