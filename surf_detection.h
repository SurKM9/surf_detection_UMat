#ifndef SURF_DETECTION_H
#define SURF_DETECTION_H

#include <QMainWindow>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/xfeatures2d.hpp>// these are the libraries in the new location     (they contain SURF implementation)
#include <iostream>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QEvent>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPalette>
#include <QColor>
#include <opencv2/core/ocl.hpp>
#include <boost/thread.hpp>



using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

namespace Ui {
class surf_detection;
}




class surf_detection : public QMainWindow
{
    Q_OBJECT

public:
    explicit surf_detection(QWidget *parent = 0);
    ~surf_detection();
    QImage cvToQImage(UMat img);
    UMat QImagetocv(QImage img);
    UMat crop_image_;
    void surf_detect();
//    void mousePressEvent(QMouseEvent *ev);
//    void mouseMoveEvent(QMouseEvent *ev);
//    void mouseReleaseEvent(QMouseEvent *ev);
    static void mouse_call(int event, int x, int y, int, void *param);
    void doMouseCallback(int event, int x, int y);

public slots:

    int Start();
    void _snapshot();
    void start_detection();

private:
    Ui::surf_detection *ui;
    vector<KeyPoint> keypoints_1;
    vector<KeyPoint> keypoints_2;
    //std::vector<vector<DMatch > > matches;
    //std::vector<DMatch > good_matches;
    //std::vector<Point2f> obj;
    //std::vector<Point2f> scene;
    UMat img_keypoint1,img_keypoint2,img_matches;
    UMat cameraFeed;
    bool takeSnapshot;
    QImage copyImage;
    bool detect;
    //SurfFeatureDetector detector;
    //SurfDescriptorExtractor extractor;
    FlannBasedMatcher matcher;
    QImage snapshot;
    Mat H;
    UMat gray,gray_image,src,dst;
    Mat frame;
    int minHessian;
    int drag;
    Rect rect;
    QPoint origin;
    QRubberBand *rubberBand;
    int x,y;
    UMat img,roiImg;
    int selectflag =0;
    bool leftDown=false,leftup=false;
    Point cor1,cor2;
    Rect box;
    UMat cv_snapshot;
    QImage q_img;
    std::vector<Point2f> obj,scene;


};

#endif // SURF_DETECTION_H
