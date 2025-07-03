#include"ObjectTracking2/include/ObjectTracking2.h"
#include"Basic/include/App.h"
#include<opencv2/highgui.hpp>
#include"BFC/portable.h"
#include"arengine.h"
#include"scene.h"
//#include"demos/utils.h"
//#include"demos/model.h"
#include"utilsmym.hpp"
using namespace cv;

#include"re3d/base/base.h"
#include"CVRender/cvrmodel.h"
using namespace cv;

//static glm::mat4 ViewMat; //保存相机位姿，眼镜的Marker检测用

namespace {
    std::shared_ptr<ARApp> construct_engine() {
        std::string appName = "TestApp"; //APP名称，必须和服务器注册的App名称对应（由服务器上appDir中文件夹的名称确定）

        std::vector<ARModulePtr> modules;
        modules.push_back(createModule<ARInputs>("ARInputs"));
        modules.push_back(createModule<ObjectTracking2>("ObjectTracking2", &ObjectTracking2::create));  //用createModule创建模块，必须指定一个模块名，并且和server上的模块名对应！！

        auto appData = std::make_shared<AppData>();
        auto sceneData = std::make_shared<SceneData>();

        appData->argc = 1;
        appData->argv = nullptr;
        appData->engineDir = "./AREngine/";  // for test
        appData->dataDir = "./data/";        // for test

        std::shared_ptr<ARApp> app = std::make_shared<ARApp>();
        app->init(appName, appData, sceneData, modules);

        std::string file=std::string(MakeSdcardPath("Download/3d/box1/box1.ply"));
        if(ff::pathExist(file))
        {
            FILE *fp=fopen(file.c_str(),"rb");
            if(fp){
                fclose(fp);
            }
            else
                file=strerror(errno);
        }
        //CVRModel model(file);
        //auto center=model.getCenter();
        //printf("%.2f",center[0]);

        std::any cmdData = std::string(MakeSdcardPath("Download/3d/box1/box1.ply"));
        app->call("ObjectTracking2", ObjectTracking2::CMD_SET_MODEL, cmdData);

        return app;
    }


    class Scene_3dtracking_test:public IScene {
        std::shared_ptr<ARApp> _eng;
    public:

        virtual bool initialize() {
            _eng = construct_engine();
            _eng->start();
            return true;
        }

        virtual void processFrame(cv::Mat &img) {
            if (_eng) {
                auto _res=_eng->sceneData->getData("ObjectTracking2Result");
                auto _inputs=_eng->sceneData->getData("ARInputs");

                if (_res.has_value())
                {
                    ObjectTracking2::Result res = std::any_cast<ObjectTracking2::Result>(_res);

                    Mat dimg = img.clone(); //frameDataPtr->image.front();
//                    res.showResult(dimg);
                    //res.showResult(dimg);
                    for (auto& obj : res.objPoses)
                    {
                        auto modelPtr = res.modelSet->getModel(obj.modelIndex);
                        if (modelPtr)
                        {
                            CVRModel& m3d = modelPtr->get3DModel();
                            auto center = m3d.getCenter();

                            auto t = (obj.R * center + obj.t);

                            std::string text = cv::format("[%.2f,%.2f,%.2f]", t[0], t[1], t[2]);
                            cv::putText(dimg, text, cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255, 128, 0), 2, cv::LINE_AA);
                        }

                        std::vector<std::vector<cv::Point>> vpts(1);
                        vpts[0] = obj.contourProjected;
                        cv::drawContours(dimg, vpts, -1, Scalar(255, 0, 0), 2);

//                        auto rect=cv::getBoundingBox2D(obj.contourProjected);
//                        rect= rectOverlapped(rect, Rect(0,0,dimg.cols,dimg.rows));
//                        if(!rect.empty())
//                            setMem(dimg(rect),0);
                    }
                    img=dimg;
                }
            }
        }

        virtual void close() {
            if (_eng) _eng->stop();
        }
    };
}

std::shared_ptr<IScene> _createScene_3dtracking_test() {
    return std::make_shared<Scene_3dtracking_test>();
}


