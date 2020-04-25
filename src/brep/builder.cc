#include "builder.h"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Face.hxx>
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
