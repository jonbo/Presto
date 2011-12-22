@callEach #ls:
    #fn = 0
    #i = 0
    #j = len(ls)
    wl i < j:
        fn = ls[i]
        fn()
        i = i+1
    ;
;
@makewire #:
    #signal = 0
    #actions = []
    @set_signal #val:
        if signal != val:
            signal = val
            callEach(actions)
        ;
    ;
    @add_action #fn:
        actions[len(actions)] = fn
        fn()
    ;
    @dispatch #m:
        if m == "get-signal": signal;
        el?
            if m == "set-signal!": set_signal;
            el?
                if m == "add-action!": add_action;
                el?
                    println("Unknown operation -- WIRE: "+ m)
                ;
            ;
        ;
    ;
    dispatch
;
@getSignal #wire:
    wire("get-signal")
;
@setSignal #wire,val:
    #set = wire("set-signal!")
    set(val)
;
@addAction #wire,fn:
    #add = wire("add-action!")
    add(fn)
;

@and_gate #a1,a2,out:
    @action #:
        #new_value = (getSignal(a1) && getSignal(a2))
        setSignal(out,new_value)
    ;
    addAction(a1, action)
    addAction(a2, action)
;
@or_gate #a1,a2,out:
    @action #:
        #new_value = (getSignal(a1) || getSignal(a2))
        setSignal(out,new_value)
    ;
    addAction(a1, action)
    addAction(a2, action)
;
@inverter #a1,out:
    @action #:
        #new_value = !(getSignal(a1))
        setSignal(out,new_value)
    ;
    addAction(a1, action)
;
@half_adder #a,b,s,c:
    #d = makewire()
    #e = makewire()
    or_gate(a,b,d)
    and_gate(a,b,c)
    inverter(c,e)
    and_gate(d,e,s)
;
@full_adder #a,b,c_in,s,c_out:
    #s = makewire()
    #c1 = makewire()
    #c2 = makewire()
    half_adder(b,c_in,s,c1)
    half_adder(a,s,sum,c2)
    or_gate(c1,c2,c_out)
;


//#w = makewire()
//#x = makewire()
//#y = makewire()
//#z = makewire()
//inverter(w,z)
//or_gate(y,x,z)
//and_gate(w,x,y)
//setSignal(w,0)
//setSignal(x,0)
//setSignal(y,1)
//println(getSignal(z))
println("-------")

#a = makewire()
#b = makewire()
#c_in = makewire()
#c_out = makewire()
#sum = makewire()
full_adder(a,b,c_in,sum,c_out)

// Test every case of the full_adder
wl getSignal(c_in) < 2:
    setSignal(b,0)
    wl getSignal(b) < 2:
        setSignal(a,0)
        wl getSignal(a) < 2:
            print("A:", getSignal(a), " B:", getSignal(b), " Ci:", getSignal(c_in))
            println(" | Co:", getSignal(c_out), " S:", getSignal(sum))
            setSignal(a,getSignal(a)+1)
        ;
        setSignal(b,getSignal(b)+1)
    ;
    setSignal(c_in,getSignal(c_in)+1)
;


