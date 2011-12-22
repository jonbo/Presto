// --------------------------------------------------
// Single list(array) map
@map #fn,ls:
    #a = []
    #i = 0
    wl i < len(ls):
        a[len(a)] = fn(ls[i])
        i = i+1
    ;
    a
;
@sqr #x:
    x*x
;
print("Integer list squared: ")
println(map(sqr, [1,2,3,4,5,6,7,8,9,10]))


// --------------------------------------------------
//Anon/lambda functions
print("Integer list doubled: ")
println( map(@#x: x*2; , [1,2,3,4,5,6,7,8,9,10]))

@add #a,b: a+b ;
println(call(add,2,2))
println(apply(add,[2,6]))

// --------------------------------------------------
// Multi-list(array) map
@mmap #fn,ls:
    @at #ls,x: ls[x];
    #a = []
    #i = 0
    #pos = 0
    wl i < len(ls[0]):
        pos = @#x: at(x,i);
        a[len(a)] = apply(fn, map(pos,ls))
        i = i+1
    ;
    a
;
println(mmap(add, [[1,2],[3,4]]))
println(mmap(@#x,y,z: x+y+z;, [[1,2],[3,4],[5,6]]))


