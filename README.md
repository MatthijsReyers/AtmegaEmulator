# AtmegaEmulator

Readme is to be written

## Limitations
*Reverse assembling:* Because of the way the AVR assembly instructions are translated to opcodes, it is impossible to differentiate between certain instructions. An example of this is the "Branch if Equal" instruction and the ...

## Instructions support.

| Status | Instructions |
| ------ | ------------ |
| Fully supported | AND, BCLR, BRBC, BRBS, BRCC, BRCS, BREQ, BRGE, BRHC, BRHS, BRID, BRIE, BRLO, BRLT, BRMI, BRNE, BRPL, BRSH, BRTC, BRTS, BRVC, BRVS, BSET, SUB, LDI, NOP, RCALL, RET, RJMP |
| Partially supported | ADC, ADD, ASR, BREAK |
| Support planned | ADIW, ANDI, BLD, BST, CALL, CBI, CBR, CLC, CLH, CLI, CLN, CLR, CLS, CLT, CLV, CLZ, COM, CP, CPC, CPI, CPI CPSE, DEC, EICALL, EIJMP, ELPM, EOR, FMUL, FUMLS, FMULSU, ICALL, IJMP, IN, INC, JMP, LAC, LAS, LAT, LD, lDD, LDS, LPM, LSL, LSR, MOV, MOVW, MUL, MULS, MULSU, NEG, OR, ORI, OUT, POP, PUSH, RETI, ROL, ROR, SBC, SBCI, SBI, SBIC, SBIS, SBIW, SBR, SBRC, SBRS, SEC, SEH, SEI, SEN, SE, SES, SET, SEV, SEZ, SPM, ST, STD, STS, SUBI, SWAP, TST |
| Support not planned | DES, XCH, WDR |
