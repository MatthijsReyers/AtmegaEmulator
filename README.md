# AtmegaEmulator

Readme is to be written

## Limitations
*Reverse assembling:* Because of the way the AVR assembly instructions are translated to opcodes, it is impossible to differentiate between certain instructions. An example of this is the "Branch if Equal" instruction `BREQ -5` and the "Branch if Bit in SREG in Set" instruction `BRBS 1, -5`.

## Instructions support.

| Status | Instructions |
| ------ | ------------ |
| Fully supported | AND, BCLR, BRBC, BRBS, BRCC, BRCS, BREQ, BRGE, BRHC, BRHS, BRID, BRIE, BRLO, BRLT, BRMI, BRNE, BRPL, BRSH, BRTC, BRTS, BRVC, BRVS, BSET, CP, LDI, LSL, LSR, MOV, NOP, RCALL, RET, RJMP, SUB, SUBI, SWAP |
| Partially supported | ADC, ADD, ASR, BREAK |
| Support planned | ADIW, ANDI, BLD, BST, CALL, CBI, CBR, CLC, CLH, CLI, CLN, CLR, CLS, CLT, CLV, CLZ, COM, CPC, CPI, CPI CPSE, DEC, EICALL, EIJMP, ELPM, EOR, FMUL, FUMLS, FMULSU, ICALL, IJMP, IN, INC, JMP, LAC, LAS, LAT, LD, lDD, LDS, LPM, MOVW, MUL, MULS, MULSU, NEG, OR, ORI, OUT, POP, PUSH, RETI, ROL, ROR, SBC, SBCI, SBI, SBIC, SBIS, SBIW, SBR, SBRC, SBRS, SEC, SEH, SEI, SEN, SE, SES, SET, SEV, SEZ, SPM, ST, STD, STS, TST |
| Support not planned | DES, WDR, XCH |
