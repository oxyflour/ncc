#include <napi.h>
#include "brep/prime/box.h"
#include "bool/fuse.h"
#include "topo/shape.h"
#include "step/step.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    auto prime = Napi::Object::New(env);
    prime.Set("makeBox", Napi::Function::New(env, MakeBox));
    auto brep = Napi::Object::New(env);
    brep.Set("prime", prime);
    exports.Set("brep", brep);

    auto boolean = Napi::Object::New(env);
    boolean.Set("fuse", Napi::Function::New(env, fuse));
    exports.Set("bool", boolean);

    auto step = Napi::Object::New(env);
    step.Set("save", Napi::Function::New(env, SaveStep));
    step.Set("load", Napi::Function::New(env, LoadStep));
    exports.Set("step", step);

    Shape::Init(env, exports);
    return exports;
}

NODE_API_MODULE(testaddon, Init)
