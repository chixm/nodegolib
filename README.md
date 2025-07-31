# nodegolib

GoでビルドしたDLL（共有ライブラリ）をNode.jsから簡単に呼び出すためのライブラリです。  
Windows上のElectronアプリケーションなど、Node.js環境からGoのロジックを利用したい場合に活用できます。

> **対象プラットフォーム: Windows のみ**

---

## アーキテクチャ概要

```
Node.js (index.js)
    │
    │  require('bindings')
    ▼
C++ Addon (addon.cc)  ←  node-addon-api / N-API
    │
    │  LoadLibrary / GetProcAddress (Windows API)
    ▼
Go DLL (include/goshared.dll)
    │
    │  cgo でエクスポートした関数
    ▼
Go ロジック (include/main.go)
```

Node.js → C++ Addon → Go DLL という3層構造で呼び出しが行われます。

---

## 前提条件

| ツール | バージョン |
|--------|-----------|
| Node.js | 22.x 以上 |
| Python | 3.10 以上 (node-gyp のビルドに必要) |
| Go | 1.23.4 以上 |
| Visual Studio Build Tools | C++ ビルドツール込み (node-gyp に必要) |

推奨 VSCode 拡張機能: **Jest**, **Python**, **Go**

---

## セットアップ

### 1. 依存パッケージのインストール

```bash
npm install
```

### 2. Go DLL のビルド

`include/` ディレクトリ内の Go コードを DLL としてビルドします。

```bash
npm run build
```

内部では以下のコマンドが実行されます。

```bash
cd include && go build -o goshared.dll -buildmode=c-shared
```

### 3. C++ Addon のビルド

```bash
npx node-gyp rebuild
```

`npm install` 実行時に自動でビルドされる場合もありますが、手動で実行する場合はこのコマンドを使います。

---

## 使い方

### Node.js から呼び出す

```js
const { hello } = require('./index');

const result = hello("World");
console.log(result); // => "Hello World from Go!"
```

### Go 側で関数を追加する

`include/main.go` に `//export` アノテーション付きで関数を追加します。

```go
package main

import "C"

func main() {}

//export Execute
func Execute(input *C.char) *C.char {
    goInput := C.GoString(input)
    // ここに Go のロジックを実装
    return C.CString("Hello " + goInput + " from Go!\n")
}
```

**ポイント:**
- `import "C"` が必須です
- エクスポートする関数には `//export 関数名` コメントを付けます
- 文字列のやり取りは `C.char` 型を使い、`C.GoString()` / `C.CString()` で変換します
- `main()` 関数は空でも必要です（DLL ビルド時も必須）

### C++ Addon 側で関数を公開する

`addon.cc` 内で DLL から関数をロードして Node.js へ公開します。

```cpp
#include <napi.h>
#include <windows.h>

Napi::String LoadDLL(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // DLL を読み込む
  HMODULE hDLL = LoadLibraryA("include/goshared.dll");
  if (!hDLL) {
    Napi::Error::New(env, "Failed to load DLL").ThrowAsJavaScriptException();
    return Napi::String::New(env, "error");
  }

  // Go でエクスポートした関数を取得
  typedef const char* (*FuncType)(const char*);
  FuncType dllFunction = reinterpret_cast<FuncType>(GetProcAddress(hDLL, "Execute"));

  const char* result = dllFunction("your input");

  FreeLibrary(hDLL);
  return Napi::String::New(env, result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, LoadDLL));
  return exports;
}

NODE_API_MODULE(hello, Init)
```

新しい Go 関数を追加した場合は、`GetProcAddress` の第2引数に関数名を指定し、対応するラッパー関数を `Init` に登録してください。

---

## テスト

```bash
npm test
```

Jest を使ったテストが実行されます。

```js
// index.test.js の例
const { hello } = require('./index');

test('hello world test', () => {
  expect(hello("World")).toBe("Hello, World!");
});
```

---

## プロジェクト構成

```
nodegolib/
├── include/
│   ├── main.go          # Go の実装（DLL としてビルドされる）
│   ├── goshared.dll     # ビルド済み Go DLL
│   └── goshared.h       # cgo が自動生成するヘッダ
├── addon.cc             # C++ Addon（DLL を Node.js に橋渡し）
├── binding.gyp          # node-gyp のビルド設定
├── index.js             # Node.js 公開 API
├── index.test.js        # Jest テスト
└── package.json
```

---

## ビルドフロー詳細

```
include/main.go
    │
    │  go build -buildmode=c-shared
    ▼
include/goshared.dll  +  include/goshared.h
    │
    │  (binding.gyp でビルド時に build/Release/ へコピー)
    │
addon.cc  ──(node-gyp rebuild)──▶  build/Release/hello.node
    │
index.js  ──(require('bindings')('hello'))──▶  hello.node をロード
```

---

## npm パッケージとして公開する場合

プライベートパッケージとして公開するには:

```bash
npm publish --access=private
```

---

## 参考

- [node-addon-examples](https://github.com/nodejs/node-addon-examples/tree/main/src/1-getting-started/1_hello_world/node-addon-api)
- [node-addon-api (N-API C++ wrapper)](https://github.com/nodejs/node-addon-api)
- [cgo documentation](https://pkg.go.dev/cmd/cgo)
