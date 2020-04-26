#include "builder.h"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Builder.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>

#include <gp_Pln.hxx>
#include <gp_Dir.hxx>

#include "../utils.h"
#include "../topo/shape.h"

Napi::Value MakeEdge(const Napi::CallbackInfo &info) {
    if (info.Length() == 2) {
        auto p0 = obj2pt(info[0]), p1 = obj2pt(info[1]);
        return Shape::Create(BRepBuilderAPI_MakeEdge(p0, p1).Shape());
    } else {
        Napi::Error::New(info.Env(), "not implemented yet").ThrowAsJavaScriptException();
    }
}

Napi::Value MakeFace(const Napi::CallbackInfo &info) {
    if (info.Length() == 2) {
        auto pos = obj2pt(info[0]), dir = obj2pt(info[1]);
        auto plane = gp_Pln(pos, gp_Dir(dir.XYZ()));
        return Shape::Create(BRepBuilderAPI_MakeFace(plane).Shape());
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
