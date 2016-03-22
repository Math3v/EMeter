#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv ) {

    ofstream outfile;
    outfile.open( "data.txt" );

    VideoCapture cap( "./dat/ham_test_2.mp4" );
    if( cap.isOpened() == false ) {
        cerr << "Cannot open file" << endl;
        return -1;
    }

    Mat currFrame;
    Vec3b currPixel;
    unsigned long sumR, sumG, sumB;
    while( cap.read(currFrame) ) {
        int rows = currFrame.rows;
        int cols = currFrame.cols;
        sumR = sumG = sumB = 0;

        for( int i = 0; i < rows; i++ ) {
            for( int j = 0; j < cols; j++ ) {
                currPixel = currFrame.at<Vec3b>(Point(i, j));
                sumB += currPixel[0];
                sumG += currPixel[1];
                sumR += currPixel[2];
            }
        }
        
        double cmsec = cap.get( CV_CAP_PROP_POS_MSEC );
        //int minutes = (int) ((int) (cmsec / (1000.0*60.0)) % (int) 60);
        //int seconds = (int) (cmsec / 1000) % 60;

        outfile << cmsec;
        outfile << " " << sumR;
        outfile << " " << sumG;
        outfile << " " << sumB;
        outfile << endl;
    }

    outfile.close();

    return 0;
}