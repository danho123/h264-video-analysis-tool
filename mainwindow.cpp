/*****************************************
 CMPT 365: Summer 2011
 Final Project: H261/H264 Encoder and Decoder
 Created By: Dan Ho and Kenneth Wong
 Professor: Dr. Ze-Nian Li
 Date: July 30th 2011
 ******************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "time.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoLoaded = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::menuImportHandler(){
    QString qPath;
    char stats[100];
    qPath = QFileDialog::getOpenFileName(
                                         this,
                                         "Choose a file to open",
                                         QString::null,
                                         QString::null);
    
    const char* temp = qPath.toLatin1(); //convert qString to const_char
    char *path = const_cast<char*> ( temp ); //convert const_char to char
    
    if(qPath != NULL){
        videofile = new H264(path);
        
        videofile->createFrameSequence();
        ui->pathLabel->setText(qPath);
        
        sprintf(stats, "Video Details\nResolution: %dx%d\nTotal Frames: %d\nFPS: %d\n------------------------------", videofile->getWidth(), videofile->getHeight(), videofile->getFrameCount(), videofile->getFPS());
        ui->console->appendPlainText(stats);
        ui->console->appendPlainText("Video loaded successfully!");
        videoLoaded = true;
    };
}

void MainWindow::menuSaveHandler(){
    videofile->saveFrames();
    ui->console->appendPlainText("Encoded Video has been saved");
}

void MainWindow::menuResetHandler(){
    if (videoLoaded == true){
        delete videofile;
        ui->pathLabel->setText("Import an AVI file first.");
        ui->console->appendPlainText("Program has been reset");
        videoLoaded = false;
        ui->progressBar->setValue(0);
    }
}

void MainWindow::buttonCompareHandler(){
    if(videoLoaded){
        videofile->GUI();
    }
    else{
        ui->console->appendPlainText("EXCEPTION: Video is not loaded");
    }
    
}
void MainWindow::buttonPlayHandler(){
    if(videoLoaded){
        videofile->showVideo();
    }
    else{
        ui->console->appendPlainText("EXCEPTION: Video is not loaded");
    }
}
void MainWindow::buttonEncodeHandler(){
    if(videoLoaded){
        videofile->setM(ui->mSpinBox->value());
        videofile->setN(ui->nSpinBox->value());
        videofile->setScale(ui->qualitySpinBox->value());
        videofile->setMacroBlockSize(ui->mbSpinBox->value());
        videofile->setHierarchical(ui->hierarchicalCheckBox->isChecked());
        videofile->setQP(ui->qpSpinBox->value());
        videofile->setSearchWindowSize(ui->swSpinBox->value());
        ui->progressBar->setRange(0, videofile->frameCount);
        const char* temp;
        if(ui->h261RadioBox->isChecked()){
            temp = "H261";
        }
        else{
            temp = "H264";
        }
        videofile->setCompressionType(temp);

        if(ui->sequentialRadioBox->isChecked()){
            temp = "Sequential";
        }
        else{
            temp = "Logarithmic";
        }
        videofile->setSearchAlgorithm(temp);

        clock_t t1, t2;
        char time[70];
        if(ui->h261RadioBox->isChecked()){
            t1 = clock();
            encodeVideo();
            t2 = clock();    
            
            float diff = ((float)t2 - (float)t1) / 1000000.0F;
            sprintf(time, "Total Compression time: %f seconds\nTotal Comparisons: %d", diff, videofile->getInstructions());

            ui->console->appendPlainText("Finished H261 encoding.");
            ui->console->appendPlainText(time);
        }
        
        if(ui->h264RadioBox->isChecked()){
            t1 = clock();
            encodeVideo();
            t2 = clock();    
            float diff = ((float)t2 - (float)t1) / 1000000.0F;
            sprintf(time, "Total Compression time: %f\nTotal Comparisons: %d", diff, videofile->getInstructions());

            ui->console->appendPlainText("Finished H264 encoding.");  
            ui->console->appendPlainText(time);
        }
    }
    else{
        ui->console->appendPlainText("EXCEPTION: Video is not loaded");
    }
}
void MainWindow::buttonProcessHandler(){
    if(videoLoaded){    
        videofile->setM(ui->mSpinBox->value());
        videofile->setN(ui->nSpinBox->value());
        videofile->setScale(ui->qualitySpinBox->value());
        videofile->setMacroBlockSize(ui->mbSpinBox->value());
        videofile->setHierarchical(ui->hierarchicalCheckBox->isChecked());
        videofile->setQP(ui->qpSpinBox->value());
        videofile->setSearchWindowSize(ui->swSpinBox->value());
        
        const char* temp;
        if(ui->h261RadioBox->isChecked()){
            temp = "H261";
        }
        else{
            temp = "H264";
        }
        videofile->setCompressionType(temp);
        
        if(ui->sequentialRadioBox->isChecked()){
            temp = "Sequential";
        }
        else{
            temp = "Logarithmic";
        }
        videofile->setSearchAlgorithm(temp);
        if(videofile->N + videofile->M >= videofile->frameCount-2){
            ui->console->appendPlainText("EXCEPTION: The selected M and N parameters exceed the maximum number of frames.");  
        }
        else{
            videofile->stageGUI();
        }
    }
    else{
        ui->console->appendPlainText("EXCEPTION: Video is not loaded");
    }
}

void MainWindow::h261RadioBoxHandler(){
    ui->mSpinBox->setEnabled(false);
    ui->nSpinBox->setValue(3);
}

void MainWindow::h264RadioBoxHandler(){


    ui->mSpinBox->setEnabled(true);
    ui->mSpinBox->setValue(3);
    ui->nSpinBox->setValue(9);
}

void MainWindow::encodeVideo(){
    char frameNumber[50];
    videofile->instructionsExecuted = 0;
    ui->encodeButton->setEnabled(false);
    ui->processButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->compareButton->setEnabled(false);
    
    //H261
    if((strcmp(videofile->compressionType, "H261") == 0)){
        for(int i = 0; i < videofile->frameCount; i++){
            qApp->processEvents();
            ui->progressBar->setValue(1+i);
            if(i%(videofile->N+1) == 0){
                //Convert to RGB --> YUV
                cvtColor(videofile->encodedFrameSequence[i], videofile->encodedFrameSequence[i], CV_BGR2YUV);
                //Perform Chromasampling on the whole image
                videofile->chromaSample(videofile->encodedFrameSequence[i]);
                videofile->dctCompression(videofile->encodedFrameSequence[i]);
                //Convert to YUV -> RGB
                cvtColor(videofile->encodedFrameSequence[i], videofile->encodedFrameSequence[i], CV_YUV2BGR);
                printf("I%i\n", i);
                sprintf(frameNumber, "Compressing Frame: I-%d", i);
                ui->console->appendPlainText(frameNumber);
                
            }
            else{
                videofile->encodePFrame(videofile->encodedFrameSequence[i-1], videofile->encodedFrameSequence[i]);
                printf("P%i\n", i);
                sprintf(frameNumber, "Compressing Frame: P-%d", i);
                ui->console->appendPlainText(frameNumber);
            }
        }
    }
    
    Mat currentPreviousFrame;
    Mat currentFutureFrame;
    
    //H264 -- needs change
    if((strcmp(videofile->compressionType, "H264") == 0)){
        int lastFrame;
        for(int i = 0; i < videofile->frameCount; i = i + videofile->M){
            qApp->processEvents();
            ui->progressBar->setValue(1+i);
            lastFrame = i;
            if(i%(videofile->N) == 0){
                //Convert to RGB --> YUV
                cvtColor(videofile->encodedFrameSequence[i], videofile->encodedFrameSequence[i], CV_BGR2YUV);
                //Perform Chromasampling on the whole image
                videofile->chromaSample(videofile->encodedFrameSequence[i]);
                videofile->dctCompression(videofile->encodedFrameSequence[i]);
                //Convert to YUV -> RGB
                cvtColor(videofile->encodedFrameSequence[i], videofile->encodedFrameSequence[i], CV_YUV2BGR);
                sprintf(frameNumber, "Compressing Frame: I-%d", i);
                printf("I%i\n", i);
                ui->console->appendPlainText(frameNumber);
                if(i != 0){
                    for(int j = i-videofile->M+1; j<i; j++){
                        videofile->encodeBFrame(videofile->encodedFrameSequence[i-videofile->M], videofile->encodedFrameSequence[j], videofile->encodedFrameSequence[i]);
                        sprintf(frameNumber, "Compressing Frame: B-%d", j);
                        printf("B%i\n", j);
                        ui->console->appendPlainText(frameNumber);
                    }
                }
            }
            else{
                videofile->encodePFrame(videofile->encodedFrameSequence[i-videofile->M], videofile->encodedFrameSequence[i]);
                sprintf(frameNumber, "Compressing Frame: P-%d", i);
                printf("P%i\n", i);
                ui->console->appendPlainText(frameNumber);
                for(int j = i-videofile->M+1; j<i; j++){
                    videofile->encodeBFrame(videofile->encodedFrameSequence[i-videofile->M], videofile->encodedFrameSequence[j], videofile->encodedFrameSequence[i]);
                    sprintf(frameNumber, "Compressing Frame: B-%d", j);
                    printf("B%i\n", j);
                    ui->console->appendPlainText(frameNumber);
                }
            }
        }
        //encode remaining frames as I frames
        for(int i = lastFrame+1; i < videofile->frameCount; i++){
            qApp->processEvents();
            ui->progressBar->setValue(1+i);
            //Convert to RGB --> YUV
            sprintf(frameNumber, "Compressing Frame: I-%d", i);
            printf("I%i\n", i);
            ui->console->appendPlainText(frameNumber);
            cvtColor(videofile->encodedFrameSequence[i], videofile->encodedFrameSequence[i], CV_BGR2YUV);
            //Perform Chromasampling on the whole image
            videofile->chromaSample(videofile->encodedFrameSequence[i]);
            videofile->dctCompression(videofile->encodedFrameSequence[i]);
            //Convert to YUV -> RGB
            cvtColor(videofile->encodedFrameSequence[i], videofile->encodedFrameSequence[i], CV_YUV2BGR);
            
        }

    }
    ui->encodeButton->setEnabled(true);
    ui->processButton->setEnabled(true);
    ui->playButton->setEnabled(true);
    ui->compareButton->setEnabled(true);
}
