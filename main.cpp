#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include <stdlib.h>
#include <time.h>

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

int getRandomByte() {
    return (rand() % 50) + 50;
}

Vec3b getRandomFilter() {
    int type = rand() % 6;
    switch(type) {
        case 0:
            return Vec3b(getRandomByte(), 0, 0);
        case 1:
            return Vec3b(0, getRandomByte(), 0);
        case 2:
            return Vec3b(0, 0, getRandomByte());
        case 3:
            return Vec3b(getRandomByte(), getRandomByte(), 0);
        case 4:
            return Vec3b(getRandomByte(), 0, getRandomByte());
        case 5:
            return Vec3b(0, getRandomByte(), getRandomByte());
    }
    return Vec3b(0, 0, 0);
}

bool filterShouldChange(int frameNo) {
    static int lastChangeFrameNo = 0;
    if( 0 == lastChangeFrameNo ){
        lastChangeFrameNo = frameNo;
        return true;
    }
    else if( frameNo - lastChangeFrameNo > 7 ) {
        lastChangeFrameNo = frameNo;
        return true;
    }
    return false;
}

int main(int argc, char** argv ) {

    ofstream outfile;

    srand( time(NULL) );

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

    int indicatorY = (int) ((float) frameHeight * 0.1);
    int indicatorX = (int) ((float) frameWidth * 0.8);

    namedWindow(WIN_TITLE);
    resizeWindow(WIN_TITLE, frameHeight, frameWidth);

    Mat currFrame, cloneFrame;
    Vec3b currPixel;
    Vec3b filterPixel;
    unsigned long sumR, sumG, sumB;
    unsigned long frameNo = 0;
    unsigned long lastR = 0;
    while( cap.read(currFrame) ) {
        sumR = sumG = sumB = 0;

        cloneFrame = currFrame.clone();

        for( int i = 0; i < frameHeight; i++ ) {
            for( int j = 0; j < frameWidth; j++ ) {
                currPixel = currFrame.at<Vec3b>(Point(j, i));
                sumR += currPixel[2];
                if( cloneFrame.at<Vec3b>(Point(j, i))[0] + filterPixel[0] > 255 ) {
                    cloneFrame.at<Vec3b>(Point(j, i))[0] = 255;
                } else {
                    cloneFrame.at<Vec3b>(Point(j, i))[0] += filterPixel[0];
                }
                if( cloneFrame.at<Vec3b>(Point(j, i))[1] + filterPixel[1] > 255 ) {
                    cloneFrame.at<Vec3b>(Point(j, i))[1] = 255;
                } else {
                    cloneFrame.at<Vec3b>(Point(j, i))[1] += filterPixel[1];
                }
                if( cloneFrame.at<Vec3b>(Point(j, i))[2] + filterPixel[2] > 255 ) {
                    cloneFrame.at<Vec3b>(Point(j, i))[2] = 255;
                } else {
                    cloneFrame.at<Vec3b>(Point(j, i))[2] += filterPixel[2];
                }
            }
        }

        outfile << frameNo;
        outfile << " " << sumR;
        outfile << endl;

        ++frameNo;
        if( lastR != 0 ) {
            if( (float) sumR / (float) lastR > 1.10 ) {
                showIndicator(indicatorX, indicatorY, cloneFrame);
                cout << "Dot " << frameNo << " " << indicatorX << ":" << indicatorY << endl;
                if( true == filterShouldChange(frameNo) ) {
                    filterPixel = getRandomFilter();
                }
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