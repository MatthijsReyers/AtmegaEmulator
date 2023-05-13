import { DOUBLE_WIDTH_INSTRUCTIONS, InstructionSymbol } from "@atmega-emulator/emulator";
import { Chunk } from "./chunk";

export function assignAddresses(instructions: Chunk[]): Chunk[]
{
    let offset = 0;
    for (let i = 0; i < instructions.length; i++) {
        const inst = instructions[i];
        inst.memoryAddress = (i + offset);
        // 32bit instructions take up the space of two instructions and thus cause all instructions
        // after to be offset by 16 bits.
        console.log(DOUBLE_WIDTH_INSTRUCTIONS.includes(inst.opcode as InstructionSymbol), inst.opcode)
        if (DOUBLE_WIDTH_INSTRUCTIONS.includes(inst.opcode as InstructionSymbol)) {
            offset += 1;
        }
    }
    return instructions;
}

/** 
 * Formats the text for a given instruction by removing tabs and double whitespace etc. 
 */
export function formatInstructionText(text: string) {
    return text.split(',').map(s => s.replaceAll(/\s+/g, ' ').trim()).join(', ');
}
