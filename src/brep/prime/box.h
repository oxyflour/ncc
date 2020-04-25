#include <BRepPrimAPI_MakeBox.hxx>
#include <napi.h>

class Box : public Napi::ObjectWrap<Box> {
public:
    static void Init(Napi::Env env, Napi::Object exports);
    Box(const Napi::CallbackInfo &info);
private:
    TopoDS_Shape shape;
    gp_Pnt p0;
    gp_Pnt p1;
    Napi::Value GetP0(const Napi::CallbackInfo &info);
    Napi::Value GetP1(const Napi::CallbackInfo &info);
};
