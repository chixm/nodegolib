# nodegolib
node library for golang based dll

# npm launch when accessed private
npm publish --access=private

# Expecting Use in Windows Only
this nodegolib package is targeted on Windows Application(ex. Electron).

## References
https://github.com/nodejs/node-addon-examples/tree/main/src/1-getting-started/1_hello_world/node-addon-api


# Prepare
Requires 
- Python 
-- 3.10
- Go
-- 1.23.4

install node-gyp
```
npm install global node-gyp
```

## VSCode Extensions

Jest

Python

Go

# Build 

```
npm install .
```

To Create dll, run command below in lib directry
```
 go build -buildmode=c-shared -o goshared.dll
```
instead, you can use
```
npm run build
```
for golang dll build