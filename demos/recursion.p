@timeDiff #a,b:
    @abs #a: 
        if a < 0: a*(0-1);
        el? a; 
    ;
    #sec = b[0]-a[0]
    #milli = b[1]-a[1]
    if milli == abs(milli):
        println("(Took ", sec,".",milli," seconds)")
    ;
    el?
        println("(Took ", sec-1,".",(1000+milli)," seconds)")
    ;
;

// ---------------------------------------------
// Recursive factorial
@fact #n:
    if n == 1: 1;
    el? n * fact(n-1);
;

#amount = 10
println("The first ", amount, " factorials are:")
#i = 0
wl (i=i+1) <= amount:
    print(fact(i)," ")
;
println(".")

// ---------------------------------------------
// A slow recusive fib
@fib #n:
    if n == 0: 0;
    el? 
        if n == 1: 1;
        el? fib(n-1) + fib(n-2);
    ;
;
amount = 24
println("(slow) The first ", amount, " fibs are:")
#start = time([])
i = 0
wl i <= amount:
    print(fib(i)," ")
    i = i+1
;
#stop = time([])
println(".")
timeDiff(start,stop)

// ---------------------------------------------
// A (way) faster memorized version of recursive fib
@mfib #n,table:
    if len(table) > n: table[n];
    el? table[n] = mfib(n-1,table) + mfib(n-2,table);
;

amount = 35
println("(fast memorized) The first ", amount, " fibs are:")
start = time([])
i = 0
wl i <= amount:
    print(mfib(i,[0,1])," ")
    i = i+1
;
stop = time([])
println(".")
timeDiff(start,stop)
