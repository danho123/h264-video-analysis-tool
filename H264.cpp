/*****************************************
 CMPT 365: Summer 2011
 Final Project: H261/H264 Encoder and Decoder
 Created By: Dan Ho and Kenneth Wong
 Professor: Dr. Ze-Nian Li
 Date: July 30th 2011
 ******************************************/
#include "H264.h"

double const Pi=3.141592653589793;

//variables for CompareGUI
int playback_elem = 0;
Mat* GUIoriginalFrame;
Mat* GUIencodedFrame;
Mat* GUIdifferenceFrame;

//variables for StageGUI
Mat frameStages[2][5];
int frameType = 0;
int frameStage = 0;
int GUImbsize;
int const maxFrameTypes = 1;
int const maxFrameStages = 4;
int GUIp;
const char* GUIalgorithm;

/** Function Headers */
void Stage( int, void* );
void Playback( int, void* );
void mouseHandlerOriginal(int event, int x, int y, int flags, void* param);
void mouseHandlerEncoded(int event, int x, int y, int flags, void* param);
void mouseHandlerDifference(int event, int x, int y, int flags, void* param);
void mouseHandlerMotionVector(int event, int x, int y, int flags, void* param);

int intraMatrix[8][8] = {
    {8,16,19,22,26,27,29,34},
    {16,16,22,24,27,29,34,37},
    {19,22,26,27,29,34,34,37},
    {22,22,26,27,29,34,37,40}, 
    {22,26,27,29,32,35,40,48},
    {26,27,29,32,35,40,48,58},
    {26,27,29,34,38,46,56,69},
    {27,29,35,38,46,56,69,83}
};

int interMatrix[8][8] = {
    {16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16}
};

//Constructors
H264::H264() {}
H264::H264(char* videoFile){
    
    capture = cvCaptureFromAVI( videoFile );
    fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    frameCount = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
    width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    videoName = videoFile;
    frameSequence = new Mat[frameCount];
    encodedFrameSequence = new Mat[frameCount];
    M = 3;
    N = 3;
    GUIoriginalFrame = frameSequence;
    GUIencodedFrame = encodedFrameSequence;
    GUIdifferenceFrame = new Mat[frameCount];
    mvs = new MotionVector[width*height/2*16];
    p = 15;
    mbsize = 16;
    scale = 1;
    qp = 0;
    compressionType = "H261";
    searchAlgorithm = "Sequential";
    hierarchical = false;
    instructionsExecuted = 0;
    
}

void H264::createFrameSequence(){

    Mat frame;
    int frame_number = 0;
    
    while (1) 
    {
        // get frame 
        frame = cvQueryFrame(capture);
        
        if (frame_number >= frameCount) 
        {
            
            printf("Resolution: %dx%d\nFPS: %d\nTotal Frames: %d\n", width, height,fps, frameCount);
            break;
        } 
        
        if(frame.rows%8 != 0 | frame.cols%8 != 0){padFrames(frame);};
        
        GUIencodedFrame[frame_number] = frame.clone();
        frameSequence[frame_number] = frame.clone();
        frame_number++;
    }
    // free resources
    frame.release();
}

//Encoding Component
void H264::encodeVideo(){
    instructionsExecuted = 0;
    //H261
    if((strcmp(compressionType, "H261") == 0)){
        for(int i = 0; i < frameCount; i++){
            if(i%(N+1) == 0){
                //Convert to RGB --> YUV
                cvtColor(encodedFrameSequence[i], encodedFrameSequence[i], CV_BGR2YUV);
                //Perform Chromasampling on the whole image
                chromaSample(encodedFrameSequence[i]);
                dctCompression(encodedFrameSequence[i]);
                //Convert to YUV -> RGB
                cvtColor(encodedFrameSequence[i], encodedFrameSequence[i], CV_YUV2BGR);
                printf("I%i\n", i);
                
            }
            else{
                encodePFrame(encodedFrameSequence[i-1], encodedFrameSequence[i]);
                printf("P%i\n", i);                
            }
        }
    }
    
    Mat currentPreviousFrame;
    Mat currentFutureFrame;
    
    //H264
    if((strcmp(compressionType, "H264") == 0)){
        int lastFrame;
        for(int i = 0; i < frameCount; i = i + M){
            lastFrame = i;
            if(i%(N) == 0){
                //Convert to RGB --> YUV
                cvtColor(encodedFrameSequence[i], encodedFrameSequence[i], CV_BGR2YUV);
                //Perform Chromasampling on the whole image
                chromaSample(encodedFrameSequence[i]);
                dctCompression(encodedFrameSequence[i]);
                //Convert to YUV -> RGB
                cvtColor(encodedFrameSequence[i], encodedFrameSequence[i], CV_YUV2BGR);
                printf("I%i\n", i);
                if(i != 0){
                    for(int j = i-M+1; j<i; j++){
                        encodeBFrame(encodedFrameSequence[i-M], encodedFrameSequence[j], encodedFrameSequence[i]);
                        printf("B%i\n", j);  
                    }
                }
            }
            else{
                encodePFrame(encodedFrameSequence[i-M], encodedFrameSequence[i]);
                printf("P%i\n", i);   
                for(int j = i-M+1; j<i; j++){
                    encodeBFrame(encodedFrameSequence[i-M], encodedFrameSequence[j], encodedFrameSequence[i]);
                    printf("B%i\n", j);  
                }
            }
        }
        //encode remaining frames as I frames
        for(int i = lastFrame+1; i < frameCount; i++){
            //Convert to RGB --> YUV
            printf("I%i\n", i);
            cvtColor(encodedFrameSequence[i], encodedFrameSequence[i], CV_BGR2YUV);
            //Perform Chromasampling on the whole image
            chromaSample(encodedFrameSequence[i]);
            dctCompression(encodedFrameSequence[i]);
            //Convert to YUV -> RGB
            cvtColor(encodedFrameSequence[i], encodedFrameSequence[i], CV_YUV2BGR);

        }
    }
}

