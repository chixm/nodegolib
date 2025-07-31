
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

async function hello(name) {
  try {
    const result = await addon.executeAsyncPromise(name);
    return `${result}`;
  } catch (error) {
    console.error("Error in hello function:", error);
    throw error;
  }
}

module.exports = { hello };
