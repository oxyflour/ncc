#include "utils.h"

gp_Pnt obj2pt(Napi::Object obj) {
    return gp_Pnt(
        obj.Get("x").As<Napi::Number>().DoubleValue(),
        obj.Get("y").As<Napi::Number>().DoubleValue(),
        obj.Get("z").As<Napi::Number>().DoubleValue()
    );
}

Napi::Object pt2obj(Napi::Env env, gp_Pnt &pt) {
    auto obj = Napi::Object::New(env);
    obj.Set("x", Napi::Number::New(env, pt.X()));
    obj.Set("y", Napi::Number::New(env, pt.Y()));
    obj.Set("z", Napi::Number::New(env, pt.Z()));
    return obj;
}