void H264::encodePFrame(Mat previousReferenceFrame, Mat &targetFrame){
    int counter = 0; //keeps count of # of motion vector
    
    //initialize empty frame which will be used to build predicted frame (P')
    Mat predictedFrame = Mat::zeros(targetFrame.size(), targetFrame.type());
    //find a corresponding motion vector per macro block
    for(int rowBlock = 0; rowBlock < width/mbsize; rowBlock++){
        for(int colBlock = 0; colBlock < (height/mbsize); colBlock++){
            //call sequential search to find motion vector
            mvs[counter] = MotionVector(mbsize, rowBlock*mbsize, colBlock*mbsize, previousReferenceFrame, targetFrame);
            if((strcmp(searchAlgorithm, "Sequential") == 0)){
                mvs[counter].mvSequentialSearch(p, instructionsExecuted);
            }
            if((strcmp(searchAlgorithm, "Logarithmic") == 0)){
                mvs[counter].mvLogarithmicSearch(p, instructionsExecuted);
            }

            //set ROI on the predicted frame
            Mat predictedROI(predictedFrame, Rect(rowBlock*mbsize, colBlock*mbsize, mbsize, mbsize));
            //retrieve 'best match' macro block from the reference frame
            Mat predictedMV = mvs[counter].retrievePredictedBlock();
            
            //paint 'best match' block onto the predicted frame
            predictedMV.copyTo(predictedROI); 
            counter++;
        }
    }
    //imshow("Predicted", predictedFrame);
    //Initialize required frames for DCT/Integer transform
    //D
    Mat differenceFrame(targetFrame.size(), CV_32FC3);
    //D~
    Mat dctDifferenceFrame(targetFrame.size(), CV_32FC3);
    //P~
    Mat encodedP(targetFrame.size(), CV_32FC3);
    
    //convert targetFrame and predictedFrames to a 16-bit signed integer Mat type
    targetFrame.convertTo(targetFrame, CV_32FC3);
    predictedFrame.convertTo(predictedFrame,CV_32FC3);
    
    //D = P - P'
    subtract(targetFrame, predictedFrame, differenceFrame);
    //D~ = DCT(D)
    if((strcmp(compressionType, "H261") == 0)){
        dctCompression(differenceFrame); //D compression (change to integer transform for h264)
    }
    if((strcmp(compressionType, "H264") == 0)){
        integerTransform(differenceFrame);     
    }

    //P~ = P' + D~
    add(predictedFrame, differenceFrame, encodedP); //final encoded P frame: 
    
    //convert back to a 8 bit unsigned integer Mat type
    targetFrame.convertTo(targetFrame, CV_8UC3);
    predictedFrame.convertTo(predictedFrame, CV_8UC3);
    differenceFrame.convertTo(differenceFrame, CV_8UC3);
    encodedP.convertTo(encodedP, CV_8UC3);

    //write to encodedFrameSequence
    encodedP.copyTo(targetFrame); //copy to encoded video sequence
}

void H264::encodeBFrame(Mat previousReferenceFrame, Mat &targetFrame, Mat futureReferenceFrame){
    
    //initialize empty frame which will be used to build predicted frame (P')
    Mat predictedFrame = Mat::zeros(targetFrame.size(), targetFrame.type());
    Mat twosFrame = Mat::zeros(mbsize, mbsize, CV_32FC3);
    for (int row = 0; row < mbsize; row++){
        for (int col = 0; col < mbsize; col++){
            twosFrame.at<Vec3f>(row,col)[0] = 2.0;
            twosFrame.at<Vec3f>(row,col)[1] = 2.0;
            twosFrame.at<Vec3f>(row,col)[2] = 2.0;
        }
    }
    
    //find a corresponding motion vector per macro block
    for(int rowBlock = 0; rowBlock < width/mbsize; rowBlock++){
        for(int colBlock = 0; colBlock < (height/mbsize); colBlock++){
            //call sequential search to find motion vector
            MotionVector preV = MotionVector(mbsize, rowBlock*mbsize, colBlock*mbsize, previousReferenceFrame, targetFrame);
            MotionVector futV = MotionVector(mbsize, rowBlock*mbsize, colBlock*mbsize, futureReferenceFrame, targetFrame);
            if((strcmp(searchAlgorithm, "Sequential") == 0)){
                preV.mvSequentialSearch(p, instructionsExecuted);
                futV.mvSequentialSearch(p, instructionsExecuted);
            }
            if((strcmp(searchAlgorithm, "Logarithmic") == 0)){
                preV.mvLogarithmicSearch(p, instructionsExecuted);
                futV.mvLogarithmicSearch(p, instructionsExecuted);
            }
            //set ROI on the predicted frame
            Mat predictedROI(predictedFrame, Rect(rowBlock*mbsize, colBlock*mbsize, mbsize, mbsize));
            
            //retrieve 'best match' macro block from the reference frame
            Mat previousReferenceROI = preV.retrievePredictedBlock();
            Mat futureReferenceROI = futV.retrievePredictedBlock();
            
            //CONVERT THE 'best match' FRAMES TO FLOAT NUMBERS.
            previousReferenceROI.convertTo(previousReferenceROI, CV_32FC3);
            futureReferenceROI.convertTo(futureReferenceROI, CV_32FC3);
            
            //ADD PREVEIOUSREFERENCEROI TO FUTUREREFERENCEROI
            Mat summedROI(previousReferenceROI.size(), CV_32FC3);
            add(previousReferenceROI, futureReferenceROI, summedROI);
            
            //DIVIDE THE ADDED FRAMES BY 2 -> AVERAGE BLOCK
            Mat dividedROI(previousReferenceROI.size(), CV_32FC3);
            divide(summedROI, twosFrame, dividedROI);
            
            //CONVERT THE AVERAGE BLOCK BACK TO INTEGER
            Mat averageROI;
            dividedROI.convertTo(averageROI, CV_8UC3);
            
            //paint 'best match' block onto the predicted frame
            averageROI.copyTo(predictedROI); 
        }
    }
    Mat differenceFrame(targetFrame.size(), CV_32FC3);
    //D~
    Mat dctDifferenceFrame(targetFrame.size(), CV_32FC3);
    //P~
    Mat encodedB(targetFrame.size(), CV_32FC3);
    targetFrame.convertTo(targetFrame, CV_32FC3);
    predictedFrame.convertTo(predictedFrame,CV_32FC3);
    //D = P - P'
    subtract(targetFrame, predictedFrame, differenceFrame);
    //D~ = IntegerTransform(D)
    integerTransform(differenceFrame);
    //P~ = P' + D~
    add(predictedFrame, differenceFrame, encodedB); //final encoded P frame: 
    
    //convert back to a 8 bit unsigned integer Mat type
    targetFrame.convertTo(targetFrame, CV_8UC3);
    predictedFrame.convertTo(predictedFrame, CV_8UC3);
    differenceFrame.convertTo(differenceFrame, CV_8UC3);
    encodedB.convertTo(encodedB, CV_8UC3);
    
    //write to encodedFrameSequence
    encodedB.copyTo(targetFrame); //copy to encoded video sequence
}

