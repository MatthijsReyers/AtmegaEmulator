.device atmega168       ; message to assembler not an instruction.
ldi     r17, 0          ; 1110 0000 0001 0000
ldi     r18, 3          ; 1110 0000 0010 0011
ldi     r16, 0          ; 1110 0000 0000 0000
loop:  
    sbrc    r18, 0      ; 1111 1101 0010 0000
    add     r16, r17    ; 0000 1111 0000 0001
    lsl     r17         ; 0000 1100 0001 0001
    lsr     r18         ; 1001 0101 0010 0110
    brne    loop        ; 1111 01-- ---- ----
again:
    rjmp again          ; 1001 010- ---- 110- 
                        ; ---- ---- ---- ----
