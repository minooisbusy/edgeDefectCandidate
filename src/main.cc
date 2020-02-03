#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/core/hal/interface.h>
#include"lsd.h"
#include<iostream>
#define CV_MAKE_TYPE CV_MAKETYPE
#define uint unsigned int
#define CV_PI 3.1415926535897932384626433832795
using namespace std;
inline cv::Point3d cross(const cv::Point& a, const cv::Point& b)
{
    cv::Point3d tmp =cv::Point3d(a.y - b.y, -a.x + b.x, a.x*b.y-a.y*b.x);
    if(tmp.x < 0)
        tmp = tmp*-1;
    return tmp;
}
int main(int argc, char **argv)
{
    if(argc<2)
    {
        cout<<"Usage: ./edgeDefect <image>"<<endl;
        return 0;
    }
    cv::Mat src = cv::imread(argv[1] ,cv::IMREAD_COLOR);
    if( src.empty())
        cout<<"Image file is not exist"<<endl;
    cv::Mat tmp, src_gray;
    cv::cvtColor(src, tmp, cv::COLOR_RGB2GRAY);
    tmp.convertTo(src_gray,CV_64FC1);

    int cols = src_gray.cols;
    int rows = src_gray.rows;

    image_double image = new_image_double(cols, rows);
    image->data = src_gray.ptr<double>(0);
    ntuple_list ntl =lsd(image);

    cv::Mat lsd = cv::Mat::zeros(rows, cols, CV_8UC1);
    cv::Point pt1, pt2;
    
    for (uint j=0; j != ntl->size; ++j)
    {
        pt1.x = ntl->values[0 + j * ntl->dim];
        pt1.y = ntl->values[1 + j * ntl->dim];
        pt2.x = ntl->values[2 + j * ntl->dim];
        pt2.y = ntl->values[3 + j * ntl->dim];
        //double width = ntl->values[4 + j * ntl->dim];
        //cout<<width<<endl;
        
        cv::Point3d l = cross(pt1,pt2);
        double x = cv::fastAtan2(-l.x,l.y)/360*255*3;
        x=cv::abs(x);
        int r(0),g(0),b(0);
        if ( x>255*2)
        {
            r=x-255*2;
            x-=255;
        }
        if( x>255)
        {
            g=x-255;
            x-=255;
        }
        b=x;
        //TODO: Normalize 128~255 blue? color and apply line parameter:
        cv::line(src, pt1, pt2, cv::Scalar(b,g,r),4);
        cv::circle(src, pt1, 3, cv::Scalar(0,255,0),-1);
        cv::circle(src, pt2, 3, cv::Scalar(0,0,255),-1);
    }
    free_ntuple_list(ntl);
    //cv::resize(src, src,cv::Size(cols/4,rows/4));
    //cv::imshow("src", src);
    //cv::imshow("lsd", lsd);
    //cv::waitKey(0);
    //cv::destroyAllWindows();
    cv::imwrite("demo.jpg", src);
    return 0;
}
