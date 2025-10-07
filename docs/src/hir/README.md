# HIR part
Now we need to convert our `AST` into a simpler representation. A common approach here is to convert the `AST` into `Three-Address Code` (3AC).
![markup](../../media/HIR.png)

## HIR optimization
- Constant folding
- HIR Peephole optimization

## Example of HIR
```
{
    fn sum(i32 a, i32 b) -> i32
    {
        alloc i32s a0;
        load_arg(i32s a0);
        alloc i32s b1;
        load_arg(i32s b1);
        {
            prm_st(i32s a0);
            prm_st(i32s b1);
            alloc arrs c2, size: n2;
            i32t tmp12 = arrs c2[n0];
            i32t tmp13 = arrs c2[n1];
            i32t tmp14 = i32t tmp12 + i32t tmp13;
            return i32t tmp14;
        }
    }
    
    start {
        alloc i64s argc3;
        load_starg(i64s argc3);
        alloc u64s argv4;
        load_starg(u64s argv4);
        {
            alloc i32s a5;
            i32t tmp15 = n10 as i32;
            i32s a5 = i32t tmp15;
            alloc i32s b6;
            i32t tmp16 = n10 as i32;
            i32s b6 = i32t tmp16;
            alloc i32s c7;
            i32t tmp17 = n10 as i32;
            i32s c7 = i32t tmp17;
            alloc i32s d8;
            i32t tmp18 = n10 as i32;
            i32s d8 = i32t tmp18;
            alloc i32s k9;
            i32t tmp19 = n10 as i32;
            i32s k9 = i32t tmp19;
            alloc i32s f10;
            i32t tmp20 = n10 as i32;
            i32s f10 = i32t tmp20;
            store_arg(i32s a5);
            store_arg(i32s b6);
            i32t tmp21 = call sum(i32 a, i32 b) -> i32, argc c2;
            i32t tmp22 = i32s a5 * i32s b6;
            i32t tmp23 = i32t tmp22 + i32s c7;
            i32t tmp24 = i32t tmp23 + i32s d8;
            i32t tmp25 = i32t tmp24 + i32s k9;
            i32t tmp26 = i32t tmp25 + i32s f10;
            i32t tmp27 = i32t tmp21 > i32t tmp26;
            if i32t tmp27, goto l73;
            {
                exit n1;
            }
            l73:
            alloc i32s l11;
            u64t tmp28 = &(i32s f10);
            i32t tmp29 = u64t tmp28 as i32;
            i32s l11 = i32t tmp29;
            exit i32s l11;
        }
    }
}
```