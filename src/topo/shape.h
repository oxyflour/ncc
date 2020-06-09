#include <napi.h>
#include <TopoDS.hxx>

class Shape : public Napi::ObjectWrap<Shape> {
public:
    Shape(const Napi::CallbackInfo &info);
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const TopoDS_Shape &shape);

    TopoDS_Shape shape;
    Napi::Value Type(const Napi::CallbackInfo &info);
    Napi::Value Bound(const Napi::CallbackInfo &info);
    Napi::Value Find(const Napi::CallbackInfo &info);

    Napi::Value GetLinearProps(const Napi::CallbackInfo &info);
    Napi::Value GetSurfaceProps(const Napi::CallbackInfo &info);
    Napi::Value GetVolumeProps(const Napi::CallbackInfo &info);

    Napi::Value Test(const Napi::CallbackInfo &info);
private:
    static Napi::FunctionReference constructor;
};
