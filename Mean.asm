        stl 0      
        adj -1      ; adjust stack pointer
loop:   ldc num
        ldnl 0      ; load num into A
        ldc sum
        ldnl 0      
        add         ; A = B + A
        ldc sum
        stnl 0      
        ldc num
        ldnl 0      ; load num into A again
        adc -1      
        brz loop1    ; if A == 0, stop sum calculation
        ldc num
        stnl 0      ; otherwise update num value
        br loop     ; repeat calculation
loop1:  ldc sum
        ldnl 0
        ldc numm
        ldnl 0
        sub
        brlz done
        ldc sum
        stnl 0
        ldc ans
        ldnl 0
        adc 1
        ldc ans
        stnl 0
        br loop1

done:   
        adj 1       ; if done, adjust stack pointer      
        HALT      

num: data 10 
numm: data 10   
sum: data 0 
ans: data 0   

; output = 55 (sum of first 10 numbers)
