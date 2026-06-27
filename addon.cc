#include <napi.h>
#include <windows.h>
#include <string>

// グローバルでDLLハンドルを保持
HMODULE g_hDLL = nullptr;

void EnsureDLLLoaded()
{
  if (!g_hDLL)
  {
    g_hDLL = LoadLibraryA("include/goshared.dll");
  }
}

// AsyncWorker to handle the asynchronous execution of the DLL function
class ExecutePromiseWorker : public Napi::AsyncWorker
{
public:
  ExecutePromiseWorker(Napi::Env env, std::string input, Napi::Promise::Deferred deferred)
      : Napi::AsyncWorker(env), input_(input), deferred_(deferred) {}

  void Execute() override
  {
    EnsureDLLLoaded();
    FARPROC pFunc = GetProcAddress(g_hDLL, "ExecuteAsync");
    if (!pFunc)
    {
      error_ = "Failed to find function in DLL";
      return;
    }
    typedef const char *(*FuncType)(const char *);
    FuncType dllFunction = reinterpret_cast<FuncType>(pFunc);
    const char *res = dllFunction(input_.c_str());
    if (res)
    {
      result_ = res;
      free((void *)res);
    }
    else
    {
      error_ = "DLL function returned null";
    }
  }

  void OnOK() override
  {
    Napi::HandleScope scope(Env());
    if (error_.empty())
    {
      deferred_.Resolve(Napi::String::New(Env(), result_));
    }
    else
    {
      deferred_.Reject(Napi::Error::New(Env(), error_).Value());
    }
  }

  void OnError(const Napi::Error &e) override
  {
    Napi::HandleScope scope(Env());
    deferred_.Reject(e.Value());
  }

private:
  std::string input_;
  std::string result_;
  std::string error_;
  Napi::Promise::Deferred deferred_;
};

Napi::Value ExecuteAsyncPromise(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsString())
  {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string input = info[0].As<Napi::String>().Utf8Value();
  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
  (new ExecutePromiseWorker(env, input, deferred))->Queue();
  return deferred.Promise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "executeAsyncPromise"),
              Napi::Function::New(env, ExecuteAsyncPromise));
  return exports;
}

NODE_API_MODULE(addon, Init)