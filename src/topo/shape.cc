#include "shape.h"

#include <Bnd_Box.hxx>

#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopExp_Explorer.hxx>

#include <Geom_BSplineSurface.hxx>
#include <Geom_BSplineCurve.hxx>

#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_HSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBndLib.hxx>

#include "../utils.h"

Shape::Shape(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Shape>(info) {
}

void Shape::Init(Napi::Env env, Napi::Object exports) {
    auto func = DefineClass(env, "Shape", {
        InstanceAccessor("type", &Shape::Type, NULL),
        InstanceMethod("bound", &Shape::Bound),
        InstanceMethod("find", &Shape::Find),
        InstanceMethod("test", &Shape::Test),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    auto types = Napi::Object::New(env);
    types.Set("COMPOUND", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_COMPOUND));
    types.Set("COMPSOLID", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_COMPSOLID));
    types.Set("EDGE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_EDGE));
    types.Set("FACE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_FACE));
    types.Set("SHAPE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_SHAPE));
    types.Set("SHELL", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_SHELL));
    types.Set("SOLID", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_SOLID));
    types.Set("VERTEX", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_VERTEX));
    types.Set("WIRE", Napi::Number::New(env, TopAbs_ShapeEnum::TopAbs_WIRE));
    func.Set("types", types);

    exports.Set("Shape", func);
}

Napi::Value Shape::Type(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), shape.ShapeType());
}

Napi::Value Shape::Test(const Napi::CallbackInfo &info) {
    auto trans = gp_Trsf();
    trans.SetTranslation(gp_Vec(0, 1, 0));
    shape.Move(trans);
    /*
    auto surface = BRepAdaptor_Surface(face);
    auto bspline = surface.BSpline();
    auto nu = bspline->NbUPoles(), nv = bspline->NbVPoles();
    for (int i = 1; i <= nu; i ++) {
        auto u = (i - 1) / (nu - 1);
        for (int j = 1; j <= nv; j ++) {
            auto v = (j - 1) / (nv - 1);
            auto pt = bspline->Value(u, v);
            pt.SetX(pt.X() + 0.1);
            int u0, v0, u1, v1;
            bspline->MovePoint(u, v, pt, i, i, j, j, u0, v0, u1, v1);
        }
    }
    return Shape::Create(BRepBuilderAPI_MakeFace(bspline,
        surface.FirstUParameter(), surface.LastUParameter(),
        surface.FirstVParameter(), surface.LastVParameter(), 1e-3));
     */
    return info.This();
}

Napi::Value Shape::Bound(const Napi::CallbackInfo &info) {
    Bnd_Box box;
    BRepBndLib::Add(shape, box);

    double xmin, ymin, zmin, xmax, ymax, zmax;
    box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    auto min = Napi::Object::New(info.Env());
    min.Set("x", xmin);
    min.Set("y", ymin);
    min.Set("z", zmin);
    auto max = Napi::Object::New(info.Env());
    max.Set("x", xmax);
    max.Set("y", ymax);
    max.Set("z", zmax);
    auto ret = Napi::Object::New(info.Env());
    ret.Set("min", min);
    ret.Set("max", max);
    return ret;
}

Napi::Value Shape::Find(const Napi::CallbackInfo &info) {
    auto type = static_cast<TopAbs_ShapeEnum>(info[0].As<Napi::Number>().Int32Value());
    auto arr = Napi::Array::New(info.Env());
    TopExp_Explorer exp;
    int i = 0;
    for (exp.Init(shape, type); exp.More(); exp.Next(), i ++) {
        arr.Set(i, Shape::Create(exp.Current()));
    }
    return arr;
}

Napi::Value Shape::Create(const TopoDS_Shape &shape) {
    auto inst = constructor.New({ });
    Shape::Unwrap(inst)->shape = shape;
    return inst;
}

Napi::FunctionReference Shape::constructor;
