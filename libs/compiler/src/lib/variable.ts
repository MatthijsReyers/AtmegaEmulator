import { Chunk, isVariable } from "./chunk";

export function getVariables(chunks: Chunk[]): Map<string, string> 
{
    chunks = chunks.filter(isVariable);
    const variables = new Map<string, string>();
    for (const chunk of chunks) {
        const tmp = chunk.text.match(/\.equ\s*([^=\s]*)\s*=\s*(\S*)\s*/);
        if (!tmp || tmp.length < 3) {
            throw new Error(`Failed to parse variable on line ${chunk.line_nr}: "${chunk.text}".`);
        }
        if (variables.has(tmp[1])) {
            throw new Error(`Cannot redefine constant "${tmp[1]}" on line ${chunk.line_nr}, it has been defined before.`);
        }
        variables.set(tmp[1], tmp[2]);
    }
    return variables;
}
