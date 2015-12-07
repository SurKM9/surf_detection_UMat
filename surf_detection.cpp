#include "surf_detection.h"
#include "ui_surf_detection.h"




surf_detection::surf_detection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::surf_detection)
{
    ui->setupUi(this);
    takeSnapshot = false;
    detect =false;
    minHessian = 400;

}

surf_detection::~surf_detection()
{
    delete ui;
}

void surf_detection::surf_detect(){

    UMat img_extractor,snap_extractor;

    if(crop_image_.empty())
         cv_snapshot.copyTo(dst);
    else
        crop_image_.copyTo(dst);
        //dst = QImagetocv(crop_image_);

    imshow("dst",dst);

    Ptr<SURF> detector = SURF::create(minHessian);
    Ptr<DescriptorExtractor> extractor = SURF::create(minHessian);

    cvtColor(dst,src,CV_BGR2GRAY);
    cvtColor(frame,gray_image,CV_BGR2GRAY);


    detector->detect(src,keypoints_1);
    //printf("Object: %d keypoints detected\n", (int)keypoints_1.size());
    detector->detect(gray_image,keypoints_2);
    printf("Object: %d keypoints detected\n", (int)keypoints_1.size());

    extractor->compute(src,keypoints_1,img_extractor);
   // printf("Object: %d descriptors extracted\n", img_extractor.rows);
    extractor->compute(gray_image,keypoints_2,snap_extractor);

    std::vector<Point2f> scene_corners(4);
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = (cvPoint(0,0));
    obj_corners[1] = (cvPoint(src.cols,0));
    obj_corners[2] = (cvPoint(src.cols,src.rows));
    obj_corners[3] = (cvPoint(0, src.rows));

    vector<DMatch> matches;
    matcher.match(img_extractor,snap_extractor,matches);

    double max_dist = 0; double min_dist = 100;

      //-- Quick calculation of max and min distances between keypoints
      for( int i = 0; i < img_extractor.rows; i++ )
      { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
      }
      printf("-- Max dist : %f \n", max_dist );
      printf("-- Min dist : %f \n", min_dist );

      vector< DMatch > good_matches;

        for( int i = 0; i < img_extractor.rows; i++ )
        { if( matches[i].distance <= max(2*min_dist, 0.02) )
          { good_matches.push_back( matches[i]); }
        }

        UMat img_matches;
        drawMatches( src, keypoints_1, gray_image, keypoints_2,
                       good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                       vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        if(good_matches.size()>=4){

            for(int i =0;i<good_matches.size();i++){

                //get the keypoints from good matches
                obj.push_back(keypoints_1[good_matches[i].queryIdx].pt);
                scene.push_back(keypoints_2[good_matches[i].trainIdx].pt);

            }
        }

        H = findHomography(obj,scene,CV_RANSAC);

        perspectiveTransform(obj_corners,scene_corners,H);

        line( img_matches, scene_corners[0], scene_corners[1], Scalar(0, 255, 0), 4 );

        line( img_matches, scene_corners[1], scene_corners[2], Scalar( 0, 255, 0), 4 );

        line( img_matches, scene_corners[2], scene_corners[3], Scalar( 0, 255, 0), 4 );

        line( img_matches, scene_corners[3], scene_corners[0], Scalar( 0, 255, 0), 4 );

//    drawKeypoints(src,keypoints_1,img_keypoint1,Scalar::all(-1),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//    drawKeypoints(gray_image,keypoints_2,img_keypoint2,Scalar::all(-1),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

//    QImage img_1 = cvToQImage(img_keypoint1);
//    QImage img_2 = cvToQImage(img_keypoint2);

        imshow("Good matches",img_matches);
//    ui->label_2->setScaledContents(true);
//    ui->label->setPixmap(QPixmap::fromImage(img_2));
//    ui->label_2->setPixmap(QPixmap::fromImage(img_1));


}

//void surf_detection::mousePressEvent(QMouseEvent *ev)
//{
//    if(ui->label_2->underMouse()){
//       // cout <<"Entered Press"<<endl;
//        origin = ev->pos();
//        //if (!rubberBand)
//            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

//            rubberBand->show();
//    }
//}

//void surf_detection::mouseMoveEvent(QMouseEvent *ev)
//{
//    if(ui->label_2->underMouse()){
//    rubberBand->setGeometry(QRect(origin, ev->pos()).normalized());
//    // cout <<"Entered Move"<<endl;
//    }
//}

//void surf_detection::mouseReleaseEvent(QMouseEvent *ev)
//{
//    if(ui->label_2->underMouse()){

//    QPoint a = mapToGlobal(origin);
//    QPoint b = ev->globalPos();

//    a = ui->label_2->mapFromGlobal(a);
//    b = ui->label_2->mapFromGlobal(b);

//    rubberBand->hide();

//    QPixmap OriginalPix(*ui->label_2->pixmap());

//    double sx = ui->label_2->rect().width();
//    double sy = ui->label_2->rect().height();
//    sx = OriginalPix.width() / sx;
//    sy = OriginalPix.height() / sy;

//    a.setX(int(a.x() * sx));
//    b.setX(int(b.x() * sx));
//    a.setY(int(a.y() * sy));
//    b.setY(int(b.y() * sy));


//    QRect myRect(a,b);

//    QImage newImage;
//    newImage = OriginalPix.toImage();

//    copyImage = newImage.copy(myRect);

//    ui->label_2->setScaledContents(true);
//    ui->label_2->setPixmap(QPixmap::fromImage(copyImage));
//    ui->label_2->repaint();

//    }
//}

QImage surf_detection::cvToQImage(UMat _img){

    Mat RGB;
    Mat rgb_img,n_img;
    _img.copyTo(rgb_img);
    _img.copyTo(n_img);
    QImage m_img;
    if(rgb_img.channels()==3){


        cvtColor(rgb_img,RGB,CV_BGR2RGB);
        m_img = QImage((const unsigned char*)(RGB.data),RGB.cols,RGB.rows,RGB.step,QImage::Format_RGB888).copy();
        return m_img;
    }
    else{

        m_img = QImage((const unsigned char*)(n_img.data),n_img.cols,n_img.rows,n_img.step,QImage::Format_RGB888).copy();
        return m_img;
    }
}

UMat surf_detection::QImagetocv(QImage img){

    Mat tmp(img.height(),img.width(),CV_8UC3,
            (uchar*)img.bits(),img.bytesPerLine());
    UMat con;
    tmp.copyTo(con);
    UMat result;
    cvtColor(con,result,CV_BGR2RGB);
    return result;

}

void surf_detection::_snapshot(){

    takeSnapshot = true;
}


void surf_detection::start_detection(){

    detect = true;
}

void surf_detection::mouse_call(int event, int x, int y, int, void *param){

    surf_detection *self = static_cast<surf_detection*>(param);
    //surf_detection *s;
    self->doMouseCallback(event,x,y);


}
void surf_detection::doMouseCallback(int event, int x, int y){

    if(event==EVENT_LBUTTONDOWN)
        {
            leftDown=true;
            cor1.x=x;
            cor1.y=y;
           cout <<"Corner 1: "<<cor1<<endl;

        }
    if(event==EVENT_LBUTTONUP)
        {
            if(abs(x-cor1.x)>20&&abs(y-cor1.y)>20) //checking whether the region is too small
            {
                leftup=true;
                cor2.x=x;
                cor2.y=y;
                cout<<"Corner 2: "<<cor2<<endl;
            }
            else
            {
                cout<<"Select a region more than 20 pixels"<<endl;
            }
        }

        if(leftDown==true&&leftup==false) //when the left button is down
        {
            Point pt;

            pt.x=x;
            pt.y=y;
            UMat temp_img=cv_snapshot.clone();
            rectangle(temp_img,cor1,pt,Scalar(0,0,255)); //drawing a rectangle continuously
            //imshow("Original",temp_img);

        }
        if(leftDown==true&&leftup==true) //when the selection is done
        {

            box.width=abs(cor1.x-cor2.x);
            box.height=abs(cor1.y-cor2.y);
            box.x=min(cor1.x,cor2.x);
            box.y=min(cor1.y,cor2.y);
            UMat crop(cv_snapshot,box);   //Selecting a ROI(region of interest) from the original pic
            crop.copyTo(crop_image_);
            namedWindow("Cropped Image",WINDOW_NORMAL);
            imshow("Cropped Image",crop); //showing the cropped image
            leftDown=false;
            leftup=false;

        }

}


int surf_detection::Start(){


    ocl::setUseOpenCL(true);
    if (!ocl::haveOpenCL())
    {
        cout << "OpenCL is not available..." << endl;
        //return;
    }
    VideoCapture cap(1);
    //cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    while(1){


        cap >> frame;
        if(frame.empty())
            return -1;

        if(ui->radioButton_2->isChecked()){

            Size down_scale(frame.cols/2,frame.rows/2);
            cv::resize(frame,frame,down_scale,0,0,cv::INTER_AREA);
            frame.copyTo(cameraFeed);
        }
        else{

            frame.copyTo(cameraFeed);
        }

        ui->label->setScaledContents(true);
        q_img = cvToQImage(cameraFeed);
        ui->label->setPixmap(QPixmap::fromImage(q_img));

        if (takeSnapshot)
        {
            snapshot = q_img.copy();
            takeSnapshot = false;
            ui->label_2->setPixmap(QPixmap::fromImage(snapshot));
            cv_snapshot = cameraFeed.clone();
            imshow("Mat Image",cv_snapshot);
            cvSetMouseCallback("Mat Image",surf_detection::mouse_call,this);
        }

        if(detect){

            QTime timer;
            //boost::thread thr;
            timer.start();
            //thr = boost::thread(&surf_detection::surf_detect,this);
            surf_detect();
            //QtConcurrent::run((this),&surf_detection::surf_detect);
            //QFuture<void> future = QtConcurrent::run((*this),&surf_detection::surf_detect);
            //future.waitForFinished();
            int runtime = timer.elapsed();
            cout << runtime << endl;
            //imshow("Camfeed",img_keypoint2);
            //imshow("Snapshot",img_keypoint1);
            //detect = false;

        }

        cvWaitKey(30);
    }

    return 0;
}

