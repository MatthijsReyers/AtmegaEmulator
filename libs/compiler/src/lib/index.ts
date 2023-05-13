const test = `
; Tells compiler what cpu we are using.
; (Actually optional for most chip/asembler combinations).
; .device atmega168     ; Arduino UNO
; .device atmega2560    ; Arduino Mega
.device atmega328p    ; Arduino Nano

; variables for the locations of the data-direction/port registers.
.equ DDRB = 4
.equ PORTB = 5

;

; call demo2
rcall demo2

; load on ON & OFF  
ldi r16, $20
ldi r17, $0

call demo

; set out mode on pin
out DDRB, r16 

; out     PORTB, r16      ; Turn ON

out     PORTB, r17      ; Turn OFF

rcall demo2
demo:
demo3:
demo2:
call sendr16tolaptop    ; send r16 over serial everytime loop restarts.
call sendr16tolaptop    ; send r16 over serial everytime loop restarts.

.include "rs232link.inc"

loop:
    jmp loop

rcall demo2

; My thingy here!`;
