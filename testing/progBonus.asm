.device atmega168       ; message to assembler not an instruction.
again:
ldi     r16, 0          ; 1110 0000 0010 0011
ldi     r17, 4          ; 1110 0000 0001 0000
ldi     r18, 9          ; 1110 0000 0010 0011
loop:  
    sbrc    r18, 0      ; 1111 1101 0010 0000
    add     r16, r17    ; 0000 1111 0000 0001
    lsl     r17         ; 0000 1100 0001 0001
    lsr     r18         ; 1001 0101 0010 0110
    brne    loop        ; 1111 01-- ---- ----
ldi     r19, 0          ; 1110 0000 0010 0011
ldi     r20, 6          ; 1110 0000 0001 0000
ldi     r21, 5          ; 1110 0000 0010 0011
loop2:  
    sbrc    r21, 0      ; 1111 1101 0010 0000
    add     r19, r20    ; 0000 1111 0000 0001
    lsl     r20         ; 0000 1100 0001 0001
    lsr     r21         ; 1001 0101 0010 0110
    brne    loop2       ; 1111 01-- ---- ----
    rjmp again          ; 1100 ---- ---- ---- 
