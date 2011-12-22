@assert #a,b:
    print(a)
    if a == b: println(" (correct)");
    el? println(" (incorrect) -> should be ",b);
;

assert(2+2, 4)
assert(2.2+2, 4.2)
assert(2.2+2.2, 4.4)

assert(2.2-0.1, 2.1)

assert(5*0.1, 0.5)

assert(10/3, 3)
assert(10/3.0, 3.33333)
assert(10.0/3, 3.33333)

assert(10%3, 1)
assert(5.3%2, 1.3)

assert(2^3, 8)
assert(16^.5, 4)


assert(2.2 <= 2.1, 0)
assert(2.2 > 2.1, 1)

assert(3 && 0, 0)
assert(3 || 0, 1)

