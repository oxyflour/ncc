#include "../../utils.h"
#include "box.h"

void Box::Init(Napi::Env env, Napi::Object exports) {
    auto func = DefineClass(env, "Box", {
        InstanceAccessor("p0", &Box::GetP0, NULL),
        InstanceAccessor("p1", &Box::GetP1, NULL),
    });
    exports.Set("Box", func);
}

Box::Box(const Napi::CallbackInfo &info): Napi::ObjectWrap<Box>(info) {
    if (info.Length() < 2) {
    }
    p0 = obj2pt(info[0].As<Napi::Object>());
    p1 = obj2pt(info[1].As<Napi::Object>());
    shape = BRepPrimAPI_MakeBox(p0, p1).Shape();
    auto self = info.This().As<Napi::Object>();
}

Napi::Value Box::GetP0(const Napi::CallbackInfo &info) {
    return pt2obj(info.Env(), p0);
}
Napi::Value Box::GetP1(const Napi::CallbackInfo &info) {
    return pt2obj(info.Env(), p1);
}
