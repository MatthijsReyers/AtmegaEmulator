
; ; Immediately jump to main.
; rjmp    abc

main:
    ldi     r20, 0          ; 1110 0000 0100 0000
    ldi     r23, 200        ; 
    
    ldi     r17, 99
    rcall    shuffle
    
    ldi     r17, 5
    rcall    shuffle
    
    ; empty all registers.
    ldi     r20, 0
    ldi     r23, 0
    ldi     r17, 0
    ldi     r16, 0

    ; infinite loop.
    exit:
        rjmp exit


; The shuffle subroutine.
shuffle:
    ldi     r16, 0
    back:
        sbrc    r17, 0
        add     r16, r17
        lsl     r17
        lsr     r16
        brne    back
    ret
