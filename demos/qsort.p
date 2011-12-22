@qsort #a,p,r:
    @part #a,p,r:
      #x = a[r]
      #i = p-1
      #j = p
      #tmp = 0
      @swap #x,y:
          tmp = a[x]
          a[x] = a[y]
          a[y] = tmp
      ;
      wl j < r:
        if a[j] <= x:
          i = i+1
          swap(i,j)
        ;
        j = j+1
      ;
      swap(i+1,r)
      i+1
    ;

    if p < r:
      #q = part(a,p,r)
      qsort(a,p,q-1)
      qsort(a,q+1,r)
    ;
;


#arr = [3,2,4,10,1,2,5,20,13,3,2,3,4,5,22,100,200,30]
qsort(arr,0,len(arr)-1)
print(arr)
