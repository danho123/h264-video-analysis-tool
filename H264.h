/*****************************************
 CMPT 365: Summer 2011
 Final Project: H261/H264 Encoder and Decoder
 Created By: Dan Ho and Kenneth Wong
 Professor: Dr. Ze-Nian Li
 Date: July 30th 2011
 ******************************************/
#include "MotionVector.h"
class H264 {

public:
	//constructors
    H264();
    H264(char* videoFile);
    
    //Setup functions
    void createFrameSequence();
    
    //Getters and Setters
    void setFrameSequence(int index, Mat matrix);
    Mat getFrameSequence(int index);
    Mat getEncodedFrameSequence(int index);
    int getFrameCount();
    int getFPS();
    int getWidth();
    int getHeight();
    int getInstructions();
    
    void setM(int newM);
    void setN(int newN);
    void setScale(int newScale);
    void setP(int newP);
    void setSearchAlgorithm(const char* newAlgorithm);
    void setCompressionType(const char* newType);
    void setHierarchical(bool boolean);
    void setMacroBlockSize(int newSize);
    void setQP(int newQP);
    void setSearchWindowSize(int newP);
    
    
    //Debug functions
    void saveFrames();
    void showVideo();
    void showStats();
    void padFrames(Mat &frame);
    char* int_to_string(int value, char* result, int base);
    
    //Encoding Component
    void encodeVideo();
    void encodePFrame(Mat previousReferenceFrame, Mat &pFrame);
    void encodeBFrame(Mat previousReferenceFrame, Mat &targetFrame, Mat futureReferenceFrame);
       
    
    //DCT Component
    void dctCompression(Mat &iFrame);
    void chromaSample(Mat &iFrame);
    
    //tested OK
    void dct_1D_1(int (&matrix)[8][8]); //Columns
    void dct_1D_2(double src[8][8], int (&dst)[8][8]); //Rows
    
    //tested - OK
    void quantization(int (&matrix)[8][8], int qMatrix[8][8]);
    void dequantization(int (&matrix)[8][8], int qMatrix[8][8]);
    
    
    //tested - OK
    void idct_1D_1(int (&matrix)[8][8]);
    void idct_1D_2(double src[8][8], int (&dst)[8][8]);
    
    //Integer Transform Component
    void integerTransform(Mat &frame);
    void fIntegerTransform(float (&matrix)[4][4]);
    void iIntegerTransform(float (&matrix)[4][4]);
    void integerQuantization(float (&matrix)[4][4]);
    void integerDequantization(float (&matrix)[4][4]);

    
	//General Functions
    Mat createMatFromArray(int arr[8][8]);
    void showPixels(Mat image, int channel);
    void showMatrix(int arr[8][8]);
    int round(double x);
    double calculatePSNR(const Mat &original, const Mat &compressed);
    
    //GUI
    void GUI();
    void stageGUI();
    
    //destructor
    ~H264(); 


private:
    
    //video & frame sequence
    CvCapture* capture;
    Mat*       frameSequence;
    Mat*       encodedFrameSequence;
    
    MotionVector* mvs;
    //video information
    int       fps;
    char*     videoName;
    int       frameCount;
    int       width;
    int       height; 
    int       instructionsExecuted;
    
    //compression variables
    int       M; //interval between P-frames and preceding I- or P- frames
    int       N; //interval between consecutive I-frames
    int       scale;
    int       p;
    int       mbsize;
    int       qp;
    const char*    compressionType;
    const char*    searchAlgorithm;
    bool    hierarchical;
    
    friend class  MainWindow;
};
