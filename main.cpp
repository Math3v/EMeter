#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define WIN_TITLE "ElectroMeter"
#define VERSION "0.0.1"

void showIndicator(int x, int y, Mat frame) {
    circle(
    /* Frame::Mat */        frame, 
    /* Circle center */     Point(x,y),
    /* Radius */            20, 
    /* Scalar(R, G, B) */   Scalar(0, 0, 255), 
    /* Thickness */         -1, 
    /* LineType */          8, 
    /* Shift */             0);
}

int main(int argc, char** argv ) {

    ofstream outfile;

    if( argc != 2 ) {
        cerr << "No input file. Usage: " << argv[0] << " VIDEO_CAPTURE_FILE" << endl;
        cerr << "Matej Minarik (C) ElectroMeter " << VERSION << endl;
        return 1;
    }

    outfile.open( "data.txt" );
    VideoCapture cap( argv[1] );
    if( cap.isOpened() == false && outfile.is_open() ) {
        cerr << "Cannot open file" << endl;
        return -1;
    }

    double frameWidth =  cap.get( CV_CAP_PROP_FRAME_WIDTH );
    double frameHeight = cap.get( CV_CAP_PROP_FRAME_HEIGHT );
    double videoFPS = cap.get( CV_CAP_PROP_FPS );

    cout << " Width: " << frameWidth << endl;
    cout << " Height: " << frameHeight << endl;
    cout << " FPS: " << videoFPS << endl;

    int indicatorX = (int) ((float) frameHeight * 0.7);
    int indicatorY = (int) ((float) frameWidth * 0.1);

    namedWindow(WIN_TITLE);
    resizeWindow(WIN_TITLE, frameHeight, frameWidth);

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
                sumR += currPixel[2];
            }
        }

        outfile << frameNo;
        outfile << " " << sumR;
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
    cap.release();

    return 0;
}