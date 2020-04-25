#include <napi.h>
#include <gp_Pnt.hxx>

gp_Pnt obj2pt(Napi::Object obj);
Napi::Object pt2obj(Napi::Env env, gp_Pnt &pt);
