
ldi     r16, 10
ldi     r17, 250
add     r17, r16        ; Carry flag

ldi     r16, 10
ldi     r17, 10
sub     r16, r17        ; Zero flag

ldi     r16, 1
ldi     r17, -5
add     r17, r16        ; Negative flag


rjmp    end
