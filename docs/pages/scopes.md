# Scopes
## Variables and lifetime
Variables live in their declared scopes. You cannot point to variables from an outer scope. This makes the manual program stack managment a way easier given the determined behavior of the stack allocator in this compiler. 
```cpl
start() {
   ptr u64 p;
   {
      arr t[10; i32];
      p = ref t; : <= No warning here, but it still illegal :
   }             : <= array "t" died here :

   p[0] = 1;     : <= Pointer to allocated but 'freed' stack :
   exit 0;
}
```

**Note 1:** Example above will cause a memory corruption error instead of the `SF` due the stack allocation method in CPL. (The pointer after the scope is pointing to the already allocated area. However, the compiler can use this area for the another array / variable, etc.). </br>
**Note 2:** This compiler tries to `kill` all variables / arrays / strings outside their scopes, even if they are used as a referenced value somewhere else in the further code. </br>
**Note 3:** In the example above, execution may be success (further code can ignore the 'freed' space in the stack and prefer the register placement for new variables), but it is still the Undefined Behavior. 

## Visibility rules
Outer variables can be seen by current and nested scopes.
```cpl
{
   {
      i32 a = 10; : <= Doesn't see any variables :
   }

   i64 b = 10; : <= Doesn't see any variables :

   {
      i8 c = 9; : <= See the "b" variable :

      {
         f32 a = 10.0; : <= See the "b" and the "c" variables :
      }

      i8 a = 0; : <= See the "b" and the "c" variables :
   }
}
```