//Debug Functions
void H264::saveFrames(){
    //needs work
    VideoWriter* newVideo = new VideoWriter("/Users/danho/Projects/Multimedia_Tools/H264_Video_Compression/Video/encoded.avi", CV_FOURCC('D','I','B',' '), fps, frameSequence[0].size(), 1);
    newVideo->open("/Users/danho/Projects/Multimedia_Tools/H264_Video_Compression/Video/encoded.avi", CV_FOURCC('D','I','B',' '), fps, frameSequence[0].size(), 1);
    for(int i = 0; i < frameCount; i++){
        newVideo->write(encodedFrameSequence[i]);
        printf("Saving %dth frame.\n", i);
    }
}

void H264::showVideo(){
    Mat frame;
    // display video
    int key = 0;
    cvNamedWindow( "video", 0 );
    int frameNumber = 0;
    while(key != 'q') {
        /* get a frame */
        frame = encodedFrameSequence[frameNumber];
        // im = frame;
        frameNumber++;
        /* always check */
        if(frameNumber >= frameCount) break;
        
        /* display frame */
        imshow("video", frame);
        
        /* quit if user press 'q' */
        key = cvWaitKey(30);
    }
    
    /* free memory */
    cvDestroyWindow("video");
}


//DCT Transform
void H264::dctCompression(Mat &iFrame){
    int Y[8][8], U[8][8], V[8][8];
    int externalRow = 0, externalCol = 0;

    while (externalRow < iFrame.rows) {
        while (externalCol < iFrame.cols){
            //set ROI
            Mat roi(iFrame, Rect(externalCol, externalRow, 7, 7));
            
            //Generate YUV Array Values for an 8 By 8 block of the iFrame
            for (int row = 0; row<8; row++){
                for (int col = 0; col<8; col++){
                    if(iFrame.type() == CV_8UC3){
                        Y[row][col] = roi.at<Vec3b>(row,col)[0];
                        U[row][col] = roi.at<Vec3b>(row,col)[1];
                        V[row][col] = roi.at<Vec3b>(row,col)[2];
                    }
                    else{
                        Y[row][col] = roi.at<Vec3f>(row,col)[0];
                        U[row][col] = roi.at<Vec3f>(row,col)[1];
                        V[row][col] = roi.at<Vec3f>(row,col)[2];
                    }
                }
            }
            //DCT
            dct_1D_1(Y);
            dct_1D_1(U);
            dct_1D_1(V);


            //Perform Quantization
            //I Frame
            if(iFrame.type() == CV_8UC3){
                quantization(Y, intraMatrix);
                quantization(U, intraMatrix);
                quantization(V, intraMatrix);
    //            
                //Perform Dequantization
                dequantization(Y, intraMatrix);
                dequantization(U, intraMatrix);
                dequantization(V, intraMatrix);
            }
            //P Frame
            else{
                quantization(Y, interMatrix);
                quantization(U, interMatrix);
                quantization(V, interMatrix);
                //            
                //Perform Dequantization
                dequantization(Y, interMatrix);
                dequantization(U, interMatrix);
                dequantization(V, interMatrix);
            }
            
            //Perform IDCT
            idct_1D_1(Y);
            idct_1D_1(U);
            idct_1D_1(V);
            
            //Paint to the new canvas
            for (int row = 0; row<8; row++){
                for (int col = 0; col<8; col++){
                    if(iFrame.type() == CV_8UC3){
                        roi.at<Vec3b>(row,col)[0] = saturate_cast<uchar>(Y[row][col]);
                        roi.at<Vec3b>(row,col)[1] = saturate_cast<uchar>(U[row][col]);
                        roi.at<Vec3b>(row,col)[2] = saturate_cast<uchar>(V[row][col]);
                    }
                    else{
                        roi.at<Vec3f>(row,col)[0] = saturate_cast<float>(Y[row][col]);
                        roi.at<Vec3f>(row,col)[1] = saturate_cast<float>(U[row][col]);
                        roi.at<Vec3f>(row,col)[2] = saturate_cast<float>(V[row][col]);
                    }
                }
            }
            //Change external Col
            externalCol = externalCol + 8;
        }
        //Set externalCol back to ZERO
        externalCol = 0;
        //Change external Row
        externalRow = externalRow + 8;
    }
}

void H264::chromaSample(Mat &iFrame){
    int avgU;
    int avgV;
    
    for(int rowBlock = 0; rowBlock < iFrame.cols/2; rowBlock++){
        for(int colBlock = 0; colBlock < (iFrame.rows/2); colBlock++){
            Mat roi(iFrame, Rect(rowBlock*2,colBlock*2,1,1));
            avgU = (roi.at<Vec3b>(0,0)[1] + roi.at<Vec3b>(0,1)[1])/2;
            avgV = (roi.at<Vec3b>(0,0)[2] + roi.at<Vec3b>(0,1)[2])/2;
            
            roi.at<Vec3b>(0,0)[1] = saturate_cast<uchar>(avgU);
            roi.at<Vec3b>(0,1)[1] = saturate_cast<uchar>(avgU);
            roi.at<Vec3b>(1,0)[1] = saturate_cast<uchar>(avgU);
            roi.at<Vec3b>(1,1)[1] = saturate_cast<uchar>(avgU);
            
            roi.at<Vec3b>(0,0)[2] = saturate_cast<uchar>(avgV);
            roi.at<Vec3b>(0,1)[2] = saturate_cast<uchar>(avgV);
            roi.at<Vec3b>(1,0)[2] = saturate_cast<uchar>(avgV);
            roi.at<Vec3b>(1,1)[2] = saturate_cast<uchar>(avgV);
        }
    }
}

