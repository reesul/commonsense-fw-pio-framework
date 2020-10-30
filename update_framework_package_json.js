fs = require('fs')

let args = process.argv.slice(2)
const package_file = args[0]
const version = args[1]

let package = require('./'+package_file)

console.log(package)

package.version = version

fs.writeFile(package_file, JSON.stringify(package, null, 4), function(err) {
    if (err) console.log(err);
});