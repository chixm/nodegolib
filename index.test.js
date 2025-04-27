const { hello } = require('./index');

test('hello world test', () => {
  expect(hello("World")).toBe("Hello, World!");
});
