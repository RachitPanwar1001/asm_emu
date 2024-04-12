        stl 0       
        adj -1      ; adjust stack pointer
loop:   ldc numm
        ldnl 0      ; load num into A
        ldc sum
        ldnl 0      
        add         ; A = B + A
        ldc sum
        stnl 0      
        ldc num
        ldnl 0      ; load num into A again
        adc -1      
        brz done    ; if A == 0, stop sum calculation
        ldc num
        stnl 0      ; otherwise update num value
        br loop     ; repeat calculation
done:   
        adj 1       ; if done, adjust stack pointer
        ldl 0       
        HALT      

num: data 10 
numm: data 10     
sum: data 0         