#include "shape.h"
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <TopExp_Explorer.hxx>

#include "../utils.h"

Shape::Shape(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Shape>(info) {
    if (info.Length() >= 1 && info[0].IsArray()) {
        auto arr = info[0].As<Napi::Array>();
        TopoDS_Compound comp;
        BRep_Builder builder;
        builder.MakeCompound(comp);
        for (int i = 0; i < arr.Length(); i ++) {
            auto shape = Shape::Unwrap(arr.Get(i).As<Napi::Object>())->shape;
            builder.Add(comp, shape);
        }
        shape = comp;
    }
}

void Shape::Init(Napi::Env env, Napi::Object exports) {
    auto func = DefineClass(env, "Shape", {
        InstanceAccessor("type", &Shape::GetType, NULL),
        InstanceMethod("find", &Shape::find),
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

Napi::Value Shape::GetType(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), shape.ShapeType());
}

Napi::Value Shape::find(const Napi::CallbackInfo &info) {
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
