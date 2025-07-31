const { hello, helloAsync } = require('./index');

describe('hello functions', () => {
  test('sum function', () => {
    expect(1 + 1).toBe(2);
  });

  /**test('hello world test', () => {
    try {
      const result = hello("World");
      expect(result).toBe("Hello, World!");
    } catch (error) {
      console.error("Error in hello function:", error);
    }
  });*/

  test('hello async test', async () => {
    const result = await helloAsync("Async World");
    expect(result).toBe("Hello, Async World!");
  });
});