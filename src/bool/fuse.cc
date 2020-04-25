#include <BRepAlgoAPI_Fuse.hxx>
#include "fuse.h"

#include "../topo/shape.h"

Napi::Value fuse(const Napi::CallbackInfo &info) {
    BRepAlgoAPI_Fuse fuse;
    TopTools_ListOfShape args, tools;

    fuse.SetArguments(Shape::arr2list(info[0].As<Napi::Array>(), args));
    fuse.SetTools(Shape::arr2list(info[1].As<Napi::Array>(), tools));
    if (info.Length() > 2) {
        auto opts = info[2].As<Napi::Object>();
        if (opts.Has("fuzzyValue")) {
            auto fuzzyValue = opts.Get("fuzzyValue").As<Napi::Number>().DoubleValue();
            fuse.SetFuzzyValue(fuzzyValue);
        }
        // TODO:
        // https://www.opencascade.com/doc/occt-7.3.0/overview/html/occt_user_guides__boolean_operations.html
    }

    fuse.Build();
    if (fuse.HasErrors()) {
        // TODO
        Napi::Error::New(info.Env(), "Fuse Failed").ThrowAsJavaScriptException();
    }

    return Shape::NewInstance(fuse.Shape());
}
