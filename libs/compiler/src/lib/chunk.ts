export type Chunk = {
    /* Line number */
    line_nr: number;

    /* Raw text string */
    text: string;

    /* Memory address of instruction */
    memoryAddress?: number;
    
    opcode?: string;
    args?: string[];
};

export function isHeader(chunk: Chunk): boolean {
    return /^\.([^$]*)$/.test(chunk.text);
}

export function isInstruction(chunk: Chunk): boolean {
    return !isLabel(chunk) && !isHeader(chunk)
}

export function isLabel(chunk: Chunk): boolean {
    return /^([^:]*)\s*:$/.test(chunk.text);
}

export function isDeviceHeader(chunk: Chunk): boolean {
    return /^\.device(.*)/.test(chunk.text);
}

export function isVariable(chunk: Chunk): boolean  
{
    return /^\.equ ([^$]*)$/.test(chunk.text);
}

export function splitChunk(instruction: Chunk) 
{
    const tmp = instruction.text.match(/^([a-zA-Z]{2,5})(\s+[^\s,]*,)?\s+([^\s,]*)/);
    if (tmp === null) {
        throw new Error(
            `Syntax error on line ${instruction.line_nr}: "${instruction.text}"`
        );
    }
    instruction.opcode = tmp[1].toLowerCase();
    instruction.args = [];
    if (tmp[2] && tmp[2].trim().length)
        instruction.args.push(tmp[2].replace(',','').trim())
    if (tmp[3] && tmp[3].length)
        instruction.args.push(tmp[3])
}