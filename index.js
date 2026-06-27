
const addon = require('bindings')('addon');

async function hello(name) {
  const result = await addon.executeAsyncPromise(name);
  return `${result}`;
}

module.exports = { hello };
