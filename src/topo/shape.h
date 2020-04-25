#include <napi.h>
#include <TopoDS.hxx>

class Shape : public Napi::ObjectWrap<Shape> {
public:
    Shape(const Napi::CallbackInfo &info);
    static void Init(Napi::Env env, Napi::Object exports);
    static TopoDS_ListOfShape &arr2list(Napi::Array arr, TopoDS_ListOfShape &list);
    static Napi::Value NewInstance(const TopoDS_Shape &shape);
    TopoDS_Shape shape;
private:
    static Napi::FunctionReference constructor;
};
