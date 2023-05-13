import { ATmegaChip } from "./chip";
import { Adc, Add, Adiw, Instruction, InstructionSymbol } from "./instruction";
import { RegisterFile, StatusRegister } from "./register";

export class ArithmeticLogicUnit
{
    private readonly chip: ATmegaChip;
    private readonly regs: RegisterFile;
    private readonly status: StatusRegister;

    private disabledInstructions: Set<InstructionSymbol> = new Set([]);

    public constructor(chip: ATmegaChip) {
        this.chip = chip;
        this.regs = chip.registers;
        this.status = chip.registers.statusReg;
    }

    /**
     * Disables a given instruction. This is needed because the ALU indiscriminately implements all
     * instructions supported by Atmega chips. Trying to execute a disabled instruction will cause
     * an error to be thrown.
     * 
     * @param symbol: InstructionSymbol -  
     */
    public disableInstruction(symbol: InstructionSymbol) {
        this.disabledInstructions.add(symbol);
    }

    /**
     * executeInstruction
     */
    public executeInstruction(inst: Instruction) {
        if (this.disabledInstructions.has(inst.data.symbol)) {
            throw new Error(
                `Unsupported instruction: '${inst.data.symbol}' at addr 0x0${inst.memoryAddress.toString(16)}. `
                +`The ${this.chip.TYPE} chip does not support this instruction.`
            );
        }

        // Note the use of a giant switch-case over a lookup map because this way the compiler can 
        // do type checking on the instruction data objects.
        switch (inst.data.symbol) {
            case 'adc': this.adc(inst.data, inst); break;
            case 'add': this.add(inst.data, inst); break;
            case 'adiw': this.adiw(inst.data, inst); break;
        
            default:
                throw new Error(`Unsupported instruction: '${inst.data.symbol}' at addr 0x0${inst.memoryAddress.toString(16)}`);
        }
    }


    /** 
     * **** Add with Carry ****
     * Adds two registers and the contents of the C Flag and places the result in the destination
     * register Rd.
     */
    protected adc(data: Adc, inst: Instruction) {
        throw new Error("");
    }

    /** 
     * **** Add without Carry ****
     * Adds two registers without the C Flag and places the result in the destination register Rd.
     */
    protected add(data: Add, inst: Instruction) {
        const dest = this.regs.getGeneralReg(data.regDest);
        const left = this.regs.getGeneralReg(data.regIn);
        const result = left.value + dest.value;
        dest.value = result & 0x0FF ;
        this.status.negativeFlag = !!(result & 0b0_1000_0000);
    }

    /** 
     * **** Add Immediate to Word ****
     * Adds an immediate value (0 - 63) to a register pair and places the result in the register 
     * pair. This instruction operates on the upper four register pairs, and is well suited for 
     * operations on the pointer registers.
     */
    protected adiw(data: Adiw, inst: Instruction) {
        throw new Error("");
    }
}