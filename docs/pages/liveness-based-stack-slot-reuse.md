# Liveness-based stack slot reuse
CPL performs a compile-time analysis of variable lifetimes to reduce stack usage. The compiler tracks when a local variable is still needed (“live”) and when it is no longer used (“dead”). Once a variable is proven dead, its stack slot may be reused for another variable declared later.

## Idea
- Every local variable has a lifetime interval: from its initialization until its last use.
- A pointer/reference created with ref is treated as a use that may extend the lifetime of the referenced value.
- The compiler may reuse stack memory only when it can prove that the old value and all its references are no longer used.

Think of it as stack coloring or liveness-based stack slot allocation: two variables that are never live at the same time can share the same stack memory

## What this is NOT
Not a memory-safety system

- CPL doesn't prevent all dangling pointers or aliasing issues. The analysis is used to avoid incorrect stack reuse, not to “make pointers safe”.
- Not Rust ownership / borrowing.
- Rust enforces rules like “one mutable reference or many immutable ones” and prevents data races / use-after-free in safe code.

CPL does not enforce these restrictions. CPL only ensures that the compiler does not intentionally reuse a stack slot while it is still provably needed.

## Example
```cpl
{
    start() {
        i32 a = 0; : <= Allocate 8 bytes :
        ptr i32 p; : <= Allocate 8 bytes :
        if 1; {
            p = ref a; : <= "p" becomes a new owner of "a" :
        }

        i32 c = 0; : <= "p" is still alive, so "a" is not reusable yet :
        exit p;
    }
}
```
