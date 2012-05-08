/*****************************************
 CMPT 365: Summer 2011
 Final Project: H261/H264 Encoder and Decoder
 Created By: Dan Ho and Kenneth Wong
 Professor: Dr. Ze-Nian Li
 Date: July 30th 2011
 ******************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QProgressBar>
QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionContact;
    QAction *actionImport;
    QAction *actionSave;
    QAction *actionReset;
    QAction *actionReset_2;
    QWidget *centralWidget;
    QPlainTextEdit *console;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QRadioButton *sequentialRadioBox;
    QRadioButton *logarithmicRadioBox;
    QCheckBox *hierarchicalCheckBox;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QRadioButton *h261RadioBox;
    QRadioButton *h264RadioBox;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label_5;
    QVBoxLayout *verticalLayout;
    QSpinBox *qpSpinBox;
    QSpinBox *qualitySpinBox;
    QSpinBox *swSpinBox;
    QSpinBox *mbSpinBox;
    QSpinBox *mSpinBox;
    QSpinBox *nSpinBox;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_6;
    QLabel *title;
    QLabel *label;
    QProgressBar *progressBar;
    QVBoxLayout *verticalLayout_2;
    QLabel *pathLabel;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *encodeButton;
    QPushButton *playButton;
    QPushButton *compareButton;
    QPushButton *processButton;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(525, 505);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionContact"));
        actionContact = new QAction(MainWindow);
        actionContact->setObjectName(QString::fromUtf8("actionContact"));
        actionImport = new QAction(MainWindow);
        actionImport->setObjectName(QString::fromUtf8("actionImport"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        actionReset_2 = new QAction(MainWindow);
        actionReset_2->setObjectName(QString::fromUtf8("actionReset_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        console = new QPlainTextEdit(centralWidget);
        console->setObjectName(QString::fromUtf8("console"));
        console->setGeometry(QRect(10, 358, 501, 91));
        console->setReadOnly(true);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 130, 223, 221));
        gridLayout_4 = new QGridLayout(layoutWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        sequentialRadioBox = new QRadioButton(groupBox_2);
        sequentialRadioBox->setObjectName(QString::fromUtf8("sequentialRadioBox"));
        sequentialRadioBox->setChecked(true);

        gridLayout_2->addWidget(sequentialRadioBox, 1, 0, 1, 1);

        logarithmicRadioBox = new QRadioButton(groupBox_2);
        logarithmicRadioBox->setObjectName(QString::fromUtf8("logarithmicRadioBox"));

        gridLayout_2->addWidget(logarithmicRadioBox, 2, 0, 1, 1);

        hierarchicalCheckBox = new QCheckBox(groupBox_2);
        hierarchicalCheckBox->setObjectName(QString::fromUtf8("hierarchicalCheckBox"));

        gridLayout_2->addWidget(hierarchicalCheckBox, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        h261RadioBox = new QRadioButton(groupBox);
        h261RadioBox->setObjectName(QString::fromUtf8("h261RadioBox"));
        h261RadioBox->setChecked(true);

        gridLayout->addWidget(h261RadioBox, 0, 0, 1, 1);

        h264RadioBox = new QRadioButton(groupBox);
        h264RadioBox->setObjectName(QString::fromUtf8("h264RadioBox"));

        gridLayout->addWidget(h264RadioBox, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(270, 130, 232, 218));
        gridLayout_3 = new QGridLayout(groupBox_4);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_3->addWidget(label_7);

        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_3->addWidget(label_2);

        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_3->addWidget(label_6);

        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_3->addWidget(label_4);

        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_3->addWidget(label_5);


        horizontalLayout_2->addLayout(verticalLayout_3);
        
        progressBar = new QProgressBar(MainWindow);
        progressBar->setRange(0, 100);

        
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        qpSpinBox = new QSpinBox(groupBox_4);
        qpSpinBox->setObjectName(QString::fromUtf8("qpSpinBox"));
        qpSpinBox->setMaximum(51);

        verticalLayout->addWidget(qpSpinBox);

        qualitySpinBox = new QSpinBox(groupBox_4);
        qualitySpinBox->setObjectName(QString::fromUtf8("qualitySpinBox"));
        qualitySpinBox->setMaximum(31);
        qualitySpinBox->setMinimum(1);
        qualitySpinBox->setValue(1);

        verticalLayout->addWidget(qualitySpinBox);

        swSpinBox = new QSpinBox(groupBox_4);
        swSpinBox->setObjectName(QString::fromUtf8("swSpinBox"));
        swSpinBox->setValue(15);
        swSpinBox->setMaximum(500);

        verticalLayout->addWidget(swSpinBox);

        mbSpinBox = new QSpinBox(groupBox_4);
        mbSpinBox->setObjectName(QString::fromUtf8("mbSpinBox"));
        mbSpinBox->setValue(16);
        mbSpinBox->setMinimum(4);
        mbSpinBox->setSingleStep(4);

        verticalLayout->addWidget(mbSpinBox);

        mSpinBox = new QSpinBox(groupBox_4);
        mSpinBox->setObjectName(QString::fromUtf8("mSpinBox"));
        mSpinBox->setValue(0);
        mSpinBox->setMaximum(999);

        verticalLayout->addWidget(mSpinBox);

        nSpinBox = new QSpinBox(groupBox_4);
        nSpinBox->setObjectName(QString::fromUtf8("nSpinBox"));
        nSpinBox->setValue(3);
        mSpinBox->setMaximum(999);
        
        verticalLayout->addWidget(nSpinBox);


        horizontalLayout_2->addLayout(verticalLayout);


        gridLayout_3->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 0, 507, 121));
        verticalLayout_4 = new QVBoxLayout(layoutWidget1);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        title = new QLabel(layoutWidget1);
        title->setObjectName(QString::fromUtf8("title"));
        QFont font;
        font.setPointSize(19);
        font.setBold(true);
        font.setWeight(75);
        title->setFont(font);
        title->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(title);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label);


        verticalLayout_4->addLayout(verticalLayout_6);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pathLabel = new QLabel(layoutWidget1);
        pathLabel->setObjectName(QString::fromUtf8("pathLabel"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        pathLabel->setFont(font1);
        pathLabel->setFrameShape(QFrame::StyledPanel);

        verticalLayout_2->addWidget(pathLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        encodeButton = new QPushButton(layoutWidget1);
        encodeButton->setObjectName(QString::fromUtf8("encodeButton"));

        horizontalLayout_3->addWidget(encodeButton);

        playButton = new QPushButton(layoutWidget1);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        QFont font2;
        font2.setStrikeOut(false);
        font2.setKerning(true);
        playButton->setFont(font2);
        playButton->setCheckable(false);
        playButton->setAutoDefault(false);
        playButton->setFlat(false);

        horizontalLayout_3->addWidget(playButton);

        compareButton = new QPushButton(layoutWidget1);
        compareButton->setObjectName(QString::fromUtf8("compareButton"));

        horizontalLayout_3->addWidget(compareButton);

        processButton = new QPushButton(layoutWidget1);
        processButton->setObjectName(QString::fromUtf8("processButton"));

        horizontalLayout_3->addWidget(processButton);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_4->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        groupBox_4->raise();
        layoutWidget->raise();
        console->raise();
        layoutWidget->raise();
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 525, 22));
        progressBar->setGeometry(QRect(10,460, 501, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionImport);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionReset_2);
        //menuFile->addAction(actionContact);
        menuFile->addAction(actionQuit);


        retranslateUi(MainWindow);
        //menu items
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionImport, SIGNAL(triggered()), MainWindow, SLOT(menuImportHandler()));
        QObject::connect(actionSave, SIGNAL(triggered()), MainWindow, SLOT(menuSaveHandler()));
        QObject::connect(actionReset_2, SIGNAL(triggered()), MainWindow, SLOT(menuResetHandler()));
        
        //push buttons
        QObject::connect(encodeButton, SIGNAL(clicked()),MainWindow, SLOT(buttonEncodeHandler()));
        QObject::connect(playButton, SIGNAL(clicked()),MainWindow, SLOT(buttonPlayHandler()));
        QObject::connect(compareButton, SIGNAL(clicked()),MainWindow, SLOT(buttonCompareHandler()));
        QObject::connect(processButton, SIGNAL(clicked()),MainWindow, SLOT(buttonProcessHandler()));   
        
        //radio buttons
        QObject::connect(h261RadioBox, SIGNAL(clicked()),MainWindow, SLOT(h261RadioBoxHandler()));
        QObject::connect(h264RadioBox, SIGNAL(clicked()),MainWindow, SLOT(h264RadioBoxHandler()));        
        

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CMPT365: Final Project", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionContact->setText(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        actionImport->setText(QApplication::translate("MainWindow", "Import...", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save...", 0, QApplication::UnicodeUTF8));
        actionReset->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        actionReset_2->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Motion Vector Search Algorithm", 0, QApplication::UnicodeUTF8));
        sequentialRadioBox->setText(QApplication::translate("MainWindow", "Sequential", 0, QApplication::UnicodeUTF8));
        logarithmicRadioBox->setText(QApplication::translate("MainWindow", "Logarithmic", 0, QApplication::UnicodeUTF8));
        hierarchicalCheckBox->setText(QApplication::translate("MainWindow", "Hierarchical", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Compression Type", 0, QApplication::UnicodeUTF8));
        h261RadioBox->setText(QApplication::translate("MainWindow", "H261", 0, QApplication::UnicodeUTF8));
        h264RadioBox->setText(QApplication::translate("MainWindow", "H264", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Additional Settings", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "QP", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Scale", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "P (Search Window)", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Macroblock Size", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "M (P-Frame Interval)", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "N (I-Frame Interval)", 0, QApplication::UnicodeUTF8));
        title->setText(QApplication::translate("MainWindow", "H261/H264 Encoder & Decoder Demo", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "By: Danny Ho & Kenneth Wong", 0, QApplication::UnicodeUTF8));
        pathLabel->setText(QApplication::translate("MainWindow", "Import an AVI file first...", 0, QApplication::UnicodeUTF8));
        encodeButton->setText(QApplication::translate("MainWindow", "Encode Video", 0, QApplication::UnicodeUTF8));
        playButton->setText(QApplication::translate("MainWindow", "Play Video", 0, QApplication::UnicodeUTF8));
        compareButton->setText(QApplication::translate("MainWindow", "Compare Video", 0, QApplication::UnicodeUTF8));
        processButton->setText(QApplication::translate("MainWindow", "View Stages", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        hierarchicalCheckBox->setEnabled(false);
        actionSave->setVisible(false);
        progressBar->setTextVisible(true);
        
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
