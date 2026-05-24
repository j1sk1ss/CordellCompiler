# Containers

To create a simple container you can use the next code as a reference:
```cpl
container a {
    i32 a;
    i32 b;
}
```

This is a simple example of "How to create a container?". By default, this structure uses the same align for every field (max bytness of the target architecture). To change this, you can provide an annotation:
```cpl
@[align(1)]
container a {
    i8 a;
    i16 b;
    i32 c;
} :/ packed, will occupy 7 bytes on the stack /:
```

In a nutshell, `CPL` containers works the same as it do structures in `C`. They serve as a convenient way to store different types in one place. But in difference with `C` structures, they also can have functions:
```cpl
container a {
    function something() {
        return 100;
    }
}

a a;
a.something();
```

Of course `C` structures can have a pointer to a function. But in addition to the existed features, `CPL` containers can use the `self` annotation.
```cpl
container a {
    i32 val;

    @[self]
    function init(ptr a self) -> i0 {
        self.val = 0;
    }
}

a a;
a.init();
a.val; :/ 0 /:
``` 

It is a pure syntax sugar and it doesn't have anything in common with classes from Object Oriented Programming. 'Methods' from containers are default functions. Parser may pass a self pointer in a call, if the function is marked as a 'self' function. </br>
*P.S.:* Self functions **must** have a self argument in the signature. </br>
*P.P.S.:* There is no way of making a constructor or destructor for a container. Treat containers as a modified `C` structure, not a `Rust` structure or `C++` class. To do something you need to do it explicitly.