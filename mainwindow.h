/*****************************************
 CMPT 365: Summer 2011
 Final Project: H261/H264 Encoder and Decoder
 Created By: Dan Ho and Kenneth Wong
 Professor: Dr. Ze-Nian Li
 Date: July 30th 2011
 ******************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "H264.h"
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    H264* videofile;
    QFileDialog* filedialog;
    bool videoLoaded;
    ~MainWindow();

public slots:
    void buttonEncodeHandler();
    void buttonCompareHandler();
    void buttonPlayHandler();
    void buttonProcessHandler();
    
    void menuImportHandler();
    void menuSaveHandler();
    void menuResetHandler();
    
    void h261RadioBoxHandler();
    void h264RadioBoxHandler();
    
    void encodeVideo();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
