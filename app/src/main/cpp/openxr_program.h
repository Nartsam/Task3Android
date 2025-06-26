#pragma once

#include"options.h"
#include<string>
#include<vector>
#include"demos/utils.h"
#include"imgui/imgui.h"
#include"demos/gui.h"
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"


//inline EGLDisplay display;
//inline EGLSurface surface;
//inline EGLContext context;
//inline bool InitEGL(ANativeWindow* window) {
//    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
//    eglInitialize(display, nullptr, nullptr);
//    const EGLint configAttribs[] = {
//            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
//            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
//            EGL_BLUE_SIZE, 8,
//            EGL_GREEN_SIZE, 8,
//            EGL_RED_SIZE, 8,
//            EGL_DEPTH_SIZE, 16,
//            EGL_NONE
//    };
//    EGLConfig config;
//    EGLint numConfigs;
//    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
//    surface = eglCreateWindowSurface(display, config, window, nullptr);
//    const EGLint contextAttribs[] = {
//            EGL_CONTEXT_CLIENT_VERSION, 3,
//            EGL_NONE
//    };
//    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
//    eglMakeCurrent(display, surface, surface, context);
//    return true;
//}

struct IOpenXrProgram{
    virtual ~IOpenXrProgram() = default;
    void InitializeApplication(){
        mPanel = std::make_shared<Gui>("dashboard");
        mPanel->initialize(width, height);  //set resolution
        float width, height;
        mPanel->getWidthHeight(width, height);

        glm::mat4 model = glm::mat4(1.0f);
        float scale = 1.0f;
        scale = 0.7;
        model = glm::translate(model, glm::vec3(-0.0f, -0.3f, -1.0f));
        model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale * (width / height), scale, 1.0f));
        mPanel->setModel(model);


        is_running=true;
    }
    // Process any events in the event queue.
    virtual void PollEvents(bool *exitRenderLoop,bool *requestRestart){

    }
    // Manage session lifecycle to track if RenderFrame should be called.
    bool IsSessionRunning() const{
        return is_running;
    }
    // Manage session state to track if input should be processed.
    bool IsSessionFocused() const{
        return is_focused;
    }
    // Sample input actions and generate haptic feedback.
    void PollActions(){

    }
    void RenderFrame(){
        mPanel->begin();
        if (ImGui::CollapsingHeader("information")) {
            ImGui::BulletText("device model: %s", "Test");
            ImGui::BulletText("device OS: %s", "Android");
        }
        ImGui::Text("Please use the hand ray to pinch click.");
        mPanel->end();
        glm::mat4 p=glm::ortho(
                0.0f, (float)width,      // left, right
                0.0f, (float)height,     // bottom, top
                -1.0f, 1.0f             // near, far
        );
        glm::mat4 v=glm::mat4(1.0f);
        mPanel->render(p,v);
//        infof("in render frame");
    }
    void ProcessFrame(){

    }
public:
//    struct MarkerInfo{
//        std::string  markerImageFile;
//        float   phyWidth;  // 图像物理宽度（米）
//        float   phyHeight; // 图像物理高度（米）
//    };
//
//    struct MarkerData{
//        struct DMarker {
//            uint32_t id;
//            float pose[7];
//        };
//        std::vector<DMarker>  added;
//        std::vector<DMarker>  updated;
//        std::vector<DMarker>  removed;
//    };
//    int AddMarkerImages(const std::vector<MarkerInfo> &markers){return 0;}
//    void ProcessMarkerData(MarkerData &markerData){warnf("No Implement of ProcessMarkerData");}

    bool is_running{false};
    bool is_focused{false};
    int width{480};
    int height{640};
    std::shared_ptr<Gui> mPanel;
};