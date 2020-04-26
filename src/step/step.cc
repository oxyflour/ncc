#include "step.h"

#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

#include "../topo/shape.h"

Napi::Value LoadStep(const Napi::CallbackInfo &info) {
    std::string file = info[0].As<Napi::String>();
    STEPControl_Reader reader;
    auto stat = reader.ReadFile(file.c_str());
    if (stat != IFSelect_RetDone) {
        auto msg = std::string("read from ") + file + "failed";
        Napi::Error::New(info.Env(), msg).ThrowAsJavaScriptException();
    } else {
        return Shape::Create(reader.OneShape());
    }
}

Napi::Value SaveStep(const Napi::CallbackInfo &info) {
    std::string file = info[0].As<Napi::String>();
    STEPControl_Writer writer;
    auto shape = Shape::Unwrap(info[1].As<Napi::Object>())->shape;
    auto stat = writer.Transfer(shape, STEPControl_StepModelType::STEPControl_AsIs);
    if (stat != IFSelect_RetDone) {
        auto msg = std::string("write to ") + file + "failed";
        Napi::Error::New(info.Env(), msg).ThrowAsJavaScriptException();
    } else {
        writer.Write(file.c_str());
    }
}