void H264::dct_1D_2(double dct[8][8], int (&dst)[8][8]){
    //F(u,v)
    int calc_dct[8][8];
    memcpy(calc_dct, dst, 64*sizeof(int));
    
    double temp;
    double c;
    for(int u=0; u<8; u++){
        for(int v=0;v<8;v++){
            temp = 0;
            for(int i=0; i<8;i++){
                //printf("%i ", f[i][v]);
                temp = temp + cos((2*i+1)*u*Pi/16)*(dct[i][v]);
            }
            if(u==0){
                c = sqrt(2)/2;
            }
            else{
                c = 1;
            }
            temp = temp*c/2;
            //printf("%f ", temp);
            calc_dct[u][v] = round(temp);
        }
        //printf("\n");
    }
    //copy back to original array
    memcpy(dst, calc_dct, 64*sizeof(int));
}

void H264::dct_1D_1(int (&dct)[8][8]){
    //G(i,v) - 1D DCT on columns
    
    double calc_dct[8][8];
    
	memcpy(calc_dct, dct, 64*sizeof(double));
    
    double temp;
    double c;
    for(int i=0; i<8; i++){
        for(int v=0;v<8;v++){
            temp = 0;
            for(int j=0; j<8;j++){
                dct[i][j] -= 128;
                temp +=cos((2*j+1)*v*Pi/16)*dct[i][j];
            }
            if(v==0){
                c = sqrt(2)/2;
            }
            else{
                c = 1;
            }
            temp = temp*c/2;
            calc_dct[i][v] = temp;
        }
    }
    
    dct_1D_2(calc_dct, dct);
}

void H264::quantization(int (&matrix)[8][8], int qMatrix[8][8]){
    int temp = 0;
    //H261 Quantization
    if((strcmp(compressionType, "H261") == 0)){
        for(int row = 0; row <8; row++){
            for(int col = 0; col <8; col++){
                //DC Component is always 8
                if(row == 0 && col==0){
                    temp = round((double)matrix[row][col]/(double)8);
                    matrix[row][col] = temp;
                }
                else{
                //AC Component
                    temp = round((double)matrix[row][col]/((double)2*scale));
                    matrix[row][col] = temp;
                }
            }
        }
    }
    //H264
    else{
        for(int row = 0; row <8; row++){
            for(int col = 0; col <8; col++){
                temp = round((double)matrix[row][col]/((double)qMatrix[row][col]*scale));
                matrix[row][col] = temp;
            }
        }
    }
}

void H264::dequantization(int (&matrix)[8][8], int qMatrix[8][8]){
    int temp = 0;
    //H261 Dequantization
    if((strcmp(compressionType, "H261") == 0)){
        for(int row = 0; row <8; row++){
            for(int col = 0; col <8; col++){
                //DC Component
                if(row == 0 && col==0){
                    temp = matrix[row][col]*8;
                    matrix[row][col] = temp;
                }
                else{
                    temp = matrix[row][col]*2*scale;
                    matrix[row][col] = temp;
                }
            }
        }
    }
    //H264
    else{
        for(int row = 0; row <8; row++){
            for(int col = 0; col <8; col++){
                temp = matrix[row][col]*qMatrix[row][col]*scale;
                matrix[row][col] = temp;
            }
        }
    }
}

void H264::idct_1D_1(int (&matrix)[8][8]){
    
    double calc_dct[8][8];    
	memcpy(calc_dct, matrix, 64*sizeof(double));    
    double temp;
    double c;
    
    for(int row=0; row<8; row++){
        for(int i=0;i<8;i++){
            temp = 0;
            for(int u=0; u<8;u++){
                if(u==0){
                    c = sqrt(2)/2;
                }
                else{
                    c = 1;
                }
                temp +=c*cos((2*i+1)*u*Pi/16)*matrix[row][u]/2;
                calc_dct[row][i] = temp;
            }
        }
    }
    idct_1D_2(calc_dct, matrix);
}

void H264::idct_1D_2(double (*src)[8], int (&dst)[8][8]){
      
    double temp;
    double c;
    
    for(int col=0; col<8; col++){
        for(int i=0;i<8;i++){
            temp = 0;
            for(int u=0; u<8;u++){
                if(u==0){
                    c = sqrt(2)/2;
                }
                else{
                    c = 1;
                }
                temp +=c*cos((2*i+1)*u*Pi/16)*src[u][col]/2;
            }
            dst[i][col] = round(temp) + 128;
        }
    }
}

//setters and getters
void H264::setFrameSequence(int index, Mat matrix){
    frameSequence[index] = matrix;
}

Mat H264::getFrameSequence(int index){
    return frameSequence[index];
}

Mat H264::getEncodedFrameSequence(int index){
    return encodedFrameSequence[index];
}

int H264::getFrameCount(){
    return frameCount;
}

int H264::getFPS(){
    return fps;
}

int H264::getWidth(){
    return width;
}

int H264::getHeight(){
    return height;
}

int H264::getInstructions(){
    return instructionsExecuted;
}

void H264::setM(int newM){
    M = newM;
}

void H264::setN(int newN){
    N = newN;
}

void H264::setScale(int newScale){
    scale = newScale;
}

void H264::setSearchAlgorithm(const char* newAlgorithm){
    searchAlgorithm = newAlgorithm;
}
void H264::setCompressionType(const char* newType){
    compressionType = newType;
}
void H264::setHierarchical(bool boolean){
    hierarchical = boolean;
}
void H264::setMacroBlockSize(int newSize){
    mbsize = newSize;
}

void H264::setQP(int newQP){
    qp = newQP;
}

