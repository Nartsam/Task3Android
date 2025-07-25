#include<string>
#include<sstream>
#include"glm/glm.hpp"
#include<opencv2/opencv.hpp>
#include"demos/utils.h"


inline std::string GlmMat4_to_String(const glm::mat4 &matrix){
    std::ostringstream oss;
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            oss<<matrix[i][j]; if(j<3){oss<<" ";}
        }
        if(i<3){oss<<"\n";}
    }
    return oss.str();  // 返回构建的字符串
}
inline glm::mat4 CV_Matx44f_to_GLM_Mat4(const cv::Matx44f &mat) {  // 将 cv::Matx44f 转换为 glm::mat4
    return glm::mat4(
            mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3),  // 第一行
            mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3),  // 第二行
            mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3),  // 第三行
            mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3)   // 第四行
    );
}
inline bool StringStartsWith(const std::string &str,const std::string &starts_with){
    if((str.rfind(starts_with,0)==0)) return true; //start with
    return false;
}
inline std::string MakeSdcardPath(const std::string &path) {
    static const std::string SdcardPrefix("/storage/emulated/0/");
    std::string res=path;
    if(!path.empty() && !StringStartsWith(path,SdcardPrefix)) res=SdcardPrefix+res;
    return res;
}
inline cv::Mat GetDistCoeffs(double k1, double k2, double k3, double p1, double p2) { //构造cv::Mat格式的相机外参矩阵
    cv::Mat distCoeffs=(cv::Mat_<double>(1,5) <<k1, k2, p1, p2, k3);
    return distCoeffs;
}
inline cv::Mat GetCameraMatrix(double fx, double fy, double cx, double cy) { //构造cv::Mat格式的相机内参矩阵
    cv::Mat cameraMatrix=(cv::Mat_<double>(3,3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
    return cameraMatrix;
}

//inline void GetGLModelView(const cv::Matx33f &R, const cv::Vec3f &t, float* mModelView, bool cv2gl){
//    //绕X轴旋转180度，从OpenCV坐标系变换为OpenGL坐标系
//    //同时转置，opengl默认的矩阵为列主序
//    if (cv2gl){
//        mModelView[0] = R(0, 0);
//        mModelView[1] = -R(1, 0);
//        mModelView[2] = -R(2, 0);
//        mModelView[3] = 0.0f;
//
//        mModelView[4] = R(0, 1);
//        mModelView[5] = -R(1, 1);
//        mModelView[6] = -R(2, 1);
//        mModelView[7] = 0.0f;
//
//        mModelView[8] = R(0, 2);
//        mModelView[9] = -R(1, 2);
//        mModelView[10] = -R(2, 2);
//        mModelView[11] = 0.0f;
//
//        mModelView[12] = t(0);
//        mModelView[13] = -t(1);
//        mModelView[14] = -t(2);
//        mModelView[15] = 1.0f;
//    }
//    else{
//        mModelView[0] = R(0, 0);
//        mModelView[1] = R(1, 0);
//        mModelView[2] = R(2, 0);
//        mModelView[3] = 0.0f;
//
//        mModelView[4] = R(0, 1);
//        mModelView[5] = R(1, 1);
//        mModelView[6] = R(2, 1);
//        mModelView[7] = 0.0f;
//
//        mModelView[8] = R(0, 2);
//        mModelView[9] = R(1, 2);
//        mModelView[10] = R(2, 2);
//        mModelView[11] = 0.0f;
//
//        mModelView[12] = t(0);
//        mModelView[13] = t(1);
//        mModelView[14] = t(2);
//        mModelView[15] = 1.0f;
//    }
//}
inline void GetGLModelView(const cv::Matx33f &R, const cv::Vec3f &t, float* mModelView, bool cv2gl){ //2025-06-11修改版
    //绕X轴旋转180度，从OpenCV坐标系变换为OpenGL坐标系
    //同时转置，opengl默认的矩阵为列主序
    if (cv2gl){
        mModelView[0] = R(0, 0);
        mModelView[1] = -R(1, 0);
        mModelView[2] = -R(2, 0);
        mModelView[3] = 0.0f;

        mModelView[4] = R(0, 1);
        mModelView[5] = -R(1, 1);
        mModelView[6] = -R(2, 1);
        mModelView[7] = 0.0f;

        mModelView[8] = R(0, 2);
        mModelView[9] = -R(1, 2);
        mModelView[10] = -R(2, 2);
        mModelView[11] = 0.0f;

        mModelView[12] = t(0);
        mModelView[13] = -t(1);
        mModelView[14] = -t(2);
        mModelView[15] = 1.0f;
    }
    else{
//        mModelView[0] = R(0, 0);
//        mModelView[1] = R(1, 0);
//        mModelView[2] = R(2, 0);
//        mModelView[3] = 0.0f;
//
//        mModelView[4] = R(0, 1);
//        mModelView[5] = R(1, 1);
//        mModelView[6] = R(2, 1);
//        mModelView[7] = 0.0f;
//
//        mModelView[8] = R(0, 2);
//        mModelView[9] = R(1, 2);
//        mModelView[10] = R(2, 2);
//        mModelView[11] = 0.0f;
//
//        mModelView[12] = t(0);
//        mModelView[13] = t(1);
//        mModelView[14] = t(2);
//        mModelView[15] = 1.0f;

        mModelView[0] = R(0, 0);
        mModelView[4] = R(1, 0);
        mModelView[8] = R(2, 0);
        mModelView[12] = 0.0f;

        mModelView[1] = R(0, 1);
        mModelView[5] = R(1, 1);
        mModelView[9] = R(2, 1);
        mModelView[13] = 0.0f;

        mModelView[2] = R(0, 2);
        mModelView[6] = R(1, 2);
        mModelView[10] = R(2, 2);
        mModelView[14] = 0.0f;

        mModelView[3] = t(0);
        mModelView[7] = t(1);
        mModelView[11] = t(2);
        mModelView[15] = 1.0f;
    }
}
inline cv::Matx44f FromRT(const cv::Vec3f &rvec, const cv::Vec3f &tvec, bool cv2gl){
    if (isinf(tvec[0]) || isnan(tvec[0]) || isinf(rvec[0]) || isnan(rvec[0]))
        return cv::Matx44f(1.0f, 0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f);

    cv::Matx33f R;
    cv::Rodrigues(rvec, R);
    cv::Matx44f m;
    GetGLModelView(R, tvec, m.val, cv2gl);
    return m;
}

inline cv::Matx44f FromR33T(const cv::Matx33f &R, const cv::Vec3f &tvec, bool cv2gl){
    if (isinf(tvec[0]) || isnan(tvec[0]) )
        return cv::Matx44f(1.0f, 0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f);

    cv::Matx44f m;
    GetGLModelView(R, tvec, m.val, cv2gl);
    return m;
}

// glm::mat4 转换为 cv::Mat（4x4，32位浮点数）// 注意 glm 是列主序,这里仅复制未做转置
inline cv::Mat GLM_Mat4_to_CV_Mat(const glm::mat4& glmMat) {
    cv::Mat cvMat(4, 4, CV_32F); // 创建 4x4 浮点矩阵
    for (int col = 0; col < 4; ++col) {  // 将 glm::mat4 的值按列主序复制到 cv::Mat
        for (int row = 0; row < 4; ++row) {
            cvMat.at<float>(row, col) = glmMat[row][col];
        }
    }
    return cvMat;
}
// 将 cv::Mat 转换为 glm::mat4,假设 cv::Mat 是一个 4x4 的浮点矩阵（CV_32F 类型）：// 注意 glm 是列主序,这里仅复制未做转置
inline glm::mat4 CV_Mat_to_GLM_Mat4(const cv::Mat& cvMat) {
    //CV_Assert(cvMat.rows == 4 && cvMat.cols == 4 && cvMat.type() == CV_32F); // 检查尺寸和类型
    glm::mat4 glmMat(1.0f); // 初始化为单位矩阵
    if(cvMat.rows==4||cvMat.cols==4){
        if(cvMat.type() != CV_32F) warnf(std::string("传入的cv::Mat格式为: "+std::to_string(cvMat.type())+", 会尝试按照CV_32F进行转换,也许会导致错误").c_str());
        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) { // 注意：cv 是 row-major，glm 是 column-major
                glmMat[row][col] = cvMat.at<float>(row, col); //这里需要原样复制回来，否则结果不正确. 经测试，这种写法才是正确的(2025-06-10)
                //glmMat[col][row] = cvMat.at<float>(row, col); //还是应该做一个转置(2025-06-11)
            }
        }
    }
    else warnf(std::string("传入的cv::Mat尺寸为("+std::to_string(cvMat.rows)+", "+std::to_string(cvMat.cols)+"), 无法转换glm::mat4, 将返回单位矩阵").c_str());
    return glmMat;
}
inline cv::Mat Matx44f_to_Mat(const cv::Matx44f& matx) {
    cv::Mat mat(4, 4, CV_32F);
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col) mat.at<float>(row, col) = matx(row, col);
    return mat;
}
inline cv::Matx44f Mat_to_Matx44f(const cv::Mat& mat) {
    //CV_Assert(mat.rows == 4 && mat.cols == 4 && mat.type() == CV_32F);
    cv::Matx44f matx(1.0f, 0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f);//初始化为单位矩阵
    if(mat.rows==4||mat.cols==4){
        if(mat.type() != CV_32F) warnf(std::string("传入的cv::Mat格式为: "+std::to_string(mat.type())+", 会尝试按照CV_32F进行转换,也许会导致错误").c_str());
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col) matx(row, col) = mat.at<float>(row, col);
    }
    else warnf(std::string("传入的cv::Mat尺寸为("+std::to_string(mat.rows)+", "+std::to_string(mat.cols)+"), 无法转换Matx44f, 将返回单位矩阵").c_str());
    return matx;
}
inline cv::Mat RokidCameraMatrix; //眼镜相机的内参
inline cv::Mat RokidDistCoeffs; //眼镜相机的外参