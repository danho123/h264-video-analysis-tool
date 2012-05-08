/*****************************************
 CMPT 365: Summer 2011
 Final Project: H261/H264 Encoder and Decoder
 Created By: Dan Ho and Kenneth Wong
 Professor: Dr. Ze-Nian Li
 Date: July 30th 2011
 ******************************************/
#include "MotionVector.h"
#define LARGE_NUMBER 1000000
MotionVector::MotionVector() {
    u = 0;
    v = 0;
}

MotionVector::MotionVector(int size, int mx, int my, Mat previousFrame, Mat currentFrame){
    blockSize = size;
    
    x = mx;
    y = my;
    u = 0;
    v = 0;
    
    previousReferenceFrame = previousFrame;
    tFrame = currentFrame;
}

//setters and getters
void MotionVector::mvSetUV(int new_u, int new_v)
{
    u = new_u;
    v = new_v;
}


int MotionVector::mvGetU(){
    return u;
}

int MotionVector::mvGetV(){
    return v;
}


//Search Algorithms
void MotionVector::mvSequentialSearch(int p, int &instructionsExecuted){
    cvtColor(tFrame, tFrame, CV_BGR2YUV);
    cvtColor(previousReferenceFrame, previousReferenceFrame, CV_BGR2YUV);
    
    Scalar differenceVector;
    float curr_MAD;
    float min_MAD = LARGE_NUMBER;
    
    int temp = ((2*p+1)/2-blockSize);
    int searchWidth = 2*p+1+blockSize;
    int searchHeight = searchWidth;
    
    int searchOriginX = x-temp-1;
    int searchOriginY = y-temp-1;
    
    //set bounds if search window is out of range of frame
    if(searchOriginX < 0){ searchOriginX = 0;}
    if(searchOriginY < 0){ searchOriginY = 0;}
    if(searchOriginX+searchWidth >= previousReferenceFrame.cols){ searchWidth = previousReferenceFrame.cols-searchOriginX;}
    if(searchOriginY+searchHeight >= previousReferenceFrame.rows){ searchHeight = previousReferenceFrame.rows-searchOriginY;}
    
    Mat targetMacroBlock(tFrame, Rect(x, y, blockSize, blockSize));
    Mat differenceBlock;
    
    for(int i = searchOriginX; i < searchOriginX + searchWidth - blockSize+1; i++){
        for(int j = searchOriginY; j < searchOriginY + searchHeight - blockSize+1; j++){
            instructionsExecuted++;
            Mat referenceMacroBlock(previousReferenceFrame, Rect(i, j, blockSize, blockSize));
            absdiff(targetMacroBlock, referenceMacroBlock, differenceBlock);
            differenceVector = sum(differenceBlock);
            curr_MAD = differenceVector[0]; // only get Y channel

            if(curr_MAD < min_MAD){
                 min_MAD = curr_MAD;
                 u = i - x;
                 v = j - y;
            }
        }
    }
    
    
    cvtColor(tFrame, tFrame, CV_YUV2BGR);
    cvtColor(previousReferenceFrame, previousReferenceFrame, CV_YUV2BGR);
    
}

void MotionVector::mvLogarithmicSearch(int p, int &instructionsExecuted){
   
    cvtColor(tFrame, tFrame, CV_BGR2YUV);
    cvtColor(previousReferenceFrame, previousReferenceFrame, CV_BGR2YUV);
    
    Scalar differenceVector;
    float curr_MAD;
    float min_MAD = LARGE_NUMBER;
    int newX;
    int newY;

    Mat targetMacroBlock(tFrame, Rect(x, y, blockSize, blockSize));
    Mat differenceBlock;

    int searchOriginX = x;
    int searchOriginY = y;
    while(ceil((double)p/2) > 1){

        for(int g = ceil(-p/2); g < ceil(p/2); g = g + ceil(p/2)){
            for(int h = ceil(-p/2); h < ceil(p/2); h = h + ceil(p/2)){
                instructionsExecuted++;
                newX = searchOriginX + g;
                newY = searchOriginY + h;
                
                if(newX < 0){ newX = 0;}
                if(newY < 0){ newY = 0;}
                if(newX+blockSize >= previousReferenceFrame.cols){ newX = previousReferenceFrame.cols-blockSize;}
                if(newY+blockSize >= previousReferenceFrame.rows){ newY = previousReferenceFrame.rows-blockSize;}
                
                //printf("%d %d\n", newX, newY);
                
                Mat referenceMacroBlock(previousReferenceFrame, Rect(newX, newY, blockSize, blockSize));
                absdiff(targetMacroBlock, referenceMacroBlock, differenceBlock);
                differenceVector = sum(differenceBlock);
                curr_MAD = differenceVector[0]; // only get Y channel
                
                if(curr_MAD < min_MAD){
                    min_MAD = curr_MAD;
                    u = newX - x;
                    v = newY - y;
                }
            }
        }
        p = ceil((double)p/2);
        searchOriginX = x+u;
        searchOriginY = y+v;
    }
    cvtColor(tFrame, tFrame, CV_YUV2BGR);
    cvtColor(previousReferenceFrame, previousReferenceFrame, CV_YUV2BGR);
}

Mat MotionVector::retrievePredictedBlock(){

    Mat predictedBlock(previousReferenceFrame, Rect(x+u,y+v, blockSize, blockSize));
    return predictedBlock;
    
}
