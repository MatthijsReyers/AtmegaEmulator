ldi  r20, 15
swap r20

clc
clh
cli
cln
clt
cls

nop

ldi  r16, 7
ldi  r17, 0
ldi  r18, 100
ldi  r19, 255
ldi  r30, 7
ldi  r31, 7

mov  r0, r16
mov  r1, r0
mov  r31, r18
mov  r30, r31

ori  r31, 1
ori  r31, 0
or   r30, r31
or   r18, r16

lsr  r0
lsr  r30

eor  r0, r1
eor  r0, r30
eor  r30, r17
eor  r31, r5

push r0
push r1
push r16
push r30
push r31
pop  r31
pop  r30
pop  r16
pop  r1
pop  r0

cp   r0, r31
cp   r31, r30

dec  r17
inc  r17

dec  r0
inc  r0

dec  r31
inc  r31
