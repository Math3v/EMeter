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
    Mat prevFrame;
    int frame_no = 0;
    float sum;
    Vec3b currPixel, prevPixel;
    while( cap.read(currFrame) ) {
        int rows = currFrame.rows;
        int cols = currFrame.cols;
        float r, g, b;

        if( prevFrame.empty() == true ) {
            prevFrame = currFrame.clone();
            continue;
        }

        for( int i = 0; i < rows; i++ ) {
            for( int j = 0; j < cols; j++ ) {
                currPixel = currFrame.at<Vec3b>(Point(i, j));
                prevPixel = prevFrame.at<Vec3b>(Point(i, j));
                sum += (currPixel[0] - prevPixel[0]);
                sum += (currPixel[1] - prevPixel[1]);
                sum += (currPixel[2] - prevPixel[2]);
            }
        }

        cout << "Frame " << frame_no << " sum " << sum << endl;
        outfile << frame_no << " " << sum << endl;
        ++frame_no;
        prevFrame = currFrame.clone();
        sum = (float) 0;
    }

    outfile.close();

    return 0;
}