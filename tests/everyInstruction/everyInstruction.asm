ldi  r20, 15
swap r20

clc
clh
cli
cln
clt
cls

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