void H264::setSearchWindowSize(int newP){
    p = newP;
}

//Integer Transform Functions
void H264::integerTransform(Mat &differenceFrame){
    //Y = (C*X*CT.M)/2^15
    float R[4][4];
    float G[4][4];
    float B[4][4];
    for(int rowBlock = 0; rowBlock < width/4; rowBlock++){
        for(int colBlock = 0; colBlock < (height/4); colBlock++){
            Mat block(differenceFrame, Rect(rowBlock*4, colBlock*4, 4, 4));

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    R[i][j] = block.at<Vec3f>(i,j)[0];
                    G[i][j] = block.at<Vec3f>(i,j)[1];
                    B[i][j] = block.at<Vec3f>(i,j)[2];
                }
            }
                   
            fIntegerTransform(R);
            fIntegerTransform(G);
            fIntegerTransform(B);
            
            integerQuantization(R);
            integerQuantization(G);
            integerQuantization(B);
            
            integerDequantization(R);
            integerDequantization(G);
            integerDequantization(B);
            
            iIntegerTransform(R);
            iIntegerTransform(G);
            iIntegerTransform(B);
            
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    block.at<Vec3f>(i,j)[0] = saturate_cast<float>(R[i][j]);
                    block.at<Vec3f>(i,j)[1] = saturate_cast<float>(G[i][j]);
                    block.at<Vec3f>(i,j)[2] = saturate_cast<float>(B[i][j]);
                }
            }
        }
    }
}

void H264::fIntegerTransform(float (&arr1)[4][4]){
    float temp[4][4];
    for(int i = 0; i<4; i++){
        temp[0][i] = arr1[0][i] + arr1[1][i] + arr1[2][i] + arr1[3][i];
        temp[1][i] = 2*arr1[0][i] + arr1[1][i] - arr1[2][i] - 2*arr1[3][i];
        temp[2][i] = arr1[0][i] - arr1[1][i] - arr1[2][i] + arr1[3][i];
        temp[3][i] = arr1[0][i] - 2*arr1[1][i] + 2*arr1[2][i] - arr1[3][i];
    }
    for(int i = 0; i<4; i++){
        arr1[i][0] = temp[i][0] + temp[i][1] + temp[i][2] + temp[i][3];
        arr1[i][1] = 2*temp[i][0] + temp[i][1] - temp[i][2] - 2*temp[i][3];
        arr1[i][2] = temp[i][0] - temp[i][1] - temp[i][2] + temp[i][3];
        arr1[i][3] = temp[i][0] - 2*temp[i][1] + 2*temp[i][2] - temp[i][3];
    }
}

void H264::integerQuantization(float (&matrix)[4][4]){
    
    int stepsize = pow(2, floor(qp/6));
    int qStep = qp%6;
    //printf("%d %d\n", qStep, stepsize);
    float M[6][3] = {
        {13107, 5243, 8066},
        {11916, 4660, 7490},
        {10082, 4194, 6554},
        {9362, 3647, 5825},
        {8192, 3355, 5243},
        {7282, 2893, 4559}
    };
    
    float mfArr[4][4] = {
        {M[qStep][0], M[qStep][2], M[qStep][0], M[qStep][2]},
        {M[qStep][2], M[qStep][1], M[qStep][2], M[qStep][1]},
        {M[qStep][0], M[qStep][2], M[qStep][0], M[qStep][2]},
        {M[qStep][2], M[qStep][1], M[qStep][2], M[qStep][1]}
    };
    
    for(int i = 0; i<4; i++){
        for(int j=0; j<4; j++){

            matrix[i][j] = round(matrix[i][j] * (mfArr[i][j]/stepsize)/32768);
        }
    }
    
}
void H264::integerDequantization(float (&matrix)[4][4]){
    
    int stepsize = pow(2, floor(qp/6));
    int qStep = qp%6;
    
    float V[6][3] = {
        {10, 16, 13},
        {11, 18, 14},
        {13, 20, 16},
        {14, 23, 18},
        {16, 25, 20},
        {18, 29, 23}
    };
    
    float viArr[4][4] = {
        {V[qStep][0], V[qStep][2], V[qStep][0], V[qStep][2]},
        {V[qStep][2], V[qStep][1], V[qStep][2], V[qStep][1]},
        {V[qStep][0], V[qStep][2], V[qStep][0], V[qStep][2]},
        {V[qStep][2], V[qStep][1], V[qStep][2], V[qStep][1]}
    };
    
    for(int i = 0; i<4; i++){
        for(int j=0; j<4; j++){
            matrix[i][j] = matrix[i][j] * viArr[i][j] * stepsize;
        }
    }
}

void H264::iIntegerTransform(float (&arr1)[4][4]){
    float temp[4][4];
    for(int i = 0; i<4; i++){
        temp[0][i] = arr1[0][i] + arr1[1][i] + arr1[2][i] + .5*arr1[3][i];
        temp[1][i] = arr1[0][i] + .5*arr1[1][i] - arr1[2][i] - arr1[3][i];
        temp[2][i] = arr1[0][i] - .5*arr1[1][i] - arr1[2][i] + arr1[3][i];
        temp[3][i] = arr1[0][i] - arr1[1][i] + arr1[2][i] - .5*arr1[3][i];
    }
    for(int i = 0; i<4; i++){
        arr1[i][0] = round(temp[i][0] + temp[i][1] + temp[i][2] + .5*temp[i][3])/64;
        arr1[i][1] = round(temp[i][0] + .5*temp[i][1] - temp[i][2] - temp[i][3])/64;
        arr1[i][2] = round(temp[i][0] - .5*temp[i][1] - temp[i][2] + temp[i][3])/64;
        arr1[i][3] = round(temp[i][0] - temp[i][1] + temp[i][2] - .5*temp[i][3])/64;
    }
    
}

