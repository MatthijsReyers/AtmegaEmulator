import { flag, uint8 } from "./types";

/**
 * Most registers do not have a default value and can thus technically contain anything at 
 * startup, this can cause weird and undefined behaviour. The value of the register should not
 * be read while it is unset/uninitialized.
 */
export class Register8Bit
{
    private _value: uint8|null;

    public readonly name: string;

    public constructor(name: string, defaultValue?: uint8) {
        this.name = name;
        this._value = defaultValue || null;
    }

    public get unset() { return this._value === null; }
    
    public set value(val: uint8) {
        if (val > 0x0FF)
            throw new Error(`Cannot assign value larger than 8 bits to 8 bit register ${this.name}.`);
        this._value = val;
    }

    public get value(): uint8 {
        if (this._value === null)
            throw new Error(`Cannot read value of register ${this.name} before it has been initialized with a value.`);
        return this._value;
    }
}

/**
 * Some special 8 bit registers can used as a sort of virtual 16 bit register to perform 16 bit 
 * calculations. This class helper class just wraps around two such registers and
 */
export class Register16Bit
{
    private low: Register8Bit;
    private high: Register8Bit;

    constructor(name: string, low: Register8Bit, high: Register8Bit) {
        this.name = name;
        this.low = low;
        this.high = high;
    }

    public readonly name: string;

    public get unset(): boolean {
        return this.low.unset || this.high.unset;
    }

    public set value(val: uint8) {
        if (val > 0x0FFFF)
            throw new Error(`Cannot assign value larger than 16 bits to 16 bit register ${this.name}.`);
        this.low.value  = val & 0x000FF
        this.high.value = val & 0x0FF00
    }

    public get value(): uint8 {
        if (this.unset)
            throw new Error(`Cannot read value of register ${this.name} before it has been initialized with a value.`)
        return this.low.value + (this.high.value << 16);
    }
}

const inv = (x: number) => (x ^ 0xFF)

export class StatusRegister extends Register8Bit
{
    private c = 0b0000_0001;
    private z = 0b0000_0010;
    private n = 0b0000_0100;
    private v = 0b0000_1000;
    private s = 0b0001_0000;
    private h = 0b0010_0000;
    private t = 0b0100_0000;
    private i = 0b1000_0000;
     
    public constructor() {
        super('SREG', 0x000000);
    }

    public get carryFlag(): flag { return !!(this.value & this.c) }
    public set carryFlag(f: flag) { this.value = (this.value & inv(this.c)) | (f ? this.c : 0) }

    public get zeroFlag(): flag { return !!(this.value & this.z) }
    public set zeroFlag(f: flag) { this.value = (this.value & inv(this.z)) | (f ? this.z : 0) }

    public get negativeFlag(): flag { return !!(this.value & this.n) }
    public set negativeFlag(f: flag) { this.value = (this.value & inv(this.n)) | (f ? this.n : 0) }

    public get twoComplementOverflowFlag(): flag { return !!(this.value & this.v) }
    public set twoComplementOverflowFlag(f: flag) { this.value = (this.value & inv(this.v)) | (f ? inv(this.v) : 0) }

    public get signBit(): flag { return !!(this.value & this.s) }
    public set signBit(f: flag) { this.value = (this.value & inv(this.s)) | (f ? this.s : 0) }

    public get halfCarry(): flag { return !!(this.value & this.h) }
    public set halfCarry(f: flag) { this.value = (this.value & inv(this.h)) | (f ? this.h : 0) }

    public get bitCopy(): flag { return !!(this.value & this.t) }
    public set bitCopy(f: flag) { this.value = (this.value & inv(this.t)) | (f ? this.t : 0) }

    public get globalInterruptEnable(): flag { return !!(this.value & this.i) }
    public set globalInterruptEnable(f: flag) { this.value = (this.value & inv(this.i)) | (f ? this.i : 0) }
}

export class RegisterFile 
{
    private generalRegs: Register8Bit[] = [];
    
    public readonly regX: Register16Bit;
    public readonly regY: Register16Bit;
    public readonly regZ: Register16Bit;

    public readonly statusReg: StatusRegister

    public constructor() {
        
        
        for (let i = 0; i < 32; i++) {
            this.generalRegs.push(new Register8Bit(`r${i}`));
        }

        this.regX = new Register16Bit('RX', this.generalRegs[26], this.generalRegs[27]);
        this.regY = new Register16Bit('RY', this.generalRegs[28], this.generalRegs[29]);
        this.regZ = new Register16Bit('RZ', this.generalRegs[30], this.generalRegs[31]);

        this.statusReg = new StatusRegister();
    }

    public getGeneralReg(i: number): Register8Bit {
        if (i >= 32 || i < 0)
            throw new Error(`Cannot get general purpose register ${i}, there are only 32 registers available: [0,1..30,31].`);
        return this.generalRegs[i];
    }
}


