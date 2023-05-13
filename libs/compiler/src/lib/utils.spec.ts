import { Chunk } from './chunk';
import { assignAddresses, formatInstructionText } from './utils';

describe('Assigning addresses', () => {

    test('it should start at 0x00', () => {
        const instructions: Chunk[] = [
            { line_nr: 4, text: 'nop', opcode: 'nop' }
        ]; 
        assignAddresses(instructions);
        expect(instructions[0].memoryAddress).toBe(0);
    });

    test('it should increment addresses by 1', () => {
        const instructions: Chunk[] = [
            { line_nr: 4, text: 'nop', opcode: 'nop' },
            { line_nr: 5, text: 'nop', opcode: 'nop' },
            { line_nr: 7, text: 'nop', opcode: 'nop' },
            { line_nr: 8, text: 'nop', opcode: 'nop' },
            { line_nr: 10, text: 'nop', opcode: 'nop' },
        ]; 
        assignAddresses(instructions);
        for (let i = 0; i < instructions.length; i++) {
            expect(instructions[i].memoryAddress).toBe(i);
        }
    });

    test('it should increment 32 bit instructions by 2', () => {
        const instructions: Chunk[] = [
            { line_nr: 4, text: 'nop', opcode: 'nop' },
            { line_nr: 5, text: 'add 1 3', opcode: 'add', args: ['1', '3'] },
            { line_nr: 9, text: 'nop', opcode: 'nop' },
            { line_nr: 10, text: 'call myfunc', opcode: 'call', args: ['myfunc'] },
            { line_nr: 11, text: 'nop', opcode: 'nop' },
            { line_nr: 15, text: 'nop', opcode: 'nop' },
            { line_nr: 10, text: 'jmp myloop', opcode: 'jmp', args: ['myloop'] },
            { line_nr: 15, text: 'nop', opcode: 'nop' },
            { line_nr: 15, text: 'nop', opcode: 'nop' },
        ]; 
        assignAddresses(instructions);
        let next = 0;
        for (let i = 0; i < instructions.length; i++) {
            const inst = instructions[i];
            expect(inst.memoryAddress).toBe(next);
            next += ((inst.opcode === 'call' || inst.opcode === 'jmp') ? 2 : 1);
        }
    });

});

describe('Formatting instruction text', () => {

    test('it should do nothing to a lone instruction', () => {
        expect(formatInstructionText('nop')).toBe('nop');
        expect(formatInstructionText('break')).toBe('break');
    });

    test('it should remove leading whitespace', () => {
        expect(formatInstructionText('\tnop')).toBe('nop');
        expect(formatInstructionText('   break')).toBe('break');
    });

    test('it should extra whitespace', () => {
        expect(formatInstructionText('\tnop')).toBe('nop');
        expect(formatInstructionText('   ldi  \t r4\t\t, 93')).toBe('ldi r4, 93');
        expect(formatInstructionText('call    yourMum  ')).toBe('call yourMum');
    });
});
