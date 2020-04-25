cmake_minimum_required(VERSION 3.10)

project(binding)
add_definitions(-DNAPI_VERSION=3)
include_directories(${CMAKE_JS_INC})
file(GLOB SOURCE_FILES "src/*.cc" "src/**/*.cc"  "src/**/**/*.cc" "src/step/step.cc")

# you must have this before `add_library`
link_directories(${CMAKE_SOURCE_DIR}/deps/occt/lin64/gcc/lib)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

execute_process(
    COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR)
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE "deps/occt/include/opencascade")
target_link_libraries(${PROJECT_NAME} TKTObjDRAW.a TKQADraw.a TKXDEDRAW.a TKDCAF.a TKDCAF.a TKXSDRAW.a TKViewerTest.a TKTopTest.a TKDraw.a TKXDESTEP.a TKBinXCAF.a TKXmlXCAF.a TKXDEIGES.a TKXCAF.a TKIGES.a TKSTEP.a TKSTEP209.a TKSTEPAttr.a TKSTEPBase.a TKXSBase.a TKStd.a TKStdL.a TKXml.a TKBin.a TKXmlL.a TKBinL.a TKCAF.a TKXCAF.a TKLCAF.a TKCDF.a TKMeshVS.a TKOpenGl.a TKV3d.a TKService.a TKXMesh.a TKMesh.a TKOffset.a TKFeat.a TKFillet.a TKHLR.a TKBool.a TKBO.a TKShHealing.a TKPrim.a TKTopAlgo.a TKGeomAlgo.a TKBRep.a TKGeomBase.a TKG3d.a TKG2d.a TKMath.a TKernel.a)
