#pragma once

//#include"app/application.h"
#include<string>
#include"opencv2/core.hpp"

class IScene {
public:
public:
    virtual ~IScene() = default;
    virtual bool initialize() =0;
    //virtual void inputEvent(int leftright, const ApplicationEvent& event) {}
    //virtual void renderFrame(const XrPosef& pose, const glm::mat4& project, const glm::mat4& view, int32_t eye) {}
    virtual void processFrame(cv::Mat &img) {}
    virtual void close() {}
};


std::shared_ptr<IScene> createScene(const std::string &name);

