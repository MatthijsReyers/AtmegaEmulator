import * as emulator from "@atmega-emulator/emulator";
import { Chunk } from "./chunk";

export function parseReg(raw: string): number {
    if (raw.charAt(0) === 'r') {
        raw = raw.replace('r', '0');
    }
    return 0;
}

export function parseNum(raw: string): number {
    if (/^\d+$/.test(raw)) {
        return parseInt(raw, 10);
    } 
    if (/^\$\d+$/.test(raw)) {
        return parseInt(raw.replace(/^\$/, '0x'), 16);
    } 
    throw new Error(`Failed to parse "${raw}", expected a number.`);
}

/**
 * Coverts a given text chunk to Instruction object that the emulator expects.
 * Note that this function assumes that the `memoryAddress`, `args`, and `opcode` of the Chunk 
 * object have already been filled in by caller.
 * 
 * @param {Chunk} chunk - Chunk to parse.
 * @returns {Instruction} parsed instruction.
 */
export function parseInstruction(chunk: Chunk): emulator.Instruction 
{
    if (chunk.memoryAddress === undefined) {
        throw new Error(
            `Memory address of instruction chunk was not determined at compilation.`
        );
    }
    if (chunk.args === undefined) {
        throw new Error(
            `Memory address of instruction chunk was not determined.`
        );
    }
    const symbol = chunk.opcode as string;
    let data: emulator.InstructionData;
    switch (symbol) {
        case 'add':
        case 'adc':
        case 'and':
        case 'sub':
            if (chunk.args.length !== 2) {
                throw new Error(`Instruction "${symbol}" takes 2 operants, but got ${chunk.args.length}`);
            }
            data = {
                symbol: symbol,
                regDest: parseReg(chunk.args[0]),
                regIn: parseReg(chunk.args[1])
            };
            break;

        case 'nop':
            if (chunk.args.length !== 0) {
                throw new Error(`Instruction "${symbol}" takes 0 operants, but got ${chunk.args.length}`);
            }
            data = {
                symbol: 'nop',
            };
            break

        case 'jmp':
        case 'call':
            data = {
                symbol: symbol,
                address: parseReg(chunk.args[0]),
            };
            break;

        case 'rjmp':
        case 'rcall':
            data = {
                symbol: symbol,
                offset: parseReg(chunk.args[0]),
            };
            break;
    
        default:
            throw new Error(`Cannot compile instruction "${chunk.opcode}".`);
    }
    return {
        memoryAddress: chunk.memoryAddress,
        data:          data,
        asmText:       chunk.text,
    }
}
