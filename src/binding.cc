#include <napi.h>
#include "brep/prime/box.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    auto prime = Napi::Object::New(env);
    Box::Init(env, prime);

    auto brep = Napi::Object::New(env);
    brep.Set("prime", prime);

    exports.Set("brep", brep);
    return exports;
}

NODE_API_MODULE(testaddon, Init)
