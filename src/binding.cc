#include <napi.h>
#include "brep/primitive.h"
#include "brep/builder.h"
#include "brep/bool.h"
#include "brep/brep.h"
#include "topo/shape.h"
#include "step/step.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    auto brep = Napi::Object::New(env);

    auto primitive = Napi::Object::New(env);
    primitive.Set("makeSphere", Napi::Function::New(env, MakeSphere));
    primitive.Set("makeBox", Napi::Function::New(env, MakeBox));
    brep.Set("primitive", primitive);

    auto builder = Napi::Object::New(env);
    builder.Set("makeEdge", Napi::Function::New(env, MakeEdge));
    builder.Set("makeFace", Napi::Function::New(env, MakeFace));
    builder.Set("makeCompound", Napi::Function::New(env, MakeCompound));
    brep.Set("builder", builder);

    auto boolean = Napi::Object::New(env);
    boolean.Set("fuse", Napi::Function::New(env, fuse));
    boolean.Set("common", Napi::Function::New(env, common));
    boolean.Set("cut", Napi::Function::New(env, cut));
    boolean.Set("section", Napi::Function::New(env, section));
    boolean.Set("split", Napi::Function::New(env, split));
    brep.Set("bool", boolean);

    brep.Set("save", Napi::Function::New(env, SaveBrep));
    brep.Set("load", Napi::Function::New(env, LoadBrep));
    exports.Set("brep", brep);

    auto step = Napi::Object::New(env);
    step.Set("save", Napi::Function::New(env, SaveStep));
    step.Set("load", Napi::Function::New(env, LoadStep));
    exports.Set("step", step);

    Shape::Init(env, exports);
    return exports;
}

NODE_API_MODULE(testaddon, Init)