//general functions
void H264::showPixels(Mat image, int channel){
    for (int i=0; i<image.rows; i++)
    {
        for (int j=0; j<image.cols; j++)
        {
            //printf("%i %i\n", i, j);
            printf("%f ", image.at<Vec3f>(i,j)[channel]);
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
}

void H264::showStats(){
    printf("File Path: %s\nResolution: %dx%d\nFPS: %d\nTotal Frames: %d\n", videoName, width, height,fps, frameCount);
    printf("testing");
    
}

Mat H264::createMatFromArray(int arr[8][8]){
    Mat image = Mat::zeros(8,8, CV_8SC3);   
    for (int i=0; i<image.rows; i++)
    {
        for (int j=0; j<image.cols; j++)
        {
            image.at<Vec3b>(i,j)[0] = arr[i][j];
        }
    }
    return image;
}

char* H264::int_to_string(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }
	
    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;
	
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );
	
    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

int H264::round(double f){
    int	i;
	i = f;
	if (f > 0)
        if ((f - i) >= 0.5)  return (i+1);
  	    else return (i);
        else
            if ((i - f) >= 0.5)  return (i-1);
            else return (i);
}

void H264::showMatrix(int arr[8][8]){
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) { printf("%d ",arr[i][j]); }
        printf("\n");
    }
    printf("------------------------------\n");
}

void H264::padFrames(Mat &frame){
    int padCol = 16 - frame.cols%16;
    int padRow = 16 - frame.rows%16;
    
    Mat newFrame = Mat::zeros(frame.cols+padCol, frame.rows+padRow, CV_8UC3);
    Mat roi(newFrame, Rect(0,0, frame.cols, frame.rows));
    frame.copyTo(roi);
    frame = newFrame.clone();
}

void H264::GUI(){
    
    GUIencodedFrame = encodedFrameSequence;
    
    for(int i = 0; i < frameCount; i++)
    {
        absdiff(GUIoriginalFrame[i], GUIencodedFrame[i], GUIdifferenceFrame[i]);
    }

    /// Create windows
    namedWindow( "Original Video", CV_WINDOW_AUTOSIZE);
    cvMoveWindow( "Original Video", 350, 0 );
    namedWindow( "Encoded Video", CV_WINDOW_AUTOSIZE);
    cvMoveWindow( "Encoded Video", 350, 0 );
    namedWindow( "Difference Video", CV_WINDOW_AUTOSIZE);
    cvMoveWindow( "Difference Video", 350, 0 );

    createTrackbar( "Play Bar", "Original Video",
                   &playback_elem, frameCount-1,
                   Playback);

    Playback( 0, 0);
    cvWaitKey(0);
    cvDestroyWindow("Original Video");
    cvDestroyWindow("Encoded Video");
    cvDestroyWindow("Difference Video");
}

