; Sept 5 2019
;
; Matthijs Reyers
;
; Bonus assignment

.device atmega168

ldi     r17, 0          ; one multiplicand, try other values!
ldi     r18, 3          ; the other multiplicand, try other values!

; ############### OUR CODE ###############

ldi     r16, 0          ; r16 will carry final result.

loop:  
    sbrc    r18, 0      ; skip next if bit is clear
    add     r16, r17    ; add thing
    lsl     r17         ; shift r17 left (to multiply it)
    lsr     r18         ; shift r18 right (to get next bit)

    brne    loop        ; checks if zero flag is set by lsr (which is done when r18 is full of zeros!!)

; ########################################

call sendr16tolaptop ; send calculation result to laptop

again:
    rjmp again ; finished, go into infinite loop
    
.include "rs232link.inc"
