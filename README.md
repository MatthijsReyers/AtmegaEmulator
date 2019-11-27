# AtmegaEmulator

Readme is to be written

## Limitations
*Reverse assembling:* Because of the way the AVR assembly instructions are translated to opcodes, it is impossible to differentiate between certain instructions. An example of this is the "Branch if Equal" instruction `BREQ -5` and the "Branch if Bit in SREG in Set" instruction `BRBS 1, -5`.

## Instructions support.

| Status | Instructions |
| ------ | ------------ |
| Fully supported | ADC, ADD, AND, ASR, BLD, BCLR, BRBC, BRBS, BRCC, BRCS, BREQ, BRGE, BRHC, BRHS, BRID, BRIE, BRLO, BRLT, BRMI, BRNE, BRPL, BRSH, BRTC, BRTS, BRVC, BRVS, BSET, CLC, CLH, CLI, CLN, CLR, CLS, CLT, CLV, CLZ, CP, CPC, DEC, EOR, LDI, LSL, LSR, MOV, NOP, RCALL, RET, RJMP, SEC, SEH, SEI, SEN, SE, SES, SET, SEV, SEZ, SUB, SUBI, SWAP |
| Partially supported | BREAK |
| Support planned | ADIW, ANDI, BST, CALL, CBI, CBR, COM, CPI, CPSE, EICALL, EIJMP, ELPM, FMUL, FUMLS, FMULSU, ICALL, IJMP, IN, INC, JMP, LAC, LAS, LAT, LD, lDD, LDS, LPM, MOVW, MUL, MULS, MULSU, NEG, OR, ORI, OUT, POP, PUSH, RETI, ROL, ROR, SBC, SBCI, SBI, SBIC, SBIS, SBIW, SBR, SBRC, SBRS,  SPM, ST, STD, STS, TST |
| Support not planned | DES, WDR, XCH |
