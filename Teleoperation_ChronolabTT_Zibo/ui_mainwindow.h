/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_10;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *verticalLayout_11;
    QTableWidget *tableWidget_muTweez;
    QLabel *label_muTweezConnected;
    QPushButton *pushButton_connectmuTweezer;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_8;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *tableWidget_optitrack;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_6;
    QPushButton *pushButton_connectOptitrack;
    QPushButton *pushButton_optiCalib;
    QProgressBar *progressBar_optiCalib;
    QCheckBox *checkBox_optiTip;
    QLabel *label_optitrackConnected;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout;
    QLabel *label_smaractPositionLabel;
    QSlider *horizontalSlider_smaract;
    QComboBox *comboBox_smaract;
    QVBoxLayout *verticalLayout_3;
    QDial *dial_smaract;
    QLabel *label_smaractSpeedLabel;
    QLabel *label_2;
    QLabel *label;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *checkBox_smaractSpeedControl;
    QGridLayout *gridLayout_12;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_16;
    QLabel *label_5;
    QLineEdit *lineEdit_smaractDebugPos;
    QLabel *label_6;
    QLineEdit *lineEdit_smaractDebugSpeed;
    QVBoxLayout *verticalLayout_17;
    QLabel *label_8;
    QLineEdit *lineEdit_smaractDebugTPos;
    QLabel *label_9;
    QLineEdit *lineEdit_smaractDebugTSpeed;
    QVBoxLayout *verticalLayout_18;
    QLabel *label_7;
    QVBoxLayout *verticalLayout_19;
    QPushButton *pushButton_smaractRAZ;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_10;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_9;
    QVBoxLayout *verticalLayout_8;
    QLineEdit *lineEdit_TTdata;
    QPushButton *pushButton_TTSend;
    QSpacerItem *verticalSpacer_4;
    QPushButton *Couplage_TTToMuT;
    QLineEdit *lineEdit_testZibo;
    QLineEdit *lineEdit_testZibo2;
    QLineEdit *lineEdit_testZibo3;
    QPushButton *Couplage_OptitrackToSmaract;
    QLineEdit *lineEdit_testZibo4;
    QLineEdit *lineEdit_testZibo5;
    QLineEdit *lineEdit_testZibo6;
    QGroupBox *groupBox_10;
    QGridLayout *gridLayout_11;
    QVBoxLayout *verticalLayout_12;
    QLineEdit *lineEdit_muTweezData;
    QVBoxLayout *verticalLayout_13;
    QPushButton *pushButton_sendmuT;
    QSpacerItem *verticalSpacer_5;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_7;
    QPushButton *pushButton_connectTT;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QComboBox *comboBox_TTCOM;
    QLabel *label_TTread;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget_smaract;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_smaractLoupiote;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_AU;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1328, 918);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox_9 = new QGroupBox(centralWidget);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        gridLayout_10 = new QGridLayout(groupBox_9);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        tableWidget_muTweez = new QTableWidget(groupBox_9);
        if (tableWidget_muTweez->columnCount() < 1)
            tableWidget_muTweez->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_muTweez->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (tableWidget_muTweez->rowCount() < 3)
            tableWidget_muTweez->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_muTweez->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_muTweez->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_muTweez->setVerticalHeaderItem(2, __qtablewidgetitem3);
        tableWidget_muTweez->setObjectName(QStringLiteral("tableWidget_muTweez"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget_muTweez->sizePolicy().hasHeightForWidth());
        tableWidget_muTweez->setSizePolicy(sizePolicy);

        verticalLayout_11->addWidget(tableWidget_muTweez);


        verticalLayout_10->addLayout(verticalLayout_11);

        label_muTweezConnected = new QLabel(groupBox_9);
        label_muTweezConnected->setObjectName(QStringLiteral("label_muTweezConnected"));

        verticalLayout_10->addWidget(label_muTweezConnected);


        verticalLayout_9->addLayout(verticalLayout_10);

        pushButton_connectmuTweezer = new QPushButton(groupBox_9);
        pushButton_connectmuTweezer->setObjectName(QStringLiteral("pushButton_connectmuTweezer"));

        verticalLayout_9->addWidget(pushButton_connectmuTweezer);


        gridLayout_10->addLayout(verticalLayout_9, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_9, 0, 3, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_8 = new QGridLayout(groupBox_2);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        tableWidget_optitrack = new QTableWidget(groupBox_2);
        if (tableWidget_optitrack->columnCount() < 3)
            tableWidget_optitrack->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_optitrack->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_optitrack->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_optitrack->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        if (tableWidget_optitrack->rowCount() < 2)
            tableWidget_optitrack->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_optitrack->setVerticalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_optitrack->setVerticalHeaderItem(1, __qtablewidgetitem8);
        tableWidget_optitrack->setObjectName(QStringLiteral("tableWidget_optitrack"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableWidget_optitrack->sizePolicy().hasHeightForWidth());
        tableWidget_optitrack->setSizePolicy(sizePolicy1);
        tableWidget_optitrack->setMinimumSize(QSize(390, 4));
        tableWidget_optitrack->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_optitrack->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout_5->addWidget(tableWidget_optitrack);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        pushButton_connectOptitrack = new QPushButton(groupBox_2);
        pushButton_connectOptitrack->setObjectName(QStringLiteral("pushButton_connectOptitrack"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_connectOptitrack->sizePolicy().hasHeightForWidth());
        pushButton_connectOptitrack->setSizePolicy(sizePolicy2);

        verticalLayout_6->addWidget(pushButton_connectOptitrack);

        pushButton_optiCalib = new QPushButton(groupBox_2);
        pushButton_optiCalib->setObjectName(QStringLiteral("pushButton_optiCalib"));

        verticalLayout_6->addWidget(pushButton_optiCalib);

        progressBar_optiCalib = new QProgressBar(groupBox_2);
        progressBar_optiCalib->setObjectName(QStringLiteral("progressBar_optiCalib"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(progressBar_optiCalib->sizePolicy().hasHeightForWidth());
        progressBar_optiCalib->setSizePolicy(sizePolicy3);
        progressBar_optiCalib->setValue(0);

        verticalLayout_6->addWidget(progressBar_optiCalib);

        checkBox_optiTip = new QCheckBox(groupBox_2);
        checkBox_optiTip->setObjectName(QStringLiteral("checkBox_optiTip"));

        verticalLayout_6->addWidget(checkBox_optiTip);


        horizontalLayout_3->addLayout(verticalLayout_6);

        label_optitrackConnected = new QLabel(groupBox_2);
        label_optitrackConnected->setObjectName(QStringLiteral("label_optitrackConnected"));

        horizontalLayout_3->addWidget(label_optitrackConnected);


        verticalLayout_5->addLayout(horizontalLayout_3);


        gridLayout_8->addLayout(verticalLayout_5, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        groupBox_6 = new QGroupBox(groupBox_3);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_smaractPositionLabel = new QLabel(groupBox_6);
        label_smaractPositionLabel->setObjectName(QStringLiteral("label_smaractPositionLabel"));

        horizontalLayout->addWidget(label_smaractPositionLabel);

        horizontalSlider_smaract = new QSlider(groupBox_6);
        horizontalSlider_smaract->setObjectName(QStringLiteral("horizontalSlider_smaract"));
        horizontalSlider_smaract->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider_smaract);


        gridLayout_5->addLayout(horizontalLayout, 1, 3, 1, 1);

        comboBox_smaract = new QComboBox(groupBox_6);
        comboBox_smaract->setObjectName(QStringLiteral("comboBox_smaract"));

        gridLayout_5->addWidget(comboBox_smaract, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        dial_smaract = new QDial(groupBox_6);
        dial_smaract->setObjectName(QStringLiteral("dial_smaract"));
        dial_smaract->setMinimum(-2000000);
        dial_smaract->setMaximum(2000000);

        verticalLayout_3->addWidget(dial_smaract);

        label_smaractSpeedLabel = new QLabel(groupBox_6);
        label_smaractSpeedLabel->setObjectName(QStringLiteral("label_smaractSpeedLabel"));
        label_smaractSpeedLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_smaractSpeedLabel);


        gridLayout_5->addLayout(verticalLayout_3, 1, 4, 1, 1);

        label_2 = new QLabel(groupBox_6);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_2, 0, 4, 1, 1);

        label = new QLabel(groupBox_6);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label, 0, 3, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        checkBox_smaractSpeedControl = new QCheckBox(groupBox_6);
        checkBox_smaractSpeedControl->setObjectName(QStringLiteral("checkBox_smaractSpeedControl"));

        verticalLayout_4->addWidget(checkBox_smaractSpeedControl);


        gridLayout_5->addLayout(verticalLayout_4, 1, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setSpacing(6);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        label_4 = new QLabel(groupBox_6);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_15->addWidget(label_4);


        gridLayout_12->addLayout(verticalLayout_15, 0, 0, 1, 1);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_16->addWidget(label_5);

        lineEdit_smaractDebugPos = new QLineEdit(groupBox_6);
        lineEdit_smaractDebugPos->setObjectName(QStringLiteral("lineEdit_smaractDebugPos"));

        verticalLayout_16->addWidget(lineEdit_smaractDebugPos);

        label_6 = new QLabel(groupBox_6);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_16->addWidget(label_6);

        lineEdit_smaractDebugSpeed = new QLineEdit(groupBox_6);
        lineEdit_smaractDebugSpeed->setObjectName(QStringLiteral("lineEdit_smaractDebugSpeed"));

        verticalLayout_16->addWidget(lineEdit_smaractDebugSpeed);


        gridLayout_12->addLayout(verticalLayout_16, 2, 0, 1, 1);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        label_8 = new QLabel(groupBox_6);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_17->addWidget(label_8);

        lineEdit_smaractDebugTPos = new QLineEdit(groupBox_6);
        lineEdit_smaractDebugTPos->setObjectName(QStringLiteral("lineEdit_smaractDebugTPos"));

        verticalLayout_17->addWidget(lineEdit_smaractDebugTPos);

        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout_17->addWidget(label_9);

        lineEdit_smaractDebugTSpeed = new QLineEdit(groupBox_6);
        lineEdit_smaractDebugTSpeed->setObjectName(QStringLiteral("lineEdit_smaractDebugTSpeed"));

        verticalLayout_17->addWidget(lineEdit_smaractDebugTSpeed);


        gridLayout_12->addLayout(verticalLayout_17, 2, 2, 1, 1);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        label_7 = new QLabel(groupBox_6);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_18->addWidget(label_7);


        gridLayout_12->addLayout(verticalLayout_18, 0, 2, 1, 1);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        pushButton_smaractRAZ = new QPushButton(groupBox_6);
        pushButton_smaractRAZ->setObjectName(QStringLiteral("pushButton_smaractRAZ"));

        verticalLayout_19->addWidget(pushButton_smaractRAZ);


        gridLayout_12->addLayout(verticalLayout_19, 2, 3, 1, 1);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        label_10 = new QLabel(groupBox_6);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_20->addWidget(label_10);


        gridLayout_12->addLayout(verticalLayout_20, 0, 3, 1, 1);


        gridLayout_6->addLayout(gridLayout_12, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_6, 0, 0, 1, 1);

        groupBox_8 = new QGroupBox(groupBox_3);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        gridLayout_9 = new QGridLayout(groupBox_8);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        lineEdit_TTdata = new QLineEdit(groupBox_8);
        lineEdit_TTdata->setObjectName(QStringLiteral("lineEdit_TTdata"));

        verticalLayout_8->addWidget(lineEdit_TTdata);

        pushButton_TTSend = new QPushButton(groupBox_8);
        pushButton_TTSend->setObjectName(QStringLiteral("pushButton_TTSend"));

        verticalLayout_8->addWidget(pushButton_TTSend);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_4);

        Couplage_TTToMuT = new QPushButton(groupBox_8);
        Couplage_TTToMuT->setObjectName(QStringLiteral("Couplage_TTToMuT"));

        verticalLayout_8->addWidget(Couplage_TTToMuT);

        lineEdit_testZibo = new QLineEdit(groupBox_8);
        lineEdit_testZibo->setObjectName(QStringLiteral("lineEdit_testZibo"));

        verticalLayout_8->addWidget(lineEdit_testZibo);

        lineEdit_testZibo2 = new QLineEdit(groupBox_8);
        lineEdit_testZibo2->setObjectName(QStringLiteral("lineEdit_testZibo2"));

        verticalLayout_8->addWidget(lineEdit_testZibo2);

        lineEdit_testZibo3 = new QLineEdit(groupBox_8);
        lineEdit_testZibo3->setObjectName(QStringLiteral("lineEdit_testZibo3"));

        verticalLayout_8->addWidget(lineEdit_testZibo3);

        Couplage_OptitrackToSmaract = new QPushButton(groupBox_8);
        Couplage_OptitrackToSmaract->setObjectName(QStringLiteral("Couplage_OptitrackToSmaract"));

        verticalLayout_8->addWidget(Couplage_OptitrackToSmaract);

        lineEdit_testZibo4 = new QLineEdit(groupBox_8);
        lineEdit_testZibo4->setObjectName(QStringLiteral("lineEdit_testZibo4"));

        verticalLayout_8->addWidget(lineEdit_testZibo4);

        lineEdit_testZibo5 = new QLineEdit(groupBox_8);
        lineEdit_testZibo5->setObjectName(QStringLiteral("lineEdit_testZibo5"));

        verticalLayout_8->addWidget(lineEdit_testZibo5);

        lineEdit_testZibo6 = new QLineEdit(groupBox_8);
        lineEdit_testZibo6->setObjectName(QStringLiteral("lineEdit_testZibo6"));

        verticalLayout_8->addWidget(lineEdit_testZibo6);


        gridLayout_9->addLayout(verticalLayout_8, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_8, 0, 1, 1, 1);

        groupBox_10 = new QGroupBox(groupBox_3);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        gridLayout_11 = new QGridLayout(groupBox_10);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        lineEdit_muTweezData = new QLineEdit(groupBox_10);
        lineEdit_muTweezData->setObjectName(QStringLiteral("lineEdit_muTweezData"));

        verticalLayout_12->addWidget(lineEdit_muTweezData);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        pushButton_sendmuT = new QPushButton(groupBox_10);
        pushButton_sendmuT->setObjectName(QStringLiteral("pushButton_sendmuT"));

        verticalLayout_13->addWidget(pushButton_sendmuT);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_5);


        verticalLayout_12->addLayout(verticalLayout_13);


        gridLayout_11->addLayout(verticalLayout_12, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_10, 0, 2, 1, 1);


        gridLayout->addWidget(groupBox_3, 1, 0, 1, 4);

        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        gridLayout_7 = new QGridLayout(groupBox_7);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        pushButton_connectTT = new QPushButton(groupBox_7);
        pushButton_connectTT->setObjectName(QStringLiteral("pushButton_connectTT"));

        gridLayout_7->addWidget(pushButton_connectTT, 2, 0, 1, 1);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_3 = new QLabel(groupBox_7);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy4);
        label_3->setMinimumSize(QSize(50, 0));

        horizontalLayout_2->addWidget(label_3);

        comboBox_TTCOM = new QComboBox(groupBox_7);
        comboBox_TTCOM->setObjectName(QStringLiteral("comboBox_TTCOM"));
        sizePolicy2.setHeightForWidth(comboBox_TTCOM->sizePolicy().hasHeightForWidth());
        comboBox_TTCOM->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(comboBox_TTCOM);


        verticalLayout_7->addLayout(horizontalLayout_2);

        label_TTread = new QLabel(groupBox_7);
        label_TTread->setObjectName(QStringLiteral("label_TTread"));

        verticalLayout_7->addWidget(label_TTread);


        gridLayout_7->addLayout(verticalLayout_7, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_7, 0, 2, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidget_smaract = new QTableWidget(groupBox_4);
        if (tableWidget_smaract->columnCount() < 2)
            tableWidget_smaract->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_smaract->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_smaract->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        if (tableWidget_smaract->rowCount() < 6)
            tableWidget_smaract->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_smaract->setVerticalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_smaract->setVerticalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_smaract->setVerticalHeaderItem(2, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_smaract->setVerticalHeaderItem(3, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_smaract->setVerticalHeaderItem(4, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_smaract->setVerticalHeaderItem(5, __qtablewidgetitem16);
        tableWidget_smaract->setObjectName(QStringLiteral("tableWidget_smaract"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Ignored);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(tableWidget_smaract->sizePolicy().hasHeightForWidth());
        tableWidget_smaract->setSizePolicy(sizePolicy5);
        tableWidget_smaract->setMinimumSize(QSize(290, 210));
        tableWidget_smaract->setAutoScroll(false);

        verticalLayout->addWidget(tableWidget_smaract);


        gridLayout_2->addLayout(verticalLayout, 1, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_smaractLoupiote = new QLabel(groupBox_4);
        label_smaractLoupiote->setObjectName(QStringLiteral("label_smaractLoupiote"));

        verticalLayout_2->addWidget(label_smaractLoupiote);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer_2);

        pushButton_AU = new QPushButton(groupBox_4);
        pushButton_AU->setObjectName(QStringLiteral("pushButton_AU"));
        sizePolicy3.setHeightForWidth(pushButton_AU->sizePolicy().hasHeightForWidth());
        pushButton_AU->setSizePolicy(sizePolicy3);
        pushButton_AU->setMinimumSize(QSize(0, 120));
        QFont font;
        font.setPointSize(36);
        font.setBold(true);
        font.setWeight(75);
        pushButton_AU->setFont(font);

        verticalLayout_2->addWidget(pushButton_AU);


        gridLayout_2->addLayout(verticalLayout_2, 1, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_4, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1328, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "muTweezer", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_muTweez->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Values", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_muTweez->verticalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Y1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_muTweez->verticalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Y2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_muTweez->verticalHeaderItem(2);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "F", Q_NULLPTR));
        label_muTweezConnected->setText(QApplication::translate("MainWindow", "-", Q_NULLPTR));
        pushButton_connectmuTweezer->setText(QApplication::translate("MainWindow", "CONNECT", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Optitrack", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_optitrack->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "x", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_optitrack->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "y", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_optitrack->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "z", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_optitrack->verticalHeaderItem(0);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Positions", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_optitrack->verticalHeaderItem(1);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "Orientations", Q_NULLPTR));
        pushButton_connectOptitrack->setText(QApplication::translate("MainWindow", "CONNECT", Q_NULLPTR));
        pushButton_optiCalib->setText(QApplication::translate("MainWindow", "Calibrate", Q_NULLPTR));
        checkBox_optiTip->setText(QApplication::translate("MainWindow", "Tip transformation", Q_NULLPTR));
        label_optitrackConnected->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Debug", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Smaract", Q_NULLPTR));
        label_smaractPositionLabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        comboBox_smaract->clear();
        comboBox_smaract->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Positionner 1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Positionner 2", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Positionner 3", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Positionner 4", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Positionner 5", Q_NULLPTR)
        );
        label_smaractSpeedLabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Speed", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Position", Q_NULLPTR));
        checkBox_smaractSpeedControl->setText(QApplication::translate("MainWindow", "Speed control", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Valeurs Courantes", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Position", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Speed", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Position", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "Speed", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Target", Q_NULLPTR));
        pushButton_smaractRAZ->setText(QApplication::translate("MainWindow", "RAZ", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Remise \303\240 Z\303\251ros de la Vitesse", Q_NULLPTR));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Teletweezer", Q_NULLPTR));
        pushButton_TTSend->setText(QApplication::translate("MainWindow", "Send", Q_NULLPTR));
        Couplage_TTToMuT->setText(QApplication::translate("MainWindow", "Couplage_TTToMuT", Q_NULLPTR));
        Couplage_OptitrackToSmaract->setText(QApplication::translate("MainWindow", "Couplage_OptitrackToSmaract", Q_NULLPTR));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "muTweezer", Q_NULLPTR));
        lineEdit_muTweezData->setText(QApplication::translate("MainWindow", "GRIPPER:STATE", Q_NULLPTR));
        pushButton_sendmuT->setText(QApplication::translate("MainWindow", "Send", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Teletweezer", Q_NULLPTR));
        pushButton_connectTT->setText(QApplication::translate("MainWindow", "CONNECT", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "COM", Q_NULLPTR));
        label_TTread->setText(QApplication::translate("MainWindow", "-", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Chronolab", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Smaract", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_smaract->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "Value", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_smaract->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "Sensor", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_smaract->verticalHeaderItem(0);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "N\302\260 device", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_smaract->verticalHeaderItem(1);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "Positionner 1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_smaract->verticalHeaderItem(2);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "Positionner 2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_smaract->verticalHeaderItem(3);
        ___qtablewidgetitem14->setText(QApplication::translate("MainWindow", "Positionner 3", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_smaract->verticalHeaderItem(4);
        ___qtablewidgetitem15->setText(QApplication::translate("MainWindow", "Positionner 4", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_smaract->verticalHeaderItem(5);
        ___qtablewidgetitem16->setText(QApplication::translate("MainWindow", "Positionner 5", Q_NULLPTR));
        label_smaractLoupiote->setText(QString());
        pushButton_AU->setText(QApplication::translate("MainWindow", "A-U", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
