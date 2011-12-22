
// 1. Create an array
#a = [1,2,3,4]
println("1) ",a)

// 2. Getting a specific item
println("2) ",a[0])

// 3. Let 'b' be a reference/pointer to 'a' (since it is an array)
#b = a  

// 4. Change the first item of b
b[0] = "A"

// 5. Thus it also changes 'a'
println("3-5) ",a)



// 6. Multi-dimension arrays work too
#x = [a,["hello","world"],4]
println("6) ",x)

// 7. Calling a specific element in it is easy as well..
println("7) ",x[1][1])

// 8. Appending/adding an element to the end of the array
insert(x,"last", len(x))
println("8) ",x)

// 9. This way also works
x[len(x)] = "really last"
println("9) ",x)

// 10. or at the Beginning
insert(x,"first", 0)
println("10) ",x)

// 11. grabbing a portion of the multi-array and changing it
#t = x[2]
t[0] = "HELLO"
println("11) ",x)


// 12. Comparing arrays..
if [] == []: println("12)","Empty lists!");
if [1] == []: println("This shouldn't print");
if [1] != []: println("This should print");
#v = [2]
insert(v,1,1)
if [2,1] == v: println("This should print as well");


// 13. Constant time lookup..
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

#huge_array = [1]
huge_array[800] = 5
println(huge_array)
#start = time([])
print(huge_array[0])
#stop = time([])
timeDiff(start,stop)

start = time([])
print(huge_array[800])
stop = time([])
timeDiff(start,stop)

// Proof that timer works(since the constant time lookup is usually 0.0)
start = time([])
#i = 0
wl i < 800:
    i = i+1
;
stop = time([])
print("While loop")
timeDiff(start,stop)
