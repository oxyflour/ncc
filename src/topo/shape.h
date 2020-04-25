#include <napi.h>
#include <TopoDS.hxx>

class Shape : public Napi::ObjectWrap<Shape> {
public:
    Shape(const Napi::CallbackInfo &info);
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const TopoDS_Shape &shape);

    TopoDS_Shape shape;
    Napi::Value GetType(const Napi::CallbackInfo &info);
    Napi::Value find(const Napi::CallbackInfo &info);
private:
    static Napi::FunctionReference constructor;
};
