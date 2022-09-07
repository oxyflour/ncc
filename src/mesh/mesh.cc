#include "mesh.h"

#include <BRepMesh_IncrementalMesh.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Face.hxx>

#include "../topo/shape.h"

// https://github.com/FreeCAD/FreeCAD/blob/a4fa45b589ffd896d1a5c3a16c902c81e0ab1a27/src/Mod/PartDesign/Gui/ViewProviderAddSub.cpp
Napi::Value CreateMesh(const Napi::CallbackInfo &info) {
    auto &shape = Shape::Unwrap(info[0].As<Napi::Object>())->shape;
    IMeshTools_Parameters params;
    if (info.Length() > 1) {
        auto opts = info[1].As<Napi::Object>();
        params.Angle = opts.Has("angle") ? opts.Get("angle").As<Napi::Number>().DoubleValue() : 0.5;
        params.Deflection = opts.Has("deflection") ? opts.Get("deflection").As<Napi::Number>().DoubleValue() : 0.1;
    }
    BRepMesh_IncrementalMesh mesher(shape, params);

    TopLoc_Location loc;
    shape.Location(loc);
    int posNum = 0, idxNum = 0;
    for (TopExp_Explorer ex(shape, TopAbs_ShapeEnum::TopAbs_FACE); ex.More(); ex.Next()) {
        auto mesh = BRep_Tool::Triangulation(TopoDS::Face(ex.Current()), loc);
        posNum += mesh->NbNodes();
        idxNum += mesh->NbTriangles();
    }

    auto pos = Napi::Float32Array::New(info.Env(), posNum * 3);
    auto idx = Napi::Uint32Array::New(info.Env(), idxNum * 3);
    auto norm = Napi::Float32Array::New(info.Env(), posNum * 3);
    posNum = idxNum = 0;
    shape.Location(loc);
    auto isId = loc.IsIdentity();
    auto trans = loc.Transformation();
    for (TopExp_Explorer ex(shape, TopAbs_ShapeEnum::TopAbs_FACE); ex.More(); ex.Next()) {
        auto face = TopoDS::Face(ex.Current());
        auto mesh = BRep_Tool::Triangulation(face, loc);
        auto orient = face.Orientation();
        auto start = posNum;
        for (int i = 0, n = mesh->NbNodes(); i < n; i ++, posNum ++) {
            auto s = posNum * 3;
            auto p = mesh->Node(i + 1);
            if (!isId) {
                p.Transform(trans);
            }
            pos[s    ] = p.X();
            pos[s + 1] = p.Y();
            pos[s + 2] = p.Z();
            auto v = mesh->Normal(i + 1);
            norm[s    ] = v.X();
            norm[s + 1] = v.Y();
            norm[s + 2] = v.Z();
        }
        for (int i = 0, n = mesh->NbTriangles(); i < n; i ++, idxNum ++) {
            auto s = idxNum * 3;
            auto m = mesh->Triangle(i + 1);
            int a, b, c;
            m.Get(a, b, c);
            if (orient != TopAbs_FORWARD) {
                std::swap(a, b);
            }
            idx[s    ] = a - 1 + start;
            idx[s + 1] = b - 1 + start;
            idx[s + 2] = c - 1 + start;
        }
    }

    auto ret = Napi::Object::New(info.Env());
    ret.Set("positions", pos);
    ret.Set("indices", idx);
    ret.Set("normals", pos);
    return ret;
}
