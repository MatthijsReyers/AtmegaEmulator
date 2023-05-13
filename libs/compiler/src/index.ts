export * from './lib/error';

import { Chunk, isDeviceHeader, isInstruction, splitChunk } from './lib/chunk';
import { assignAbsoluteLabels, assignRelativeLabels, getLabels } from './lib/labels';
import * as emulator from "@atmega-emulator/emulator";
import { parseInstruction } from './lib/parsers';
import { getVariables } from './lib/variable';
import { assignAddresses, formatInstructionText } from './lib/utils';

export function compile(text: string): emulator.Program
{
    // Split each text into chunks for processing.
    let chunks: Chunk[] = text.split('\n').map((line, i) => ({ line_nr: i+1, text: line }));
    
    // Remove comments from chunk text
    chunks.map((chunk) => {
        chunk.text = chunk.text.replace(/;([^\n]*)$/, '').trim();
    });

    // Filter out empty chunks.
    chunks = chunks.filter((chunk) => chunk.text.length > 0)

    // Replace variables with their real value.
    const variables = getVariables(chunks);
    const instructions = chunks.filter(isInstruction);
    instructions.map((chunk) => {
        for (const name of variables.keys()) {
            chunk.text = chunk.text.replaceAll(name, variables.get(name)||'');
        }
    });

    // Split up instructions into opcodes and arguments.
    instructions.map(splitChunk);

    // Assign memory addresses to instructions.
    assignAddresses(instructions);

    // Replace all labels in jumps with their actual value.
    const labels = getLabels(chunks);
    assignRelativeLabels(instructions, labels);
    assignAbsoluteLabels(instructions, labels);

    instructions.map((chunk) => {
        chunk.text = formatInstructionText(chunk.text);
    });

    // 
    const deviceHeaders = chunks.filter(isDeviceHeader);
    if (deviceHeaders.length > 1) {
        throw new Error(`Found second ".device" header on line ${deviceHeaders[1].line_nr}.`);
    }
    
    return {
        chipHint: '',
        instructions: instructions.map(parseInstruction)
    };
}
