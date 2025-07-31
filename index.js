
const addon = require('bindings')('addon');

console.log("Addon loaded successfully");

console.log(addon);

function helloAsync(name) {
  return new Promise((resolve, reject) => {
    addon.executeAsyncPromise(name, (err, result) => {
      if (err) {
        return reject(err);
      }
      resolve(`${result}!`);
    });
  });
}

module.exports = { helloAsync };
