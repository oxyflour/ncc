#include "step.h"

#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>

#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <Standard_CString.hxx>
#include <StepShape_ManifoldSolidBrep.hxx>

#include "../topo/shape.h"

auto GetSolidNames(STEPControl_Reader &reader) {
    TopExp_Explorer exp;
    auto trans = reader.WS()->TransferReader();
    for (exp.Init(reader.Shape(), TopAbs_ShapeEnum::TopAbs_SOLID); exp.More(); exp.Next()) {
        auto &shape = exp.Current();
        auto ent = trans->EntityFromShapeResult(shape, 1);
        if (!ent.IsNull()) {
            auto prop = Handle(StepShape_ManifoldSolidBrep)::DownCast(ent);
            auto &meta = Shape::MetaMap[shape.HashCode(0x0fffffff)];
            meta["ManifoldSolidBrep"] = prop->Name()->ToCString();
        }
    }
}

Napi::Value LoadStep(const Napi::CallbackInfo &info) {
    std::string file = info[0].As<Napi::String>();
    STEPControl_Reader reader;
    auto stat = reader.ReadFile(file.c_str());
    if (stat != IFSelect_RetDone) {
        auto msg = std::string("read from ") + file + " failed";
        Napi::Error::New(info.Env(), msg).ThrowAsJavaScriptException();
        return info.Env().Undefined();
    } else {
        reader.TransferRoot();
        GetSolidNames(reader);
        return Shape::Create(reader.Shape());
    }
}

Napi::Value SaveStep(const Napi::CallbackInfo &info) {
    std::string file = info[0].As<Napi::String>();
    STEPControl_Writer writer;
    auto shape = Shape::Unwrap(info[1].As<Napi::Object>())->shape;
    auto stat = writer.Transfer(shape, STEPControl_StepModelType::STEPControl_AsIs);
    if (stat != IFSelect_RetDone) {
        auto msg = std::string("write to ") + file + " failed";
        Napi::Error::New(info.Env(), msg).ThrowAsJavaScriptException();
    } else {
        writer.Write(file.c_str());
    }
    return info.Env().Undefined();
}
