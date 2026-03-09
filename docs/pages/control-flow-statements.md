# Control flow statements
## if statement
`if` keyword similar to the `C`'s `if` statement. Key change here is the `;` token after the condition. 
```cpl
if <condition>; {
}
else {
}
```

For instance:
```cpl
i32 a = 10;
i32 b = 11;
if a == 12 && b < 12; {
    a = 10;
}
else {
    a = 12;
}
```

Or without a scope symbol:
```cpl
i32 a1 = 12;
i32 a;

if a1 > 10 && a1 <= 20; a = 10;
else if a1 > 20;        a = 20;
else                    a = 0;
```

## while statement
```cpl
while <condition>; {
    <statements>;
}

while <condition>; <statement>;
```

## loop statement
In difference with the `while` statement, the `loop` statement allows to build efficient infinity loops for a purpose. It is a way efficient than a `while 1;` statement given the empty 'condition' body. </br>
**Importand Note:** You *must* insert the `break` keyword somewhere in a body of this statement or use the `counter` annotation. Otherwise it will became an infinity loop.
```cpl
loop {
    <statements>;
}

loop <statement>;
```

The `loop` statement supports the `counter` annotation. This will make a counted loop with the fixed amount of iterations:
```cpl
@[counter(10)] loop {
}
```

**Note:** The `counter` annotation accepts only constants which doesn't allow to create variable loop durations with variables. To do this, you still will need to use the traditional `while` or `loop` with the end `if` statement.

## switch statement
**Note 1:** `X` should be constant value (or a primitive variable that can be `inlined`). </br>
**Note 2:** Similar to C language, the `switch` statement supports the fall 'mechanic'. It implies, that the `case` can ignore the `break` keyword. This will lead to the execution of the next case block.
```cpl
switch cond; {
    case X; {}
    case Y; {
        break;
    }
    default; {
        break;
    }
}
```

**Note 3:** *The switch statement is generated with the usage of a binary search approach. That means, consider this structure over the multiple ifs.* </br>

Switch supports several annotations:
- `no_fall` - No fall annotation will disable the fallthrough:
```cpl
@[no_fall] switch cond; {
    case X; {}
    default {}
}
```
- `straight` - Will shift switch generation from binary search approach to a linear search. In other words, it will generate several `if-elseif-eslse` statements with direct check with case statements:
```cpl
@[straight] switch cond; {
    case X; {}
    default {}
}
```
