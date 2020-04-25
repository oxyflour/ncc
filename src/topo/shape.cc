#include "shape.h"

#include "../utils.h"

Shape::Shape(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Shape>(info) {
}

void Shape::Init(Napi::Env env, Napi::Object exports) {
    auto func = DefineClass(env, "Shape", {
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Shape", func);
}

Napi::Value Shape::Create(const TopoDS_Shape &shape) {
    auto inst = constructor.New({ });
    Shape::Unwrap(inst)->shape = shape;
    return inst;
}

Napi::FunctionReference Shape::constructor;
