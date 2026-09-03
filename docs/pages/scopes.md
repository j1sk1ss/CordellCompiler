# Scopes
## Variables and lifetime
Variables live in their declared scopes. You cannot safely keep pointers to variables after their scope ends. This keeps manual stack management more predictable, because the compiler's stack allocator can reuse dead local storage.
```cpl
start() {
   ptr u64 p;
   {
      arr t[10, i32];
      p = ref t; : <= No warning here, but it is still illegal :
   }             : <= array "t" died here :

   p[0] = 1;     : <= Pointer to allocated but "freed" stack space :
   exit 0;
}
```

**Note 1:** The example above can cause memory corruption rather than a simple segmentation fault. After the scope ends, the pointer may still point to a stack slot that the compiler is allowed to reuse for another local value.

**Note 2:** The compiler tries to kill variables, arrays, and strings outside their scopes even if a pointer to that storage is still used later. CPL currently has no borrow checker or lifetime analysis that would make such code safe.
**Note 3:** In the example above, execution may appear to succeed if later code ignores the "freed" stack space or uses registers for new variables, but the code still has undefined behavior.

## Visibility rules
Variables from outer scopes are visible in the current scope and in nested scopes.
```cpl
{
   {
      i32 a = 10; : <= Does not see any variables :
   }

   i64 b = 10; : <= Does not see any variables :

   {
      i8 c = 9; : <= Sees the "b" variable :

      {
         f32 a = 10.0; : <= Sees the "b" and "c" variables :
      }

      i8 a = 0; : <= Sees the "b" and "c" variables :
   }
}
```
