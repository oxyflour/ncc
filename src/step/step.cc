#include "step.h"

#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

#include "../topo/shape.h"

Napi::Value LoadStep(const Napi::CallbackInfo &info) {
    STEPControl_Reader reader;
    std::string file = info[0].As<Napi::String>();
    auto stat = reader.ReadFile(file.c_str());
    if (stat != IFSelect_RetDone) {
        // TODO
        Napi::Error::New(info.Env(), "read step failed").ThrowAsJavaScriptException();
    } else {
        return Shape::NewInstance(reader.OneShape());
    }
}

Napi::Value SaveStep(const Napi::CallbackInfo &info) {
    STEPControl_Writer writer;
    auto shape = Shape::Unwrap(info[1].As<Napi::Object>())->shape;
    auto stat = writer.Transfer(shape, STEPControl_StepModelType::STEPControl_AsIs);
    if (stat != IFSelect_RetDone) {
        // TODO
        Napi::Error::New(info.Env(), "write step failed").ThrowAsJavaScriptException();
    } else {
        std::string file = info[0].As<Napi::String>();
        writer.Write(file.c_str());
    }
}
