import { Instruction } from "./instruction"

export type Program = {
    instructions: Instruction[]
    chipHint: string;
}
