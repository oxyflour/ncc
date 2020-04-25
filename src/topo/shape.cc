#include "shape.h"

#include "../utils.h"

TopoDS_ListOfShape &Shape::arr2list(Napi::Array arr, TopoDS_ListOfShape &list) {
    for (int i = 0; i < arr.Length(); i ++) {
        auto item = arr.Get(i).As<Napi::Object>();
        list.Append(Napi::ObjectWrap<Shape>::Unwrap(item)->shape);
    }
    return list;
}

Shape::Shape(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Shape>(info) {
}

void Shape::Init(Napi::Env env, Napi::Object exports) {
    auto func = DefineClass(env, "Shape", {
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Shape", func);
}

Napi::Value Shape::NewInstance(const TopoDS_Shape &shape) {
    auto inst = constructor.New({ });
    Shape::Unwrap(inst)->shape = shape;
    return inst;
}

Napi::FunctionReference Shape::constructor;
