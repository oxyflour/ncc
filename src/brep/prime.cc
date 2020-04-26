#include "prime.h"
#include <BRepPrimAPI_MakeBox.hxx>

#include "../topo/shape.h"
#include "../utils.h"

Napi::Value MakeBox(const Napi::CallbackInfo &info) {
    auto p0 = obj2pt(info[0]), p1 = obj2pt(info[1]);
    auto shape = BRepPrimAPI_MakeBox(p0, p1).Shape();
    return Shape::Create(shape);
}
