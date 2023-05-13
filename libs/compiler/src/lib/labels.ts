import { Chunk, isLabel } from "./chunk";

export function getLabels(chunks: Chunk[]): Map<string, number> 
{
    chunks = chunks.filter(isLabel);
    const labels = new Map<string, number>();
    for (const chunk of chunks) {
        const name = chunk.text.replace(/\s*:$/, '');
        if (name.length < 1) {
            throw new Error(`Found empty label on line ${chunk.line_nr}.`);
        }
        if (labels.has(name)) {
            throw new Error(`Found duplicate label "${name}" on line ${chunk.line_nr}, it has been used before.`);
        }
        labels.set(name, chunk.line_nr);
    }
    return labels;
}

export function assignRelativeLabels(instructions: Chunk[], labels: Map<string, number>) 
{
    for (const inst of instructions) {
        if (['rjmp', 'rcall'].includes(inst.opcode || '')) {
            if (inst.args?.length != 1) {
                throw new Error(
                    `The ${inst.opcode} instruction has 1 operand, but found ${inst.args?.length}.`
                );
            }
            if (labels.has(inst.args[0])) {
                const label_name = inst.args[0];
                const line_nr = labels.get(label_name) as number;
                const addr = getLabelAddress(label_name, line_nr, instructions);
                inst.args[0] = (addr - (inst.memoryAddress || 0) - 1).toString();
            }
        }
    }
}

export function assignAbsoluteLabels(instructions: Chunk[], labels: Map<string, number>) 
{
    for (const inst of instructions) {
        if (['jmp', 'call'].includes(inst.opcode || '')) {
            if (inst.args?.length != 1) {
                throw new Error(
                    `The ${inst.opcode} instruction has 1 operand, but found ${inst.args?.length}.`
                );
            }
            if (labels.has(inst.args[0])) {
                const label_name = inst.args[0];
                const line_nr = labels.get(label_name) as number;
                const addr = getLabelAddress(label_name, line_nr, instructions);
                inst.args[0] = addr.toString();
            }
        }
    }
}

export function getLabelAddress(label_name: string, label_line_nr: number, instructions: Chunk[]) 
{
    for (let i = 0; i < instructions.length; i++) {
        const inst = instructions[i];
        if (inst.line_nr > label_line_nr) {
            return inst.memoryAddress as number;
        }
    }
    throw new Error(`Cannot resolve label "${label_name}", no instructions occur after it.`);
}
