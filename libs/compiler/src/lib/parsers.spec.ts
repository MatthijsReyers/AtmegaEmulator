

describe('Parsing numbers', () => {

    test('it should parse decimal numbers', () => {
        for (let i = 0; i < 250; i++) {
            expect(parseInt(`${i}`)).toBe(i);
        }
    });

});