void H264::stageGUI()
{
    /// Load an image
    
    //For I Frame
    frameStages[0][0] = GUIoriginalFrame[N].clone();
    
    cvtColor(frameStages[0][0], frameStages[0][1], CV_BGR2YUV);
    frameStages[0][2] = frameStages[0][0].clone();
    chromaSample(frameStages[0][2]);
    
    //dct
    frameStages[0][3] = frameStages[0][2].clone();
    
    //restored
    frameStages[0][4] = frameStages[0][1].clone();
    chromaSample(frameStages[0][4]);
    dctCompression(frameStages[0][4]);
    cvtColor(frameStages[0][4], frameStages[0][4], CV_YUV2BGR);
    
    int externalRow = 0;
    int externalCol = 0;
    int Y[8][8];
    int U[8][8];
    int V[8][8];
    while (externalRow < height) {
        while (externalCol < width){
            //set ROI
            Mat roi(frameStages[0][3], Rect(externalCol, externalRow, 7, 7));
            
            //Generate YUV Array Values for an 8 By 8 block of the iFrame
            for (int row = 0; row<8; row++){
                for (int col = 0; col<8; col++){
                    Y[row][col] = roi.at<Vec3b>(row,col)[0];
                    U[row][col] = roi.at<Vec3b>(row,col)[1];
                    V[row][col] = roi.at<Vec3b>(row,col)[2];

                }
            }
            //DCT
            dct_1D_1(Y);
            dct_1D_1(U);
            dct_1D_1(V);
            
            //Paint to the new canvas
            for (int row = 0; row<8; row++){
                for (int col = 0; col<8; col++){
                    roi.at<Vec3b>(row,col)[0] = saturate_cast<uchar>(Y[row][col]);
                    roi.at<Vec3b>(row,col)[1] = saturate_cast<uchar>(U[row][col]);
                    roi.at<Vec3b>(row,col)[2] = saturate_cast<uchar>(V[row][col]);
                    }              
            }
            //Change external Col
            externalCol = externalCol + 8;
        }
        //Set externalCol back to ZERO
        externalCol = 0;
        //Change external Row
        externalRow = externalRow + 8;
    }


    
    frameStages[1][0] = GUIoriginalFrame[M+N].clone();
    
    int counter = 0; //keeps count of # of motion vector
    
    //initialize empty frame which will be used to build predicted frame (P')
    Mat predictionFrame = Mat::zeros(frameStages[1][0].size(), frameStages[1][0].type());
    
    //find a corresponding motion vector per macro block
    for(int rowBlock = 0; rowBlock < width/mbsize; rowBlock++){
        for(int colBlock = 0; colBlock < (height/mbsize); colBlock++){
            //call sequential search to find motion vector
            MotionVector guiMV = MotionVector(mbsize, rowBlock*mbsize, colBlock*mbsize, frameStages[0][4], frameStages[1][0]);
            if((strcmp(searchAlgorithm, "Sequential") == 0)){
                guiMV.mvSequentialSearch(p, instructionsExecuted);
            }   
            if((strcmp(searchAlgorithm, "Logarithmic") == 0)){
                guiMV.mvLogarithmicSearch(p, instructionsExecuted);
            }           
            //set ROI on the predicted frame
            Mat predictedROI(predictionFrame, Rect(rowBlock*mbsize, colBlock*mbsize, mbsize, mbsize));
            //retrieve 'best match' macro block from the reference frame
            Mat predictedMV = guiMV.retrievePredictedBlock();
            //paint 'best match' block onto the predicted frame
            predictedMV.copyTo(predictedROI);
            counter++;
        }
    }

    //Initialize required frames for DCT/Integer transform
    //D
    Mat differenceFrame(frameStages[1][0].size(), CV_32FC3);
    //D~
    Mat dctDifferenceFrame(frameStages[1][0].size(), CV_32FC3);
    //P~
    Mat encodedP(frameStages[1][0].size(), CV_32FC3);
    Mat integerFrame(frameStages[1][0].size(), CV_32FC3);
    
    //convert targetFrame and predictedFrames to a 16-bit signed integer Mat type
    frameStages[1][0].convertTo(frameStages[1][0], CV_32FC3);
    predictionFrame.convertTo(predictionFrame,CV_32FC3);
    
    //D = P - P'
    subtract(frameStages[1][0], predictionFrame, differenceFrame);
    //D~ = DCT(D)

    
    float R[4][4];
    float G[4][4];
    float B[4][4];
    
    for(int rowBlock = 0; rowBlock < width/4; rowBlock++){
        for(int colBlock = 0; colBlock < (height/4); colBlock++){
            Mat block(differenceFrame, Rect(rowBlock*4, colBlock*4, 4, 4));
            Mat intBlock(integerFrame, Rect(rowBlock*4, colBlock*4, 4, 4));

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    R[i][j] = block.at<Vec3f>(i,j)[0];
                    G[i][j] = block.at<Vec3f>(i,j)[1];
                    B[i][j] = block.at<Vec3f>(i,j)[2];
                }
            }
            
            fIntegerTransform(R);
            fIntegerTransform(G);
            fIntegerTransform(B);
            
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    intBlock.at<Vec3f>(i,j)[0] = saturate_cast<float>(R[i][j]);
                    intBlock.at<Vec3f>(i,j)[1] = saturate_cast<float>(G[i][j]);
                    intBlock.at<Vec3f>(i,j)[2] = saturate_cast<float>(B[i][j]);
                }
            }
        }
    }
    
    integerTransform(differenceFrame);
    //P~ = P' + D~
    add(predictionFrame, differenceFrame, encodedP); //final encoded P frame: 
    
    //convert back to a 8 bit unsigned integer Mat type
    frameStages[1][0].convertTo(frameStages[1][0], CV_8UC3);
    predictionFrame.convertTo(predictionFrame, CV_8UC3);
    differenceFrame.convertTo(differenceFrame, CV_8UC3);
    encodedP.convertTo(encodedP, CV_8UC3);
    integerFrame.convertTo(integerFrame, CV_8UC3);
    
    frameStages[1][1] = predictionFrame.clone();
    frameStages[1][2] = differenceFrame.clone();
    frameStages[1][3] = integerFrame.clone();
    frameStages[1][4] = encodedP.clone();
    
    rectangle( frameStages[0][0], Point( 0, 0 ), Point( 125, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[0][1], Point( 0, 0 ), Point( 150, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[0][2], Point( 0, 0 ), Point( 200, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[0][3], Point( 0, 0 ), Point( 100, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[0][4], Point( 0, 0 ), Point( 140, 15), Scalar( 0, 0, 0 ), -1, 8);
    
    putText(frameStages[0][0],"I Frame: Original", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[0][1],"I Frame: RGB->YUV", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[0][2],"I Frame: Chroma Subsampled", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[0][3],"I Frame: DCT", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[0][4],"I Frame: Restored", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    
    rectangle( frameStages[1][0], Point( 0, 0 ), Point( 125, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[1][1], Point( 0, 0 ), Point( 140, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[1][2], Point( 0, 0 ), Point( 140, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[1][3], Point( 0, 0 ), Point( 180, 15), Scalar( 0, 0, 0 ), -1, 8);
    rectangle( frameStages[1][4], Point( 0, 0 ), Point( 140, 15), Scalar( 0, 0, 0 ), -1, 8);
    
    putText(frameStages[1][0],"P Frame: Original", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[1][1],"P Frame: Predicted", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[1][2],"P Frame: Difference", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[1][3],"P Frame: Integer Transform", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);
    putText(frameStages[1][4],"P Frame: Restored", Point(7, 12), CV_FONT_HERSHEY_PLAIN, .8,
            Scalar(255, 255, 255), .8, 4);

    /// Create windows
    namedWindow( "Frame Stages Demo", CV_WINDOW_AUTOSIZE );
    
    
    createTrackbar( "Type I or P/B", "Frame Stages Demo",
                   &frameType, maxFrameTypes,
                   Stage );
    
    createTrackbar( "Stage", "Frame Stages Demo",
                   &frameStage, maxFrameStages,
                   Stage );
    
    IplImage* img0 = new IplImage(frameStages[1][1]);
    GUImbsize = mbsize;
    GUIp = p;
    GUIalgorithm = searchAlgorithm;
    cvSetMouseCallback("Frame Stages Demo", mouseHandlerMotionVector, (void*)img0);
    
    /// Default start
    Stage( 0, 0 );
    waitKey(0);
    destroyWindow("Frame Stages Demo");
    destroyWindow("Best Matched Macroblock");
}

void Stage( int, void* )
{
    imshow("Frame Stages Demo", frameStages[frameType][frameStage]);
}

void Playback( int, void*)
{
    Mat newim = GUIoriginalFrame[playback_elem];
    Mat encim = GUIencodedFrame[playback_elem];
    Mat diffim = GUIdifferenceFrame[playback_elem];
    
    IplImage* img0 = new IplImage(newim);
    IplImage* img1 = new IplImage(encim);
    IplImage* img2 = new IplImage(diffim);
    
    cvSetMouseCallback("Original Video", mouseHandlerOriginal, (void*)img0);
    cvSetMouseCallback("Encoded Video", mouseHandlerEncoded, (void*)img1); 
    cvSetMouseCallback("Difference Video", mouseHandlerDifference, (void*)img2); 
    
    
    cvRectangle( img2, Point( 0, 0 ), Point( 125, 15), Scalar( 255, 255, 255 ), -1, 8);
    Mat tmp;
    Mat m1 = img0;
    Mat m2 = img1;
    absdiff( m1.reshape(1), m2.reshape(1), tmp);
    multiply(tmp, tmp, tmp);
    
    double MSE =  1.0/(tmp.cols * tmp.rows) * sum(tmp)[0];
    char label[20];
    CvFont    font;
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .7, .7, 0, 1, 8);
    MSE = 20 * log10(255.0 / sqrt(MSE));
    sprintf(label, "PSNR: %f dB", MSE);
    cvPutText(img2,label,cvPoint(7,12),&font,CV_RGB(0,0,0));
    imshow( "Original Video", newim);
    imshow( "Encoded Video", encim);
    imshow( "Difference Video", diffim);
}

void mouseHandlerOriginal(int event, int x, int y, int flags, void* param)
{
    IplImage* img0, * img1;
    CvFont    font;
    uchar*    ptr;
    char      label[20];

    img0 = (IplImage*) param;
    img1 = cvCloneImage(img0);
    

    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .8, .8, 0, 1, 8);
    
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        ptr = cvPtr2D(img1, y, x, NULL);

        sprintf(label, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
        
        cvRectangle(img1, cvPoint(x, y - 12), cvPoint(x + 100, y + 4), CV_RGB(0,0,0), CV_FILLED, 8, 0);
        cvPutText(img1,label,cvPoint(x, y),&font,CV_RGB(255,255,255));
        
        cvShowImage("Original Video", img1);
        
        Mat encim = GUIencodedFrame[playback_elem];
        IplImage* img2 = new IplImage(encim);
        mouseHandlerEncoded(event, x, y, flags, img2);
        
    }
}
void mouseHandlerEncoded(int event, int x, int y, int flags, void* param)
{
    IplImage* img0, * img1;
    CvFont    font;
    uchar*    ptr;
    char      label[20];
    
    img0 = (IplImage*) param;
    img1 = cvCloneImage(img0);
    
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .8, .8, 0, 1, 8);
    
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        ptr = cvPtr2D(img1, y, x, NULL);

        sprintf(label, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
        
        cvRectangle(img1, cvPoint(x, y - 12), cvPoint(x + 100, y + 4), CV_RGB(0,0,0), CV_FILLED, 8, 0);
        cvPutText(img1,label,cvPoint(x, y),&font,CV_RGB(255,255,255));
        
        cvShowImage("Encoded Video", img1);
        Mat diffim = GUIdifferenceFrame[playback_elem];
        IplImage* img2 = new IplImage(diffim);
        mouseHandlerDifference(event, x, y, flags, img2);
    }
}

void mouseHandlerDifference(int event, int x, int y, int flags, void* param)
{
    IplImage* img0, * img1;
    CvFont    font;
    uchar*    ptr;
    char      label[20];
    
    img0 = (IplImage*) param;
    img1 = cvCloneImage(img0);
    
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .8, .8, 0, 1, 8);
    
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        /* read pixel */
        ptr = cvPtr2D(img1, y, x, NULL);
        
        /*
         * display the BGR value
         */
        sprintf(label, "(%d, %d, %d)", ptr[0], ptr[1], ptr[2]);
        
        cvRectangle(img1, cvPoint(x, y - 12), cvPoint(x + 100, y + 4), CV_RGB(0,0,0), CV_FILLED, 8, 0);
        cvPutText(img1,label,cvPoint(x, y),&font,CV_RGB(255,255,255));
        
        cvShowImage("Difference Video", img1);
    }
}

void mouseHandlerMotionVector(int event, int x, int y, int flags, void* param)
{
    
    int mx = floor(x/GUImbsize)*GUImbsize;
    int my = floor(y/GUImbsize)*GUImbsize;
    int u,v;
    if(frameStage == 1 && frameType == 1){
        MotionVector mv = MotionVector(GUImbsize, mx, my, frameStages[0][4], frameStages[1][0]);
        int temp = 0;
        if(strcmp(GUIalgorithm, "Sequential")==0){
            mv.mvSequentialSearch(GUIp, temp);
            u = mv.mvGetU();
            v = mv.mvGetV();
        }
        if(strcmp(GUIalgorithm, "Logarithmic")==0){
            mv.mvLogarithmicSearch(GUIp, temp);
            u = mv.mvGetU();
            v = mv.mvGetV();
        }
//        mv.mvLogarithmicSearch(100, temp);
        IplImage* img0, * img1;
        CvFont    font;
        char      label[20];

        img0 = (IplImage*) param;
        img1 = cvCloneImage(img0);
        
        cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .8, .8, 0, 1, 8);

        Mat macroBlock = mv.retrievePredictedBlock();
        Mat outline = frameStages[0][4].clone();
        /* read pixel */
        //ptr = cvPtr2D(img1, y, x, NULL);
        
        /*
         * display the BGR value
         */


        
        if (event == CV_EVENT_LBUTTONDOWN)
        {
            
            sprintf(label, "MV(%i, %i)", u,v);

            rectangle(outline, Point(mx+u,my+v), Point(mx+u+GUImbsize, my+v+GUImbsize), CV_RGB(0,255,255), .5, 8, 0);
            cvRectangle(img1, cvPoint(mx-10, my - 25), cvPoint(mx +60, my - 5), CV_RGB(0,0,0), CV_FILLED, 8, 0);
            cvRectangle(img1, cvPoint(mx, my), cvPoint(mx+GUImbsize, my+GUImbsize), CV_RGB(0,255,255), .5, 8, 0);
            cvPutText(img1,label,cvPoint(mx-11, my-12),&font,CV_RGB(255,255,255));             
            cvShowImage("Frame Stages Demo", img1);
            imshow("Best Matched Macroblock", outline);
        }
    }
    
}

double H264::calculatePSNR(const Mat& m1, const Mat& m2)
{           
    Mat tmp;
    absdiff( m1.reshape(1), m2.reshape(1), tmp);
    multiply(tmp, tmp, tmp);
    
    double MSE =  1.0/(tmp.cols * tmp.rows) * sum(tmp)[0];
    
    return 20 * log10(255.0 / sqrt(MSE));     
}


//destructor function
H264::~H264(){}
