# AtmegaEmulator

## Limitations

*Reverse assembling:* Because of the way the AVR assembly instructions are translated to opcodes, it is impossible to differentiate between certain instructions. An example of this is the "Branch if Equal" instruction `BREQ -5` and the "Branch if Bit in SREG in Set" instruction `BRBS 1, -5`. This emulator will use the dedicated instructions instead of "if bit in SREG is" style instructions wherever possible.

*Jumps and labels:* Because of the way this program is designed it does not and cannot not create or display labels, which is why it instead just shows the relative or absolute jump value for jump and call type instructions.

## Instructions support.

| Status | Instructions |
| ------ | ------------ |
| Fully supported | ADC, ADD, ADIW, AND, ANDI, ASR, BLD, BCLR, BRBC, BRBS, BRCC, BRCS, BREQ, BRGE, BRHC, BRHS, BRID, BRIE, BRLO, BRLT, BRMI, BRNE, BRPL, BRSH, BRTC, BRTS, BRVC, BRVS, BSET, BST, CBI, CLC, CLH, CLI, CLN, CLR, CLS, CLT, CLV, CLZ, COM, CP, CPC, CPI, DEC, EOR, LDI, LSL, LSR, MOV, NOP, OR, ORI, OUT, POP, PUSH, RCALL, RET, RJMP, SEC, SEH, SEI, SEN, SE, SES, SET, SEV, SEZ, SUB, SUBI, SWAP |
| Partially supported | BREAK, SLEEP |
| Support planned | CALL, CBR, CPSE, EICALL, EIJMP, ELPM, FMUL, FUMLS, FMULSU, ICALL, IJMP, IN, INC, JMP, LAC, LAS, LAT, LD, lDD, LDS, LPM, MOVW, MUL, MULS, MULSU, NEG, ROL, ROR, SBC, SBCI, SBI, SBIC, SBIS, SBIW, SBR, SBRC, SBRS, ST, STD, STS, TST |
| Support not planned | DES, RETI, SPM, WDR, XCH |

## Installation

There is currently only a linux version of AtmegaEmulator, because AtmegaEmulator uses the ncurses library, which is not easily available on Windows.

## Support and maintenance

AtmegaEmulator is a slightly out of control hobby project that stop recieving new features after January 1st 2020, because the creator needs time to focus on his education and other personal projects. Major bugs might be still fixed if any are found.
