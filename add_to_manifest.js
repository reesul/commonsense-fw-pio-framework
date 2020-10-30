fs = require('fs')

const FRAMEWORK_STRING = 'framework-commonsense'
const BASE_URL = "https://bitbucket.org/ccsg-res/commonsense-pio-framework/raw/HEAD/"
const MANIFEST_FILE = './manifest.json'
console.log('\n\nAdding SHA and filename to the manifest.json') 

manifest = require(MANIFEST_FILE)

let args = process.argv.slice(2)

let filename = args[0]
let version = args[1]
let sha = args[2]

const URL = BASE_URL + filename

// console.log("filename: " + filename)
// console.log("sha1 hash: " + sha)

let entries = manifest[FRAMEWORK_STRING]

let is_found = false
let desired_entry;
for (entry of entries) {
    if (entry.url === BASE_URL) {
        console.log(entry)
        is_found = true
        desired_entry = entry
    }
}

if (is_found) 
{
    desired_entry.sha1 = sha
}
else {
    desired_entry = {}
    desired_entry.sha1 = sha;
    desired_entry.system = "*";
    desired_entry.url = URL;
    desired_entry.version = version

    // console.log(desired_entry)
    
    entries[Object.keys(entries).length] = (desired_entry)
}

// console.log(manifest)

fs.writeFile(MANIFEST_FILE, JSON.stringify(manifest, null, 4), function(err) {
    if (err) console.log(err);
});









