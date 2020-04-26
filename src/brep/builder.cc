#include "builder.h"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRep_Builder.hxx>

#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>

#include <gp_Pln.hxx>
#include <gp_Dir.hxx>

#include "../utils.h"
#include "../topo/shape.h"

Napi::Value MakeFace(const Napi::CallbackInfo &info) {
    if (info.Length() == 2 && info[0].IsObject() && info[1].IsObject()) {
        auto pos = obj2pt(info[0].As<Napi::Object>()),
            dir = obj2pt(info[1].As<Napi::Object>());
        auto plane = gp_Pln(pos, gp_Dir(dir.XYZ()));
        auto face = BRepBuilderAPI_MakeFace(plane);
        auto shape = TopoDS_Face(face);
        return Shape::Create(shape);
    } else {
        Napi::Error::New(info.Env(), "not implemented yet").ThrowAsJavaScriptException();
    }
}

Napi::Value MakeCompound(const Napi::CallbackInfo &info) {
    auto arr = info[0].As<Napi::Array>();
    TopoDS_Compound comp;
    BRep_Builder builder;
    builder.MakeCompound(comp);
    for (int i = 0; i < arr.Length(); i ++) {
        auto shape = Shape::Unwrap(arr.Get(i).As<Napi::Object>())->shape;
        builder.Add(comp, shape);
    }
    return Shape::Create(comp);
}
