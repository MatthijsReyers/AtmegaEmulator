
export type Instruction = {
    data: InstructionData;

    /** Index/memory address of this instruction in the program, when the instruction pointer 
     * register/program counter has this value, the instruction is about to be executed. */
    memoryAddress: number;

    /** Text string of the assembly that generated this instruction. */
    asmText: string;

    /** True if the emulator needs to trigger a breakpoint before executing this instruction, false
     * or undefined otherwise */
    breakPoint?: boolean;
};

export type Adc = {
    symbol: 'adc';
    regDest: number;
    regIn: number;
};

export type Add = {
    symbol: 'add';
    regDest: number;
    regIn: number;
};

export type Adiw = {
    symbol: 'adiw';
    regDest: number;
    value: number;
};

export type And = {
    symbol: 'and';
    regDest: number;
    regIn: number;
};

export type Sub = {
    symbol: 'sub';
    regDest: number;
    regIn: number;
};

export type Jmp = {
    symbol: 'jmp';
    address: number;
};

export type Call = {
    symbol: 'call';
    address: number;
};

export type Rjmp = {
    symbol: 'rjmp';
    offset: number;
};

export type Rcall = {
    symbol: 'rcall';
    offset: number;
};

export type Nop = {
    symbol: 'nop';
};

export type InstructionData = Adc|Add|Adiw|And|Sub|Jmp|Call|Rjmp|Rcall|Nop;
export type InstructionSymbol = InstructionData['symbol'];

export const DOUBLE_WIDTH_INSTRUCTIONS: InstructionSymbol[] =  ['call', 'jmp'];
