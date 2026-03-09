# Annotations
Annotations are usefull tool in terms of system development. For non-system programmer, annotations are redundant (even the aligns and sections are avaliable by inbuilt keywords). But let's review all aavaliable annotations (at this moment):
- `naked` - Will disable all entry and exit routines in the final assembly code for an annotated function.
- `align` - Will do the same work as it does the 'align' keyword.
- `section` - Will do the same work as it does the 'section' keyword.
- `address` - Will put a function to a specific address.
- `entry` - Set function as an entry point of the code.
- `no_fall` - Generate hidden break statements in cases.
- `straight` - Generate case choice with linear search approach.
- `counter` - Make a counted loop.
- `hot` and `cold` - Send a branch to the end of a function.
- `register` - Will link a primitive (non-global) variable to a register.
- `sizeof` - Will replace the next variable with its size in bytes (Acts the same as it does the similar keyword from C language).

Annotations can be added for functions, function arguments and declarations:
```cpl
@[align(16)] i32 a;
@[entry]
@[naked]
function main() -> i0;
@[section(".bss")] glob i32 b;
```

As you can see, these annotations copy all functionality of the 'align' and 'section' keywords. Actually, it doesn't mean that these keywords now redundant. They still is a pretty convenient way of block code. For example these code:
```cpl
section (".bss") {
    align (16) {
        glob i32 a;
        glob i32 b;
        glob i32 c;
    }
    function foo();
    function bar();
}
``` 

can be reproduced with annotations:
```cpl
@[section(".bss")] @[align(16)] glob i32 a;
@[section(".bss")] @[align(16)] glob i32 b;
@[section(".bss")] @[align(16)] glob i32 c;
@[section(".bss")] function foo();
@[section(".bss")] function bar();
```

, but obviously, now it doesn't look too clean.

## Some words about annotations
*P.S. If you're not a system programmer, or you don't plan to use these annotations, you can safely skip this block. These annotations (as all annotations) are optional.* </br>

### sizeof
`Sizeof` annotation is an annotation, which means it must be used in the next way:
```cpl
i32 a;
i32 b = @[sizeof]a;
```

It won't work with non-variable and type objects. To gather the type's size, you will need to use a temprorary variable (at least for now):
```cpl
i32 index;
i64 __tmp;
loop {
    index += @[sizeof]__tmp;
}
```

It supports arrays and strings, but only as a primitive (without indexation):
```cpl
str msg = "Hello!";
arr a[10, i64];
i32 msg_len = @[sizeof]msg;
i32 a_len = @[sizeof]a / 8;
```

### register
Register annotation is similar to C's `register` keyword which links a variable to the specific selected register:
```cpl
#define RAX 0
@[register(RAX)] i32 a = 0; : Will put `0` to the `RAX` register :
```

*P.S.: This annotation accepts the index of a register from mapping table (see related documentation for every supported target), and can link up to infinity variables to one register, which means - you need to pay extra attention here, if you want to play with registers byepassing the register allocator.*

### hot and cold
Before our discussion, I want to provide the most canonic example that's even possible here:
```cpl
@[cold] if 1; { : IF1 :
    : something :
}
else { : ELSE1 :
    : something hot :
}
```

, and an exanple with the `switch` usage:
```cpl
@[no_fall]
switch 1; {
    @[cold] case 1; {}
    case 2; {}
    default {}
}
```

Now let's discuss the features and flaws of usage of these annotations. Obviously, it's one of the forms of optimization, in other words, it accelerates the output compiled file. But in other hand, it creates a lot of possible issues if you use it in the wrong way. Let me explain:
- The `cold` and `hot` annotations simply move the branch to the end of a function (That's all, no special operations. It happens even before CFG generation). In terms of CFG and Liveness analysis, considering the label guards, it doesn't change anything. But if we will consider the `IR` of the next code:
```cpl
@[cold] if 1; {
    return 2;
}
else {
}
return 1;
```

it will convert this to the next form:
```cpl
if not 1; then goto lb1; else goto lb2;
lb2: {
} : goto to the return label :
return 1;
lb 1: {
    return 2;
} : goto to the return label :
```

And here you can see a huge problem that can lead to a lot of UBs if it doesn't receive enough attention from a programmer. If we choose to remove the output `return 1;` from the function, the code will become:
```cpl
if not 1; then goto lb1; else goto lb2;
lb2: {
} : goto to the empty space label :
lb 1: {
    return 2;
} : goto to the empty space label :
```

Now the execution from the `lb2` will go straightforward to the `lb1`! </br> 
How to solve it? The compiler supports the `return` and `exit` statements even in functions without any return type, which means, if you're planning to use these annotations, make sure that you have an essential `return <something>;` statement at the function's end.
