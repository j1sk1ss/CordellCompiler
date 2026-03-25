# Annotations
Annotations are usefull tool in terms of system development. For non-system programmer, annotations are redundant (even the aligns and sections are avaliable by inbuilt keywords). But let's review all aavaliable annotations (at this moment):
- `naked` - Will disable all entry and exit routines in the final assembly code for an annotated function.
- `align` - Will do the same work as it does the 'align' keyword.
- `section` - Will do the same work as it does the 'section' keyword.
- `address` - Will put a function to a specific address.
- `entry` - Set function as an entry point of the code.
- `no_fall` - Generate hidden break statements in cases.
- `not_lazy` - Generate both sides of a logic expression before evaluation.
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

## Some words about annotations
*P.S. The following annotations are primarily intended for systems-level use cases* </br>

### align
A declaration of a primitive or of an array type (an array or a string) can be annotated with the `align`. This annotation will add an additional padding during memory stack allocation.
```cpl
@[align(16)] glob i32 a;
start() {
    @[align(8)] i32 b;
    @[align(8)] i32 c = a;
}
```

By default align set to platform `bitness / 8` (For instance on the `gnu_x86_64` this is 8 bytes).

### section
A function and a global declaration can be placed in a specific section. To perform this, you will need to use the `section` annotation:
```cpl
    @[section(".lis")] glob i32 a;
    @[section(".lis")] function foo() {}
```

**Note 1:** Function prototype doesn't affected by a section. To put the function's code to a section, you need to define the function. </br>
**Note 2:** By default all global/read-only variables and functions are placed in the platform's code section from the configuration. </br>
**Note 3:** Local functions can't be placed in the specific section. They will stay with their parent function in the same section.

### sizeof
`Sizeof` annotation is an annotation, which means it must be used in the next way:
```cpl
i32 a;
i32 b = @[sizeof]a;
```

It won't work with non-variable and type objects. To gather the type's size, you will need to use a temporary variable (at least for now):
```cpl
i32 index;
i64 __tmp;
loop {
    index += @[sizeof]__tmp;
}
```

It supports arrays and strings, but only as a primitive:
```cpl
str msg = "Hello!";
arr a[10, i64];
i32 msg_len = @[sizeof]msg;
i32 a_len = @[sizeof]a / @[sizeof]a[0];
```

### register
Register annotation is similar to C's `register` keyword which links a variable to the specific selected register:
```cpl
#define RAX 0
@[register(RAX)] i32 a = 0; : Will put `0` to the `RAX` register :
```

*P.S.: This annotation accepts the index of a register from mapping table (see related documentation for every supported target), and can link up to infinity variables to one register, which means - you need to pay extra attention here, if you want to play with registers byepassing the register allocator.*

### no_fall
The switch structure is a great tool to solve a problem with multiple cases. But in C/C++/(old)Java, this structure has to be used with the `break` statement. It is neccesary considering the fallthrough from upper cases to lower cases, and can be very annoing if there is only one statement in the case, and we forced to add the `break` to close the case. </br>
Also, the `hot` and `cold` annotations are really sensitive to unclosed cases and functions, which means, we have to add the `break` in `cold` or `hot` cases even if we don't care about fallthrough. </br>
To address this issue, the language supports the `no_fall` annotation that can be applied to switch structures:
```cpl
@[no_fall] switch cond; {
    case X; {}
    default {}
}
```

It will generate a break statement at the end of every case and default option:
```cpl
switch cond; {
    case X; { break; }
    default { break; }
}
```

P.S.: *This is a pure syntax sugar. It doesn't change the behaviour of the switch in general.*

### straight
By default, the `switch` structure is translated to a binary search block, which in some edge cases can increase the execution time and the final code size. To change the generation from binary search approach to a linear search (generate several `if-elseif-else` statements with direct check with case statements) you can use this annotation.
```cpl
@[straight] switch cond; {
    case X; {}
    default {}
}
```

It will be considered same as this code:
```cpl
if code == X; {
}
else {
}
```
