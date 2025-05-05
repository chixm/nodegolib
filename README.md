# nodegolib
node library for golang based dll

# npm launch when accessed private
npm publish --access=private

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

# Build 

```
npm install .
```

To Create dll, run command below in lib directry
```
 go build -buildmode=c-shared -o goshared.dll
```