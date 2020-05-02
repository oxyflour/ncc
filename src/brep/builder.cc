#include "builder.h"

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRep_Builder.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>

#include <gp_Pln.hxx>
#include <gp_Dir.hxx>

#include "../utils.h"
#include "../topo/shape.h"

auto fromShapes(const Napi::Array &arr, BRep_Builder &builder, TopoDS_Shape &ret) {
    for (int i = 0; i < arr.Length(); i ++) {
        builder.Add(ret, Shape::Unwrap(arr.Get(i).As<Napi::Object>())->shape);
    }
    return ret;
}

Napi::Value MakeVertex(const Napi::CallbackInfo &info) {
    auto p0 = obj2pt(info[0]);
    return Shape::Create(BRepBuilderAPI_MakeVertex(p0).Vertex());
}

Napi::Value MakeEdge(const Napi::CallbackInfo &info) {
    if (info.Length() == 2) {
        auto p0 = obj2pt(info[0]), p1 = obj2pt(info[1]);
        return Shape::Create(BRepBuilderAPI_MakeEdge(p0, p1).Edge());
    } else {
        Napi::Error::New(info.Env(), "not implemented yet").ThrowAsJavaScriptException();
    }
}

Napi::Value MakeFace(const Napi::CallbackInfo &info) {
    if (info.Length() == 2) {
        auto pos = obj2pt(info[0]), dir = obj2pt(info[1]);
        auto plane = gp_Pln(pos, gp_Dir(dir.XYZ()));
        return Shape::Create(BRepBuilderAPI_MakeFace(plane).Face());
    // TODO
    } else {
        Napi::Error::New(info.Env(), "not implemented yet").ThrowAsJavaScriptException();
    }
}

Napi::Value MakeWire(const Napi::CallbackInfo &info) {
    if (info.Length() == 1 && info[0].IsArray()) {
        TopoDS_Wire ret;
        BRep_Builder builder;
        builder.MakeWire(ret);
        return Shape::Create(fromShapes(info[0].As<Napi::Array>(), builder, ret));
    } else {
        Napi::Error::New(info.Env(), "not implemented yet").ThrowAsJavaScriptException();
    }
}

Napi::Value MakeShell(const Napi::CallbackInfo &info) {
    if (info.Length() == 1 && info[0].IsArray()) {
        TopoDS_Shell ret;
        BRep_Builder builder;
        builder.MakeShell(ret);
        return Shape::Create(fromShapes(info[0].As<Napi::Array>(), builder, ret));
    } else {
        Napi::Error::New(info.Env(), "not implemented yet").ThrowAsJavaScriptException();
    }
}

Napi::Value MakeCompound(const Napi::CallbackInfo &info) {
    if (info.Length() == 1 && info[0].IsArray()) {
        TopoDS_Compound ret;
        BRep_Builder builder;
        builder.MakeCompound(ret);
        return Shape::Create(fromShapes(info[0].As<Napi::Array>(), builder, ret));
    } else {
        Napi::Error::New(info.Env(), "not implemented yet").ThrowAsJavaScriptException();
    }
}
