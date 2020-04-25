#include "brep.h"

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>

#include "../topo/shape.h"

Napi::Value LoadBrep(const Napi::CallbackInfo &info) {
    std::string file = info[0].As<Napi::String>();
    TopoDS_Shape shape;
    BRep_Builder builder;
    if (!BRepTools::Read(shape, file.c_str(), builder)) {
        auto msg = std::string("failed to read ") + file;
        Napi::Error::New(info.Env(), msg).ThrowAsJavaScriptException();
    }
    return Shape::Create(shape);
}

Napi::Value SaveBrep(const Napi::CallbackInfo &info) {
    std::string file = info[0].As<Napi::String>();
    auto &shape = Shape::Unwrap(info[1].As<Napi::Object>())->shape;
    if (!BRepTools::Write(shape, file.c_str())) {
        auto msg = std::string("failed to write ") + file;
        Napi::Error::New(info.Env(), msg).ThrowAsJavaScriptException();
    }
}
