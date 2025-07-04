cmake_minimum_required(VERSION 3.22.1)

project("openxr-native-project")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-DXR_USE_PLATFORM_ANDROID=1 -DXR_USE_GRAPHICS_API_OPENGL_ES=1 -fexceptions -W -Wall -DDARWIN_NO_CARBON -DFT2_BUILD_LIBRARY)

file(GLOB LOCAL_HEADERS "*.h")
file(GLOB LOCAL_SOURCE "*.cpp")

file(GLOB DEMO_HEADERS "*.h")
file(GLOB DEMO_SOURCE "*.cpp")

file(GLOB GFX_WRAPPER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}openxr_loader/include/common/gfxwrapper_opengl.c)

#**************************************************add Engine
add_library(lib_opencv SHARED IMPORTED)
set(OpenCV_DIR "D:/rokid-libs/OpenCV-android-sdk/sdk/native")
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)
message(${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)

add_library(GlobalLibs INTERFACE)
target_include_directories(GlobalLibs INTERFACE
        ${OpenCV_DIR}/jni/include
)
# assimp
add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/third_party/assimp/lib/${ANDROID_ABI}/libassimp.so)


target_link_libraries(GlobalLibs INTERFACE lib_opencv assimp)
#
#add_subdirectory(ObjectTracking)

#**************************************************endof add Engine

set(OPENXR_LOADER_H_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openxr_loader/include)
set(THIRD_PARTY_H_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

#set(CVF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/AREngine/Basic/asset/cvf/)
#set(ARENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/AREngine/)
set(ARENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../cadar/AREngine/)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${OPENXR_LOADER_H_DIR}
                    ${THIRD_PARTY_H_DIR}
                    ${THIRD_PARTY_H_DIR}/assimp/include
                    ${THIRD_PARTY_H_DIR}/freetype-2.13.0/include
                    ${ANDROID_NDK}/sources/android/native_app_glue

                    ${CVF_DIR}
                    ${ARENGINE_DIR}/
                    ${OpenCV_DIR}/jni/include
)

add_subdirectory(${ARENGINE_DIR}/Basic ${CMAKE_BINARY_DIR}/AREngine/Basic)

add_subdirectory(${ARENGINE_DIR}/ObjectTracking2 ${CMAKE_BINARY_DIR}/AREngine/ObjectTracking2)

# glue
add_library(app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# oepnxr_loader.so
#add_library(openxr_loader SHARED IMPORTED)
#set_target_properties(openxr_loader PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/openxr_loader/lib/${ANDROID_ABI}/libopenxr_loader.so)


# imgui
add_library(imgui SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui.cpp)

# freetype
add_library(freetype SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/autofit/autofit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftbase.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftbbox.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftbdf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftbitmap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftcid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftdebug.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftfstype.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftgasp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftglyph.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftgxval.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftinit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftmm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftotval.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftpatent.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftpfr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftstroke.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftsynth.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftsystem.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/fttype1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/base/ftwinfnt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/bdf/bdf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/bzip2/ftbzip2.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/cache/ftcache.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/cff/cff.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/cid/type1cid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/gzip/ftgzip.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/lzw/ftlzw.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/pcf/pcf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/pfr/pfr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/psaux/psaux.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/pshinter/pshinter.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/psnames/psmodule.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/raster/raster.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/sdf/sdf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/sfnt/sfnt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/smooth/smooth.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/svg/svg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/tools/apinames.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/truetype/truetype.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/type1/type1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/type42/type42.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.13.0/src/winfonts/winfnt.c)


add_library(openxr_demo MODULE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/platformplugin_factory.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/platformplugin_android.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsplugin_factory.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsplugin_opengles.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_android.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/openxr_loader/include/common/gfxwrapper_opengl.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/openxr_program.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/demos/shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/demos/utils.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/mesh.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/model.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/controller.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/hand.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/cube.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/ray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/demos/guiBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/demos/gui.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/text.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/demos/player.cpp

#        ${CMAKE_CURRENT_SOURCE_DIR}/app/application.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/app/arengine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test_engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/app/scene.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/app/utilsmym.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/app/scene_marker_test.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/app/scenegui.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/app/scene_engine_test_aruco.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/app/scene_engine_test_rpc.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/app/scene_marker_test_rpc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/app/scene_3dtracking_test.cpp
)





target_link_libraries(openxr_demo
        android
        EGL
        GLESv3
        app_glue
        mediandk
        assimp
        imgui
        freetype
        aaudio
        log

        #**************************************************add Engine
        Basic
        GlobalLibs
        ObjectTracking2
        #ObjectTracking
        #**************************************************endof add Engine
)

