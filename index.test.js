const { hello } = require('./index');

describe('hello functions', () => {
  test('hello async test', async () => {
    const result = await hello("Async World");
    expect(result).toBe("Hello Async World from Go Async!\n");
  }, 10000); // 10秒
});
