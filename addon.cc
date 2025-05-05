#include <napi.h>
#include <windows.h>

Napi::String LoadDLL(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  HMODULE hDLL = LoadLibraryA("include/goshared.dll");

  if (!hDLL) {
    Napi::Error::New(env, "Failed to load DLL").ThrowAsJavaScriptException();
    return Napi::String::New(env, "error");
  }
  // Load the function from the DLL
  FARPROC pFunc = GetProcAddress(hDLL, "Execute");
  
  typedef const char* (*FuncType)(const char*);
  FuncType dllFunction = reinterpret_cast<FuncType>(pFunc);
  const char* result = dllFunction("addon.cc");

  return Napi::String::New(env, result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, LoadDLL));
  return exports;
}

NODE_API_MODULE(hello, Init)