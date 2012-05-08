/*****************************************
 CMPT 365: Summer 2011
 Final Project: H261/H264 Encoder and Decoder
 Created By: Dan Ho and Kenneth Wong
 Professor: Dr. Ze-Nian Li
 Date: July 30th 2011
 ******************************************/
#include <highgui.h>
#include "cv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



using namespace cv;
using namespace std;

class MotionVector {
public:
    //Constructors
    MotionVector();
    //B Frame
    MotionVector(int blockSize, int mx, int my, Mat previousReferenceFrame, Mat targetFrame, Mat futureReferenceFrame);
    //P Frame
    MotionVector(int blockSize, int mx, int my, Mat previousReferenceFrame, Mat targetFrame);    
    //Getters and Setters
    void mvSetUV(int new_u, int new_v);
    int mvGetU();
    int mvGetV();

    //Search Algorithm
    void mvSequentialSearch(int p, int &instructionsExecuted);
    void mvLogarithmicSearch(int p, int &instructionsExecuted);
    Mat retrievePredictedBlock();
    
    
private:
    Mat tFrame; Mat previousReferenceFrame; Mat futureReferenceFrame; //tFrame = target, previousRFrame = previous reference, futureRFrame, future reference
    MotionVector* motionVectors;
    int blockSize;
    int u; //(u,v) is the motion vector
    int v; 
    int x; //(x,y) is upper left corner of macroblock
    int y;
};