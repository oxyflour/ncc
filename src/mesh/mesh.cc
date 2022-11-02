#include "mesh.h"

#include <BRepMesh_IncrementalMesh.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include "../topo/shape.h"

template <typename T>
auto getPos(T pos, int idx) {
    auto n = idx * 3;
    return gp_XYZ(pos[n], pos[n + 1], pos[n + 2]);
}
auto getNorm(gp_XYZ p1, gp_XYZ p2, gp_XYZ p3) {
    auto n = (p2 - p1) ^ (p3 - p2);
    auto s = n.Modulus();
    if (s > gp::Resolution()) {
        n.Divide(s);
    } else {
        n.SetCoord(0, 0, 0);
    }
    return n;
}

Napi::Value CreateTopo(const Napi::CallbackInfo &info) {
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
    auto isId = loc.IsIdentity();
    auto trans = loc.Transformation();
    int faceIndex = 0;
    std::vector<float> positions, normals;
    std::vector<int> indices;
    auto faces = Napi::Array::New(info.Env());
    for (TopExp_Explorer ex(shape, TopAbs_ShapeEnum::TopAbs_FACE); ex.More(); ex.Next()) {
        auto face = TopoDS::Face(ex.Current());
        auto mesh = BRep_Tool::Triangulation(face, loc);

        auto pos = Napi::Float32Array::New(info.Env(), mesh->NbNodes() * 3);
        auto idx = Napi::Uint32Array::New(info.Env(), mesh->NbTriangles() * 3);
        auto norm = Napi::Float32Array::New(info.Env(), mesh->NbNodes() * 3);
        auto normNum = std::vector<int>(mesh->NbNodes() * 3);

        auto orient = face.Orientation();
        auto start = positions.size() / 3;
        for (int i = 0, n = mesh->NbNodes(); i < n; i ++) {
            auto s = i * 3;
            auto p = mesh->Node(i + 1);
            if (!isId) {
                p.Transform(trans);
            }
            positions.push_back(pos[s    ] = p.X());
            positions.push_back(pos[s + 1] = p.Y());
            positions.push_back(pos[s + 2] = p.Z());
        }
        for (int i = 0, n = mesh->NbTriangles(); i < n; i ++) {
            auto s = i * 3;
            auto m = mesh->Triangle(i + 1);
            int a, b, c;
            m.Get(a, b, c);
            if (orient != TopAbs_FORWARD) {
                std::swap(a, b);
            }
            indices.push_back((idx[s    ] = a - 1) + start);
            indices.push_back((idx[s + 1] = b - 1) + start);
            indices.push_back((idx[s + 2] = c - 1) + start);
            auto nr = getNorm(
                getPos(pos, idx[s]),
                getPos(pos, idx[s + 1]),
                getPos(pos, idx[s + 2]));
            for (int d = s; d < s + 3; d ++) {
                int q = idx[d] * 3,
                    c = normNum[q];
                normNum[q] ++;
                norm[q] = (norm[q] * c + nr.X()) / (c + 1);
                q ++;
                norm[q] = (norm[q] * c + nr.Y()) / (c + 1);
                q ++;
                norm[q] = (norm[q] * c + nr.Z()) / (c + 1);
                q ++;
            }
        }
        for (int i = 0, n = mesh->NbNodes(); i < n; i ++) {
            auto s = i * 3;
            normals.push_back(norm[s    ]);
            normals.push_back(norm[s + 1]);
            normals.push_back(norm[s + 2]);
        }
        auto ret = Napi::Object::New(info.Env());
        ret.Set("positions", pos);
        ret.Set("indices", idx);
        ret.Set("normals", norm);
        faces.Set(faceIndex ++, ret);
    }

    auto geom = Napi::Object::New(info.Env());
    auto pos = Napi::Float32Array::New(info.Env(), positions.size());
    for (int i = 0, n = positions.size(); i < n; i ++) pos[i] = positions[i];
    geom.Set("positions", pos);
    auto idx = Napi::Uint32Array::New(info.Env(), indices.size());
    for (int i = 0, n = indices.size(); i < n; i ++) idx[i] = indices[i];
    geom.Set("indices", idx);
    auto norm = Napi::Float32Array::New(info.Env(), normals.size());
    for (int i = 0, n = normals.size(); i < n; i ++) norm[i] = normals[i];
    geom.Set("normals", norm);

    auto edgeIndex = 0;
    auto edges = Napi::Array::New(info.Env());
    for (TopExp_Explorer ex(shape, TopAbs_ShapeEnum::TopAbs_EDGE); ex.More(); ex.Next()) {
        auto edge = TopoDS::Edge(ex.Current());
        BRepMesh_IncrementalMesh mesher(edge, params);
        auto mesh = BRep_Tool::Polygon3D(edge, loc);
        if (!mesh.IsNull()) {
            auto arr = mesh->Nodes();
            auto pos = Napi::Float32Array::New(info.Env(), arr.Size() * 3);
            for (auto i = arr.Lower(); i <= arr.Upper(); i ++) {
                auto p = arr.Value(i).Transformed(loc);
                pos[(i - 1) * 3    ] = p.X();
                pos[(i - 1) * 3 + 1] = p.Y();
                pos[(i - 1) * 3 + 2] = p.Z();
            }
            auto ret = Napi::Object::New(info.Env());
            ret.Set("positions", pos);
            edges.Set(edgeIndex ++, ret);
        }
    }

    auto vertIndex = 0;
    auto verts = Napi::Array::New(info.Env());
    for (TopExp_Explorer ex(shape, TopAbs_ShapeEnum::TopAbs_VERTEX); ex.More(); ex.Next()) {
        auto vert = TopoDS::Vertex(ex.Current());
        auto pt = BRep_Tool::Pnt(vert);
        auto pos = Napi::Array::New(info.Env());
        pos.Set((uint32_t) 0, pt.X());
        pos.Set(1, pt.Y());
        pos.Set(2, pt.Z());
        auto ret = Napi::Object::New(info.Env());
        ret.Set("positions", pos);
        verts.Set(vertIndex ++, ret);
    }

    auto ret = Napi::Object::New(info.Env());
    ret.Set("faces", faces);
    ret.Set("edges", edges);
    ret.Set("verts", verts);
    ret.Set("geom", geom);
    return ret;
}

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
    std::vector<int> normNum(posNum * 3);

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
            auto nr = getNorm(
                getPos(pos, idx[s]),
                getPos(pos, idx[s + 1]),
                getPos(pos, idx[s + 2]));
            for (int d = s; d < s + 3; d ++) {
                int q = idx[d] * 3,
                    c = normNum[q];
                normNum[q] ++;
                norm[q] = (norm[q] * c + nr.X()) / (c + 1);
                q ++;
                norm[q] = (norm[q] * c + nr.Y()) / (c + 1);
                q ++;
                norm[q] = (norm[q] * c + nr.Z()) / (c + 1);
                q ++;
            }
        }
    }

    auto ret = Napi::Object::New(info.Env());
    ret.Set("positions", pos);
    ret.Set("indices", idx);
    ret.Set("normals", norm);
    return ret;
}
