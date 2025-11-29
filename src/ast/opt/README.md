# AST opt
- `condunroll.c` - a module for unrolling certain conditional constructs like `if`, `while` and `switch`. The main idea of this module is to eliminate all unnecessary conditional checks. For instance:
1) If statements:
```cpl
if 1; {}
else {}
: => :
{} : Now became regular scope block without if statement :
```

2) Switch statements:
```cpl
switch 1; {
    case 1; {}
    default {}
}
: => :
{} : Now became regular scope block without switch statement :
```

3) While statements:
```cpl
while 0; {}
: => :
: ... While statement and it's body was deleted : 
```

- `deadscope.c` - Simple script for removing all `AST-scopes` that doesn't has any side-effects. Side-effects are:
1) Function calls
2) Outer-scope variable write
3) Any interruption (`lis`, `return`, `exit`)

Also, this AST-optimization tracks `all` existing scopes (including `switch cases`, `while` and `if` scopes). This means that it can delete even separated `switch` statement cases or entier `if`/`while` bodies. 
