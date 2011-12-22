// ----------------------------------------
// The basic types
println(5)
println(2.2)
println("string")


// ----------------------------------------
// Conditions
if 1 != 2: println("1 is not 2!");

if 1 != 1: println("Shouldn't print!");
el? println("This should print");


// ----------------------------------------
// Variables and functions (and conditions)
#a = 9
@fn #x: x;

if a == fn(9): println("Both are nine!");

// ----------------------------------------
// While loops
#i = 0
wl i < 10:
    print(i," ")
    i = i+1
;
println()
