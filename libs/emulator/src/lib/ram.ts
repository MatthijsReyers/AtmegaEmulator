
export type SRamSize = 512|1024|2048;

export class RandomAccessMemory 
{
    protected data: Uint8Array;
    protected size: number;

    constructor(size: SRamSize) {
        this.size = size;
        this.data = new Uint8Array(this.size);
    }
}