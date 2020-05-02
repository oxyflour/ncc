#include "shape.h"
#include <TopExp_Explorer.hxx>

#include "../utils.h"

Shape::Shape(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Shape>(info) {
}

void Shape::Init(Napi::Env env, Napi::Object exports) {
    auto func = DefineClass(env, "Shape", {
        InstanceAccessor("type", &Shape::Type, NULL),
        InstanceMethod("find", &Shape::Find),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    auto types = Napi::Object::New(env);
    types.Set("COMPOUND", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_COMPOUND));
    types.Set("COMPSOLID", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_COMPSOLID));
    types.Set("EDGE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_EDGE));
    types.Set("FACE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_FACE));
    types.Set("SHAPE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_SHAPE));
    types.Set("SHELL", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_SHELL));
    types.Set("SOLID", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_SOLID));
    types.Set("VERTEX", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_VERTEX));
    types.Set("WIRE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_WIRE));
    func.Set("types", types);

    exports.Set("Shape", func);
}

Napi::Value Shape::Type(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), shape.ShapeType());
}

Napi::Value Shape::Find(const Napi::CallbackInfo &info) {
    auto type = static_cast<TopAbs_ShapeEnum>(info[0].As<Napi::Number>().Int32Value());
    auto arr = Napi::Array::New(info.Env());
    TopExp_Explorer exp;
    int i = 0;
    for (exp.Init(shape, type); exp.More(); exp.Next(), i ++) {
        arr.Set(i, Shape::Create(exp.Current()));
    }
    return arr;
}

Napi::Value Shape::Create(const TopoDS_Shape &shape) {
    auto inst = constructor.New({ });
    Shape::Unwrap(inst)->shape = shape;
    return inst;
}

Napi::FunctionReference Shape::constructor;
