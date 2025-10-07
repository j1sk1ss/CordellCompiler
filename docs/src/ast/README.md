# Tokenization part
The tokenization part is responsible for splitting the input byte sequence into basic tokens. This module ignores all whitespace and separator symbols (such as newlines and tabs). It also classifies each token into one of the basic types: `number`, `string`, `delimiter`, `comma`, or `dot`.
![tokenization](../../media/tokenization.png)

## Example of tokenized code
Code above will produce next list of tokens.
```
line=1, type=1, data=[{], 
line=1, type=2, data=[function], 
line=1, type=2, data=[sum], 
line=1, type=1, data=[(], 
line=1, type=2, data=[i32], 
line=1, type=2, data=[a], 
line=1, type=7, data=[,], 
line=1, type=2, data=[i32], 
line=1, type=2, data=[b], 
line=1, type=1, data=[)], 
line=1, type=0, data=[=>], 
<...>
line=16, type=1, data=[}], 
line=17, type=2, data=[i32], 
line=17, type=2, data=[l], 
line=17, type=0, data=[=], 
line=17, type=2, data=[ref], 
line=17, type=2, data=[f], 
line=18, type=6, data=[;], 
line=18, type=2, data=[exit], 
line=18, type=2, data=[dref], 
line=18, type=2, data=[l], 
line=19, type=6, data=[;], 
line=20, type=1, data=[}]
```

# Markup part
The markup stage is the second part of tokenization, but it is separated from the tokenizer in this compiler due to a different design approach. It operates only on the list of tokens and includes support for scopes. The main idea is to perform basic semantic markup of variables — for example, if we declare a variable `i32 a`, all occurrences of `a` within the corresponding scope can be marked as having the `i32` type.
![markup](../../media/markup.png)

## Example of markup result
```
line=1, type=12, data=[{], 
line=1, type=44, data=[function], 
line=1, type=45, data=[sum], 
line=1, type=10, data=[(], 
line=1, type=24, data=[i32], 
line=1, type=79, data=[a], 
line=1, type=7, data=[,], 
line=1, type=24, data=[i32], 
line=1, type=79, data=[b], 
line=1, type=11, data=[)], 
line=1, type=39, data=[=>],
<...>
line=13, type=6, data=[;], 
line=14, type=12, data=[{], 
line=14, type=38, data=[exit], 
glob line=14, type=3, data=[1], 
line=15, type=6, data=[;], 
line=16, type=13, data=[}], 
line=17, type=24, data=[i32], 
line=17, type=79, data=[l], 
line=17, type=62, data=[=], 
line=17, type=79, data=[f], ref 
line=18, type=6, data=[;], 
line=18, type=38, data=[exit], 
line=18, type=79, data=[l], dref 
line=19, type=6, data=[;], 
line=20, type=13, data=[}]
```

# AST part
Next, we need to parse this sequence of marked tokens to construct an `AST` (Abstract Syntax Tree). There are many approaches to achieve this — for example, `LL` parsing, `LR` parsing, or even `hybrid` techniques that combine `LL` and `LR`. A more complete list of parser types can be found [here](https://www.geeksforgeeks.org/compiler-design/types-of-parsers-in-compiler-design/) or in related compiler design books.
![markup](../../media/ast.png)

## Example of AST
```
[ block ]
   { scope, id=1 }
      [function] (t=44, v_id=0, s_id=0)
         [sum] (t=45, v_id=0, s_id=0)
            [i32] (t=24, v_id=0, s_id=0)
         { scope, id=2 }
            [i32] (t=24, v_id=0, s_id=0)
               [a] (t=79, v_id=0, s_id=2)
            [i32] (t=24, v_id=0, s_id=0)
               [b] (t=79, v_id=1, s_id=2)
            { scope, id=3 }
               [arr] (t=32, v_id=0, s_id=0)
                  [c] (t=87, v_id=2, s_id=3)
                  [2] (t=3, v_id=0, s_id=0, glob)
                  [i32] (t=24, v_id=0, s_id=0)
                  [a] (t=79, v_id=0, s_id=2)
                  [b] (t=79, v_id=1, s_id=2)
               [return] (t=37, v_id=0, s_id=3)
                  [+] (t=53, v_id=0, s_id=0)
                     [c] (t=87, v_id=2, s_id=3)
                        [0] (t=3, v_id=0, s_id=0, glob)
                     [c] (t=87, v_id=2, s_id=3)
                        [1] (t=3, v_id=0, s_id=0, glob)
      [start] (t=36, v_id=1, s_id=0)
         [i64] (t=23, v_id=0, s_id=0)
            [argc] (t=78, v_id=3, s_id=1)
         [u64] (t=27, ptr, v_id=0, s_id=0)
            [argv] (t=82, ptr, v_id=4, s_id=1)
         [ block ]
            { scope, id=4 }
               [i32] (t=24, v_id=0, s_id=0)
                  [a] (t=79, v_id=5, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [b] (t=79, v_id=6, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [c] (t=79, v_id=7, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [d] (t=79, v_id=8, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [k] (t=79, v_id=9, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [f] (t=79, v_id=10, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [if] (t=51, v_id=0, s_id=4)
                  [>] (t=67, v_id=0, s_id=0)
                     [sum] (t=46, v_id=0, s_id=0)
                        [a] (t=79, v_id=5, s_id=4)
                        [b] (t=79, v_id=6, s_id=4)
                     [+] (t=53, v_id=0, s_id=0)
                        [+] (t=53, v_id=0, s_id=0)
                           [+] (t=53, v_id=0, s_id=0)
                              [+] (t=53, v_id=0, s_id=0)
                                 [*] (t=55, v_id=0, s_id=0)
                                    [a] (t=79, v_id=5, s_id=4)
                                    [b] (t=79, v_id=6, s_id=4)
                                 [c] (t=79, v_id=7, s_id=4)
                              [d] (t=79, v_id=8, s_id=4)
                           [k] (t=79, v_id=9, s_id=4)
                        [f] (t=79, v_id=10, s_id=4)
                  { scope, id=5 }
                     [exit] (t=38, v_id=0, s_id=5)
                        [1] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [l] (t=79, v_id=11, s_id=4)
                  [f] (t=79, ref, v_id=10, s_id=4)
               [exit] (t=38, v_id=0, s_id=4)
                  [l] (t=79, dref, v_id=11, s_id=4)
```

## AST optimization
Now we have a correct `AST` representation of the input code. Before proceeding further, we can optionally perform some optimizations at this stage. We will not spend much time here and will only cover a few examples. Note that `AST-level` optimizations are mostly redundant in this project and are included primarily for learning purposes.
- Condition unrolling: If we have an `if` statement with a constant condition, such as `if 1 { ... }`, or similar constructs with `while` or `switch`, we can unroll them by removing the condition and keeping only the scope that will always execute.
- Dead function elimination: This simple technique removes all unused functions.
- Dead scope elimination: If a scope does not affect the environment, it can be removed.
