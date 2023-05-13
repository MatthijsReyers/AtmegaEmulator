import { ArithmeticLogicUnit } from "./alu";
import { EPromSize } from "./eprom";
import { RandomAccessMemory, SRamSize } from "./ram";
import { RegisterFile } from "./register";

export type ChipType = 'ATmega48PA'|'ATmega168PA';

export interface ATmegaChip {
    readonly SRAM_SIZE: SRamSize;
    readonly EPROM_SIZE: EPromSize;
    readonly TYPE: ChipType;
    
    readonly registers: RegisterFile;
    readonly ram: RandomAccessMemory;
    readonly alu: ArithmeticLogicUnit;
}

// ; .device atmega168     ; Arduino UNO
// ; .device atmega2560    ; Arduino Mega
// .device atmega328p    ; Arduino Nano

export class ATmega48PA implements ATmegaChip 
{
    public readonly SRAM_SIZE = 512;
    public readonly EPROM_SIZE = 256;
    public readonly TYPE = 'ATmega48PA';

    public readonly registers: RegisterFile;
    public readonly ram: RandomAccessMemory;
    public readonly alu: ArithmeticLogicUnit;

    public constructor() {
        this.registers = new RegisterFile();
        this.ram = new RandomAccessMemory(this.SRAM_SIZE)
        this.alu = new ArithmeticLogicUnit(this);
    }
}

export class ATmega168PA implements ATmegaChip 
{
    public readonly SRAM_SIZE = 1024;
    public readonly EPROM_SIZE = 512;
    public readonly TYPE = 'ATmega168PA';

    public readonly registers: RegisterFile;
    public readonly ram: RandomAccessMemory;
    public readonly alu: ArithmeticLogicUnit;

    public constructor() {
        this.registers = new RegisterFile();
        this.ram = new RandomAccessMemory(this.SRAM_SIZE);
        this.alu = new ArithmeticLogicUnit(this);
    }
}
