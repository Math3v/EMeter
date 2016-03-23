#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define WIN_TITLE "Electro Meter v0.1"
#define WIN_W 480
#define WIN_H 640

void showIndicator(int x, int y, Mat frame) {
    circle(
    /* Frame::Mat */        frame, 
    /* Circle center */     Point(x,y),
    /* Radius */            40, 
    /* Scalar(R, G, B) */   Scalar(0, 0, 255), 
    /* Thickness */         -1, 
    /* LineType */          8, 
    /* Shift */             0);
}

int main(int argc, char** argv ) {

    ofstream outfile;
    outfile.open( "data.txt" );

    VideoCapture cap( "./dat/br_low_res_1.mp4" );
    if( cap.isOpened() == false ) {
        cerr << "Cannot open file" << endl;
        return -1;
    }

    double frameWidth =  cap.get( CV_CAP_PROP_FRAME_WIDTH );
    double frameHeight = cap.get( CV_CAP_PROP_FRAME_HEIGHT );
    double videoFPS = cap.get( CV_CAP_PROP_FPS );

    cout << " Width: " << frameWidth << endl;
    cout << " Height: " << frameHeight << endl;
    cout << " FPS: " << videoFPS << endl;

    int indicatorX = (int) (WIN_H * 0.15);
    int indicatorY = (int) (WIN_W * 0.85);

    namedWindow(WIN_TITLE);
    resizeWindow(WIN_TITLE, WIN_H, WIN_W);

    Mat currFrame, cloneFrame;
    Vec3b currPixel;
    unsigned long sumR, sumG, sumB;
    unsigned long frameNo = 0;
    unsigned long lastR = 0;
    while( cap.read(currFrame) ) {
        sumR = sumG = sumB = 0;

        cloneFrame = currFrame.clone();

        for( int i = 0; i < frameHeight; i++ ) {
            for( int j = 0; j < frameWidth; j++ ) {
                currPixel = currFrame.at<Vec3b>(Point(i, j));
                //sumB += currPixel[0];
                //sumG += currPixel[1];
                sumR += currPixel[2];
            }
        }

        outfile << frameNo;
        outfile << " " << sumR;
        //outfile << " " << sumG;
        //outfile << " " << sumB;
        outfile << endl;

        ++frameNo;
        if( lastR != 0 ) {
            if( (float) sumR / (float) lastR > 1.10 ) {
                showIndicator(indicatorX, indicatorY, cloneFrame);
                cout << "Dot " << frameNo << endl;
            } else {
                lastR = sumR;
            }
        } else {
            lastR = sumR;
        }

        imshow(WIN_TITLE, cloneFrame);
        if( waitKey(29) >= 0 ) break;
    }

    outfile.close();

    return 0;
}