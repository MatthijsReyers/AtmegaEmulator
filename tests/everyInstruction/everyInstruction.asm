ldi  r16, 7
ldi  r17, 0
ldi  r18, 100
ldi  r19, 255
ldi  r30, 7
ldi  r31, 7
mov  r0, r16
mov  r16, r0
mov  r31, r18
mov  r30, r31
clc
clh
cli
cln
clt
cls
push r16
push r17
push r16
rjmp -20
