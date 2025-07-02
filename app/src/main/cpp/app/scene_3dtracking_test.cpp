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

        std::string file=std::string(MakeSdcardPath("Download/3d/box1/box1.3ds"));
        if(ff::pathExist(file))
        {
            FILE *fp=fopen(file.c_str(),"rb");
            file=strerror(errno);
            if(fp){
                fclose(fp);
            }
        }

        std::any cmdData = std::string(MakeSdcardPath("Download/3d/box1/box1.3ds"));
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


