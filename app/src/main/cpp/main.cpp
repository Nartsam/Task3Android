// Copyright (c) 2017-2022, The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "common.h"
#include "options.h"
#include "platformdata.h"
#include "platformplugin.h"
#include "graphicsplugin.h"
#include "openxr_program.h"
#include "demos/utils.h"


namespace{

void ShowHelp(){
    Log::Write(Log::Level::Info,"adb shell setprop debug.xr.graphicsPlugin OpenGLES|Vulkan");
    Log::Write(Log::Level::Info,"adb shell setprop debug.xr.formFactor Hmd|Handheld");
    Log::Write(Log::Level::Info,"adb shell setprop debug.xr.viewConfiguration Stereo|Mono");
    Log::Write(Log::Level::Info,"adb shell setprop debug.xr.blendMode Opaque|Additive|AlphaBlend");
    Log::Write(Log::Level::Info,"adb shell setprop persist.log.tag V");
}

bool UpdateOptionsFromSystemProperties(Options &options){
    char value[PROP_VALUE_MAX]={};
    if(__system_property_get("debug.xr.graphicsPlugin",value)!=0){
        options.GraphicsPlugin=value;
    }

    // Check for required parameters.
    if(options.GraphicsPlugin.empty()){
        Log::Write(Log::Level::Warning,__FILE__,__LINE__,"GraphicsPlugin Default OpenGLES");
        options.GraphicsPlugin="OpenGLES";
    }
    return true;
}
}  // namespace

struct AndroidAppState{
    ANativeWindow *NativeWindow=nullptr;
    bool Resumed=false;
    std::shared_ptr<IOpenXrProgram> program;
};

/**
 * Process the next main command.
 */
static void app_handle_cmd(struct android_app *app,int32_t cmd){
    AndroidAppState *appState=(AndroidAppState *)app->userData;

    switch(cmd){
        // There is no APP_CMD_CREATE. The ANativeActivity creates the
        // application thread from onCreate(). The application thread
        // then calls android_main().
        case APP_CMD_START:{
            Log::Write(Log::Level::Info,__FILE__,__LINE__,"onStart()");
            break;
        }
        case APP_CMD_RESUME:{
            Log::Write(Log::Level::Info,__FILE__,__LINE__,"onResume()");
            appState->Resumed=true;
            if(appState->program.get()){
            }
            break;
        }
        case APP_CMD_PAUSE:{
            Log::Write(Log::Level::Info,__FILE__,__LINE__,"onPause()");
            appState->Resumed=false;
            if(appState->program.get()){
            }
            break;
        }
        case APP_CMD_STOP:{
            Log::Write(Log::Level::Info,__FILE__,__LINE__,"onStop()");
            break;
        }
        case APP_CMD_DESTROY:{
            Log::Write(Log::Level::Info,__FILE__,__LINE__,"onDestroy()");
            appState->NativeWindow=NULL;
            break;
        }
        case APP_CMD_INIT_WINDOW:{
            Log::Write(Log::Level::Info,__FILE__,__LINE__,"surfaceCreated()");
            appState->NativeWindow=app->window;
            break;
        }
        case APP_CMD_TERM_WINDOW:{
            Log::Write(Log::Level::Info,__FILE__,__LINE__,"surfaceDestroyed()");
            appState->NativeWindow=NULL;
            break;
        }
    }
}
static int32_t onInputEvent(struct android_app *app,AInputEvent *event){
    int type = AInputEvent_getType(event);
    if(type == AINPUT_EVENT_TYPE_KEY){
        int32_t action = AKeyEvent_getAction(event);
        int32_t code   = AKeyEvent_getKeyCode(event);
        Log::Write(Log::Level::Info, __FILE__, __LINE__, Fmt("onInputEvent:%d %d\n", code, action));
    }
    return 0;
}
static void killProcess(){
    pid_t pid=getpid();
    kill(pid,SIGKILL);
}



//==========================================================================================================
#include"opencv2/opencv.hpp"
#include"app/scene.h"

//std::shared_ptr<IOpenXrProgram> GlobalProgram=std::make_shared<IOpenXrProgram>();
cv::Mat ProcessCameraImage(const cv::Mat &input_image); //输入为BGRA,需要保证返回的cv::Mat是RGBA格式

std::shared_ptr<IScene> _scene=nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_rokid_openxr_android_MainActivity_onAppInit(JNIEnv *env, jobject) {
    _scene= createScene("3dtracking_test");
    _scene->initialize();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rokid_openxr_android_MainActivity_onCameraImageUpdated(JNIEnv *env, jobject,jobject buffer, jint width, jint height) {
    // 获取图像像素数据
    uchar* pixels = reinterpret_cast<uchar*>(env->GetDirectBufferAddress(buffer));
    if (!pixels) return;
    cv::Mat mat(height, width, CV_8UC4, pixels); //RGBA
    cv::Mat image;
    cv::cvtColor(mat,image,cv::COLOR_RGBA2BGR); //经测试，转换后的image才是颜色正常的

    if(_scene)
        _scene->processFrame(image);

    //image=ProcessCameraImage(image);
    //----------------------------------------------------------------------
    image.copyTo(mat); //写回数据
}

void InitApp(){
//    GlobalProgram->InitializeApplication();
}
cv::Mat ProcessCameraImage(const cv::Mat &input_image){
    static bool is_first=true;
    if(is_first) InitApp();
    is_first=false;


    cv::Mat image=input_image;
    cv::cvtColor(image,image,cv::COLOR_BGRA2RGB);
    cv::bitwise_not(image,image);
    cv::cvtColor(image,image,cv::COLOR_RGB2BGRA);


    cv::cvtColor(image,image,cv::COLOR_BGRA2RGBA); //返回值要转换回RGBA
    return image;
}