
const addon = require('bindings')('hello');

console.log(addon.hello("World")); // Should print "Hello, World!"

function hello(name) {
  // Call the C++ function from the addon
  const result = addon.hello(name);
  return `${result}!`;
}
  
module.exports = { hello };
  