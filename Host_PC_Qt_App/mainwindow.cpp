#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QTableWidget>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDebug>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&timerUpdateGUI, SIGNAL(timeout()), this, SLOT(updateGUI()));
    timerUpdateGUI.start(10);

    /// SMARACT GUI INIT
    //Création des cellules des tableaux de l'interface
    for(int i=0; i<ui->tableWidget_smaract->rowCount()-1; i++)
    {
        ui->tableWidget_smaract->setItem(i+1, 0, new QTableWidgetItem(""));
        ui->tableWidget_smaract->setItem(i+1, 1, new QTableWidgetItem(m_chronolab.positionerTypeToString(i)));
    }
    //ui->tableWidget_smaract -> item(0, 0)->setText(QString::number(m_chronolab.m_deviceIndex));
    /*for(int i=0 ; i<AXISNB;i++)
    {
        std::cout << m_chronolab.positionerTypeToString(i).toStdString() << std::endl;
        //ui->tableWidget_smaract -> item(i, 1)->setText(m_chronolab.positionerTypeToString(i));
    }*/

    m_chronolab.start();

    connect(ui->horizontalSlider_smaract,SIGNAL(sliderPressed()),this,SLOT(setSmaractPosition()));
    connect(ui->dial_smaract,SIGNAL(sliderPressed()),this,SLOT(setSmaractSpeed()));
    connect(ui->comboBox_smaract,SIGNAL(currentIndexChanged(int)), this, SLOT(setSmaractDebugValues()));
    connect(ui->pushButton_AU,SIGNAL(clicked()),this,SLOT(doEmergencyStop()));
    connect(ui->pushButton_smaractRAZ,SIGNAL(clicked()),this,SLOT(doControlSpeedStop()));

    connect(&m_chronolab, SIGNAL(smaractError()), this, SLOT(onSMARACTError()));

    setSmaractDebugValues();


    /// TT GUI INIT
     QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
     for(int i=0; i<list.count(); i++)
     {
         ui->comboBox_TTCOM->addItem(list[i].portName());
     }

     connect(ui->pushButton_connectTT,SIGNAL(clicked(bool)),this, SLOT(onConnectTT()));
     connect(ui->pushButton_TTSend,SIGNAL(clicked(bool)),this, SLOT(onSendDataTT()));


     /// Optitrack GUI INIT
     connect(ui->pushButton_connectOptitrack,SIGNAL(clicked(bool)),this,SLOT(onConnectOptitrack()));
     connect(ui->pushButton_optiCalib,SIGNAL(clicked(bool)),this,SLOT(onCalibrateOptitrack()));
     connect(&m_optitrack, SIGNAL(calibrating(int)), this, SLOT(optiCalibrating(int)));
     connect(ui->checkBox_optiTip,SIGNAL(stateChanged(int)),this,SLOT(onTipTransform()));

     for(int i=0; i<ui->tableWidget_optitrack->rowCount(); i++)
     {
         ui->tableWidget_optitrack->setItem(i, 0, new QTableWidgetItem(""));
         ui->tableWidget_optitrack->setItem(i, 1, new QTableWidgetItem(""));
         ui->tableWidget_optitrack->setItem(i, 2, new QTableWidgetItem(""));
     }
     ui->checkBox_optiTip->setEnabled(false);

     /// microTweez GUI INIT
     //Création des cellules des tableaux de l'interface
     for(int i=0; i<ui->tableWidget_muTweez->rowCount(); i++)
     {
         ui->tableWidget_muTweez->setItem(i, 0, new QTableWidgetItem(""));
     }

     connect(ui->pushButton_connectmuTweezer,SIGNAL(clicked(bool)),this,SLOT(onConnectMutweez()));
     connect(ui->pushButton_sendmuT,SIGNAL(clicked(bool)),this,SLOT(onSendDataMuT()));


     // Couplage Télétweezer et micro pince
     connect(ui->Couplage_TTToMuT, SIGNAL(clicked(bool)),this, SLOT(on_Couplage_TTToMuT_clicked()));

     /*
     // Couplage Optitrack et smaract
     connect(ui-> Couplage_OptitrackToSmaract, SIGNAL(clicked(bool)),this,SLOT(on_Couplage_OptitrackToSmaract_clicked()));
     */
}

void MainWindow::onSMARACTError()
{
    timerUpdateGUI.stop();
    std::cout << "SMARACT ERROR!!!" << std::endl;
}

void MainWindow::updateGUI()
{
    /// SMARACT
    int pos[AXISNB];
    double speed[AXISNB];
    double distance[AXISNB];

    QString posString = "positions ";
    QString speedString = "speeds ";
    QString distanceString = "distances ";

    for(unsigned int i=0; i<AXISNB; i++)
    {
        pos[i] = m_chronolab.getPosition(i);
        posString += QString::number(pos[i]) + " ";

        distance[i] = m_chronolab.getDistance(i);
        distanceString += QString::number(distance[i]) + " ";

        speed[i] = m_chronolab.getSpeed(i);
        speedString += QString::number(speed[i]) + " ";
    }

    if(m_chronolab.getState() == SMARACT_STATE::READY)
    {
        ui->label_smaractLoupiote->setStyleSheet("QLabel { background-color: green; }");
    }
    else
    {
        ui->label_smaractLoupiote->setStyleSheet("QLabel { background-color: red; }");
    }
    /*std::cout << posString.toStdString() << std::endl;
    std::cout << distanceString.toStdString() << std::endl;
    std::cout << speedString.toStdString() << std::endl;
    std::cout << "--------------------------------" << std::endl;
*/

    for (int i = 0; i<AXISNB;i++)
    {
        ui->tableWidget_smaract -> item(i+1, 0)->setText(QString::number(m_chronolab.getPosition(i)));
        //ui->tableWidget_smaract -> item(i, 1)->setText(m_chronolab.positionerTypeToString(i));
    }

    ui->lineEdit_smaractDebugPos->setText(QString::number(m_chronolab.getPosition(ui->comboBox_smaract->currentIndex())));
    ui->lineEdit_smaractDebugSpeed->setText(QString::number(m_chronolab.getSpeed(ui->comboBox_smaract->currentIndex())));
    ui->lineEdit_smaractDebugTPos->setText(QString::number(ui->horizontalSlider_smaract->value()));
    ui->lineEdit_smaractDebugTSpeed->setText(QString::number(ui->dial_smaract->value()));


    /// Optitrack
    if(m_optitrack.isTracked())
    {
        ui->label_optitrackConnected->setStyleSheet("QLabel { background-color: green; }");
    }
    else
    {
        ui->label_optitrackConnected->setStyleSheet("QLabel { background-color: red; }");
    }

    optitrack_data optData = m_optitrack.getData();

    if(ui->checkBox_optiTip->isChecked())
    {
        ui->tableWidget_optitrack -> item(0, 0)->setText(QString::number(optData.posxT));
        ui->tableWidget_optitrack -> item(0, 1)->setText(QString::number(optData.posyT));
        ui->tableWidget_optitrack -> item(0, 2)->setText(QString::number(optData.poszT));
    }
    else
    {
        ui->tableWidget_optitrack -> item(0, 0)->setText(QString::number(optData.posx));
        ui->tableWidget_optitrack -> item(0, 1)->setText(QString::number(optData.posy));
        ui->tableWidget_optitrack -> item(0, 2)->setText(QString::number(optData.posz));
    }
    ui->tableWidget_optitrack -> item(1, 0)->setText(QString::number(optData.yaw));
    ui->tableWidget_optitrack -> item(1, 1)->setText(QString::number(optData.pitch));
    ui->tableWidget_optitrack -> item(1, 2)->setText(QString::number(optData.roll));


    /// Teletweezer
    QStringList TTRead = m_teletweezer.getData();
    //std::cout << TTRead.size() << std::endl;
    if(TTRead.size() > 0)
    {
        QString TTData = "";
        for(int i=0; i<TTRead.size()-1; i++)
        {
            TTData += TTRead[i] + " | ";
        }
        TTData += TTRead[TTRead.size()-1];
        ui->label_TTread->setText(TTData);
    }
    else
    {
        ui->label_TTread->setText("NO DATA");
    }

    /// microTweezer
    QString MuTweezRead = m_mutweez.getData();

    ui->tableWidget_muTweez -> item(0, 0)->setText("A");
    ui->tableWidget_muTweez -> item(1, 0)->setText("B");
    ui->tableWidget_muTweez -> item(2, 0)->setText("C");

    if(MuTweezRead.size()==0)
    {
        ui->label_muTweezConnected->setText("NO DATA");
    }
    else
    {
        ui->label_muTweezConnected->setText(MuTweezRead);
    }


    qApp->processEvents();
}

void MainWindow::doEmergencyStop()
{
    m_chronolab.stop();
}

void MainWindow::doControlSpeedStop()
{
    m_chronolab.speedControlStop();
}

//SMARACT Consigne en position
void MainWindow::setSmaractPosition()
{
    int sensor = ui->comboBox_smaract->currentIndex();
    std::cout<<"sensor position "<<sensor<< std::endl;

    double speedTarget[AXISNB] = {0};
    speedTarget[sensor] = ui->dial_smaract->value();
    double positionTarget[AXISNB] = {0};
    positionTarget[sensor] = ui->horizontalSlider_smaract->value();

    /*std::cout<<"position ";
    for(int i=0; i>AXISNB;i++)
    {
        std::cout<<positionTarget[i]<<" " ;
    }
    std::cout<<std::endl;

    std::cout<<"vitesse ";
    for(int i=0; i>AXISNB;i++) std::cout<<speedTarget[i]<<" " ;
    std::cout<<std::endl;*/

    std::cout<<"position "<<positionTarget[sensor]<<std::endl;

    std::cout<<"vitesse "<<speedTarget[sensor]<<std::endl;

    std::cout<<"----------------------------------------"<<std::endl;

    ui->label_smaractPositionLabel->setText(QString::number(ui->horizontalSlider_smaract->value()));
    ui->label_smaractSpeedLabel->setText(QString::number(ui->dial_smaract->value()));

    m_chronolab.setTargetPos(positionTarget, speedTarget);

}

//SMARACT Consigne en vitesses
void MainWindow::setSmaractSpeed()
{
    if(!ui->checkBox_smaractSpeedControl->isChecked())
        return;

    int sensor = ui->comboBox_smaract->currentIndex();
    //std::cout<<"sensor speed "<<sensor<< std::endl;

    double speedTarget[AXISNB] = {0};
    speedTarget[sensor] = ui->dial_smaract->value();

    m_chronolab.setTargetSpeed(speedTarget);

    ui->label_smaractSpeedLabel->setText(QString::number(ui->dial_smaract->value()));

}

void MainWindow::setSmaractDebugValues()
{
    int sensor = ui->comboBox_smaract->currentIndex();
    std::cout<<"sensor debug"<<sensor<<std::endl;
    switch(sensor)
    {
        case 0:
            ui->horizontalSlider_smaract->setMinimum(-360000000);
            ui->horizontalSlider_smaract->setMaximum(360000000);
            break;
        case 1:
            ui->horizontalSlider_smaract->setMinimum(-41000000);
            ui->horizontalSlider_smaract->setMaximum(41000000);
            break;
        case 2:
            ui->horizontalSlider_smaract->setMinimum(-50000000);
            ui->horizontalSlider_smaract->setMaximum(50000000);

            break;
        case 3:
            ui->horizontalSlider_smaract->setMinimum(-21000000);
            ui->horizontalSlider_smaract->setMaximum(21000000);
            break;
        case 4:
            ui->horizontalSlider_smaract->setMinimum(-21000000);
            ui->horizontalSlider_smaract->setMaximum(21000000);
            break;

    }

    int pos = m_chronolab.getPosition(sensor);
    int speed = round(m_chronolab.getSpeed(sensor));

    ui->horizontalSlider_smaract->setValue(pos);
    ui->dial_smaract->setValue(speed);

    ui->label_smaractPositionLabel->setText(QString::number(pos));
    ui->label_smaractSpeedLabel->setText(QString::number(speed));
}

void MainWindow::onConnectTT()
{
    if(!m_teletweezer.isRunning())
    {
        if(m_teletweezer.connect(ui->comboBox_TTCOM->currentText()))
        {
            ui->pushButton_connectTT->setText("STOP");
            m_teletweezer.start();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"),
                                           tr("Unable to connect to Teletweezer."),
                                           QMessageBox::Ok);
        }
    }
    else
    {
        m_teletweezer.stop();
        ui->pushButton_connectTT->setText("CONNECT");
    }
}

void MainWindow::onSendDataTT()
{
    QByteArray writeData = ui->lineEdit_TTdata->text().toLatin1();
    m_teletweezer.sendData(writeData);
}

void MainWindow::onConnectOptitrack()
{
    if(!m_optitrack.isRunning())
    {
        if(m_optitrack.connect())
        {
            ui->pushButton_connectOptitrack->setText("STOP");
            m_optitrack.start();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"),
                                           tr("Unable to connect to Optitrack."),
                                           QMessageBox::Ok);
        }
    }
    else
    {
        m_optitrack.stop();
        ui->pushButton_connectOptitrack->setText("CONNECT");
    }
}

void MainWindow::onCalibrateOptitrack()
{
    m_optitrack.calibrate();
    ui->pushButton_optiCalib->setEnabled(false);
}

void MainWindow::optiCalibrating(int pcentage)
{
    if(pcentage >= 100)
    {
        ui->progressBar_optiCalib->setValue(pcentage);
        ui->pushButton_optiCalib->setEnabled(true);
        ui->checkBox_optiTip->setEnabled(true);
    }
    else
    {
        ui->progressBar_optiCalib->setValue(pcentage);
    }
}

// Transformer la position obtenue par optitrack à la position de la pointe
void MainWindow::onTipTransform()
{
    m_optitrack.setTransformToTip(ui->checkBox_optiTip->isChecked());
}

void MainWindow::onConnectMutweez()
{
    if(!m_mutweez.isRunning())
    {
        // connectTo(QString IP, int rcvPort, int sndPort)
        if(!m_mutweez.connectTo("200.0.0.3",10000,20000))
        {
            std::cout << "Connection failed to 200.0.0.1" << std::endl;
            return;
        }
        m_mutweez.start();
        int cpt = 0;
        // cpt est un compteur pour laisser le temps de lire les infos
        // envoyer par le micro pince
        while(!m_mutweez.isConnected() && cpt < 5)
        {
            QThread::msleep(15);
            cpt++;
        }
        if(m_mutweez.isConnected())
        {
            ui->pushButton_connectmuTweezer->setText("STOP");
        }
        else
        {
            QMessageBox::critical(this, tr("Error"),
                                           tr("Unable to connect to micro tweezer."),
                                           QMessageBox::Ok);
        }
        /*if(m_mutweez.connectTo("200.0.0.3",9760))
        {
            ui->pushButton_connectmuTweezer->setText("STOP");
            m_mutweez.start();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"),
                                           tr("Unable to connect to micro tweezer."),
                                           QMessageBox::Ok);
        }*/
    }
    else
    {
        m_mutweez.stop();
        ui->pushButton_connectmuTweezer->setText("CONNECT");
    }
}

void MainWindow::onSendDataMuT()
{
    QByteArray writeData = ui->lineEdit_muTweezData->text().toLatin1();
    m_mutweez.sendData(writeData);
}

MainWindow::~MainWindow()
{
    m_chronolab.stop();

    delete ui;
}



/*
// Communication entre bloc OptiTrack et Smaract
void MainWindow::Couplage_OptitrackToSmaract()
// La position obtenue par OptiTrack contrôle celle de smaract
{

    // vérification de l'état de smaract et optitrack
    if(m_chronolab.isRunning() && m_optitrack.isRunning())
    {
        qDebug() << "m_chronolab.isRunning() = " << m_chronolab.isRunning() << endl;
        qDebug() << "m_optitrack.isRunning() = " << m_optitrack.isRunning() << endl;

        // Côté OPTITRACK
        // Data récupéré d'OptiTrack, sturcture optitrack_data
        optitrack_data OptiData = m_optitrack.getData();

        // Récupération des données
        // Position marker
        OptiPosX = OptiData.posx;
        OptiPosY = OptiData.posy;
        OptiPosZ = OptiData.posz;
        // Position l'avant de pince
        OptiPosXT = OptiData.posxT;
        OptiPosYT = OptiData.posyT;
        OptiPosZT = OptiData.poszT;
        // Rotations
        OptiYaw = OptiData.yaw;
        OptiPitch = OptiData.pitch;
        OptiRoll = OptiData.roll;

        //Affichage de résultats, QDebug
        qDebug() << "OptiPosX = " << OptiPosX << endl;
        qDebug() << "OptiPosY = " << OptiPosY << endl;
        qDebug() << "OptiPosZ = " << OptiPosZ << endl;
        qDebug() << "OptiPosXT = " << OptiPosXT << endl;
        qDebug() << "OptiPosYT = " << OptiPosYT << endl;
        qDebug() << "OptiPosZT = " << OptiPosZT << endl;
        qDebug() << "OptiYaw = " << OptiYaw << endl;


        // Remplissage de Tableau targetPos[], rotation, translation Y et X
        float targetPos[3] = {OptiYaw, OptiPosYT, OptiPosXT};
        qDebug() << "targetPos[3] = " << targetPos << endl;
        qDebug() << "int(sizeof (targetPos)) = " << int(sizeof (targetPos)) << endl;


        // Remplissage de tableau targetSpeed[], nécessaire de le faire???
        // Vitesses AP2M: default speed = 1500000; //1.5mm/sec
        int targetSpeed[3] = {1500000,1500000,1500000};


        // Côté Chronolab_smaract
        // Faire la limite de Position et la vitesse
        for(int i=0; i< int(sizeof (targetPos));i++)
        {
            switch ( i )
            {
                //Rotation
                case 0:
                    //limite rotation
                    if(targetPos[i] > 360000000)
                        targetPos[i] = 360000000;
                    if(targetPos[i] < 0)
                        targetPos[i] = 0;

                    break;

                //Plateau Translation Y
                case 1:
                    if(targetSpeed[i] > 10000000)
                        targetSpeed[i] = 10000000;
                    if(targetSpeed[i] < -10000000)
                        targetSpeed[i] = -10000000;

                    if(targetPos[i] > 40000000)
                        targetPos[i] = 40000000;
                    if(targetPos[i] < -40000000)
                        targetPos[i] = -40000000;

                    break;

                //Plateau Translation X
                case 2:
                    if(targetSpeed[i] > 10000000)
                        targetSpeed[i] = 10000000;
                    if(targetSpeed[i] < -10000000)
                        targetSpeed[i] = -10000000;

                    if(targetPos[i] > 50000000)
                        targetPos[i] = 50000000;
                    if(targetPos[i] < -50000000)
                        targetPos[i] = -50000000;

                    break;

                //Micro Pince Translation Z
                case 3:
                    if(targetSpeed[i] > 1000000)
                        targetSpeed[i] = 1000000;
                    if(targetSpeed[i] < -1000000)
                        targetSpeed[i] = -1000000;

                    if(targetPos[i] > 7000000)
                        targetPos[i] = 7000000;
                    if(targetPos[i] < -6000000)
                        targetPos[i] = -6000000;

                   break;

                //Micro Pince Translation Y
                case 4:
                    if(targetSpeed[i] > 1000000)
                        targetSpeed[i] = 1000000;
                    if(targetSpeed[i] < -1000000)
                        targetSpeed[i] = -1000000;

                    if(targetPos[i] > 13000000)
                        targetPos[i] = 12000000;
                    if(targetPos[i] < -5000000)
                        targetPos[i] = -5000000;

                   break;

                default:
                    break;
            }
        }

        newTargetPos = true;




        // Vérification d'état de marche Smaract
        if (m_chronolab.isRunning() && m_chronolab.getState() == SMARACT_STATE::READY)
        {
            qDebug() << "m_chronolab.isRunning() = " << m_chronolab.isRunning() << endl;
            qDebug() << "m_chronolab.getState() = " << m_chronolab.getState() << endl;

            // get target pos/speed
            SA_STATUS ret;
            int LastSensorPos[int(sizeof (targetPos))];

            // Position sensor check
            for( int i = 0; i < int(sizeof (targetPos)); i++)
            {
                LastSensorPos[i] = sensorPos[i];
                switch (sensorsType[i])
                {
                case LINEAR :
                    ret = SA_GetPosition_S(deviceIndex, i, &sensorPos[i]);
                    // Returns the current position of a positioner
                    // position (signed int 32bit), output - If the call was successful
                    // this value holds the current position of the positioner
                    // in NANO meters.

                    qDebug() << "ret SA_GetPosition_S : " << ret << endl;
                    qDebug() << "sensorPos[i] = " << sensorPos[i] << endl;

                    if(ret != SA_OK && ret!= SA_NO_SENSOR_PRESENT_ERROR)
                    {
                        emit(m_chronolab.smaractError());
                    }

                    distance[i] = sensorPos[i] - LastSensorPos[i];

                    // Affichage des résultats
                    qDebug() << i << "Sensor Pos = " << sensorPos[i] << endl;
                    qDebug() << i << "Last Sensor Pos = " << LastSensorPos[i] << endl;
                    qDebug() << i << "distance = " << distance[i] << endl;

                    break;

                case ROTARY:

                    uint Angle=0;
                    int Revolution;
                    ret = SA_GetAngle_S(deviceIndex, i, &Angle, &Revolution);
                    // Returns the current angle of a positioner.
                    // One revolution equals a full 360° turn.
                    // The angle value returned will be in the range 0..359,999,999

                    qDebug() << i << "ret SA_GetAngle_S : " << ret << endl;
                    qDebug() << i << "Revolution = " << Revolution << endl;
                    qDebug() << i << "Angle = " << Angle << endl;

                    if(ret != SA_OK && ret!= SA_NO_SENSOR_PRESENT_ERROR)
                    {
                        emit(m_chronolab.smaractError());
                    }
                    sensorPos[i] = Angle ; //+ revolution*360000000;
                    distance[i] = sensorPos[i] - LastSensorPos[i];

                    // Affichage des résultats
                    qDebug() << i << "Sensor Pos = " << sensorPos[i] << endl;
                    qDebug() << i << "Last Sensor Pos = " << LastSensorPos[i] << endl;
                    qDebug() << i << "distance = " << distance[i] << endl;

                    break;
                }
            }

            // Send target pos
            if(newTargetPos)
            {
                for (int i = 0; i < int(sizeof (targetPos)); i++)
                {
                    switch (sensorsType[i])
                    {
                    qDebug() << "sensorsType[i] = " << sensorsType << endl;

                    case LINEAR:
                    {
                        //std::cout<<"positions " << i << " " <<m_targetPos[i]<<std::endl;

                        if(m_currentControlType == SMARACT_CONTROL_TYPE::SPEED)
                        {
                            SA_Stop_S(m_deviceIndex, i);
                        }
                        m_currentControlType = SMARACT_CONTROL_TYPE::POSITION;

                        SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i]));
                        // This function configures the speed control feature of a channel
                        // for closed-loop commands such as SA_GotoPositionAbsolute_S.
                        // systemIndex (unsigned 32bit), input - Handle to an initialized system
                        // channelIndex (unsigned 32bit), input -
                        // Selects the channel of the selected system. The index is zero based.
                        // speed (unsigned 32bit), input - Sets the movement speed for
                        // closed-loop commands which is given in NANOMETERS per second
                        // for linear positioners and in MICRO DEGREES per second for rotary positioners.
                        // The valid range is 0..100,000,000.
                        // A value of 0 (default) deactivates the speed control feature.

                        SA_GotoPositionAbsolute_S(m_deviceIndex,i,m_targetPos[i],0);
                        // Move to m_targetPos[i] with m_targetgetSpeed[i]
                        // Instructs a positioner to move to a specific position.
                        // the command is only executable if the addressed channel is configured
                        // to be of type linear (see SA_SetSensorType_S).
                        // A rotary channel will return an error (use SA_GotoAngleAbsolute_S instead)
                        // While executing the command the positioner will have a movement status
                        // of SA_TARGET_STATUS. While holding the target position the positioner
                        // will have a movement status of SA_HOLDING_STATUS
                        // In asynchronous communication mode an error will be reported
                        // Position (signed 32bit), input - Absolute position to move to in NANO meters.
                        // holdTime (unsigned 32bit), input - Specifies how long (in MILLISECONDS)
                        // the position is actively held after reaching the target.
                        // The valid range is 0..60,000. A 0 deactivates this feature,
                        // a value of 60,000 is infinite (until manually stopped, see SA_Stop_S).

                        break;
                    }
                    case ROTARY:
                    {
                        //std::cout<<"positions " << i << " " <<m_targetPos[i]<<std::endl;
                        if(m_currentControlType == SMARACT_CONTROL_TYPE::SPEED)
                        {
                            SA_Stop_S(m_deviceIndex, i);
                        }
                        m_currentControlType = SMARACT_CONTROL_TYPE::POSITION;

                        SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i]));
                        uint angle = m_targetPos[i]%360000000;
                        int revolution = m_targetPos[i]/360000000;

                        SA_GotoAngleAbsolute_S(m_deviceIndex, i, angle, revolution, 0);
                        // angle (unsigned 32bit), input - Absolute angle to move to in micro degrees.
                        // The valid range is 0..359,999,999.
                        // revolution (signed 32bit), input - Absolute revolution to move to.
                        // The valid range is -32,768..32,767.
                        // holdTime (unsigned 32bit), input - Specifies how long (in milliseconds)
                        // the angle is actively held after reaching the target. The valid range is 0..60,000.
                        // A 0 deactivates this feature, a value of 60,000 is infinite

                        // move to 90° angle and hold position for one second
                        // result = SA_GotoAngleAbsolute_S(mcsHandle,0,90000000,0,1000);
                        break;
                    }
                    }

                    //int movStatus;
                    //unsigned int status;
                    //movStatus = SA_GetStatus_S(m_deviceIndex,i, &status);
                    //std::cout<< i << " " <<movStatus << " " << status <<std::endl;
                }
                //m_newTargetPos = false;
                //std::cout<<"----------------------------- " <<std::endl;
            }



        }



    }
    else
    {
        m_chronolab.stop();
        m_optitrack.stop();
    }
}*/




/*
// Communication entre bloc MuT et TT
// Ouverture de pince contrôle celle de micro-pince
void MainWindow::Couplage_TTToMuT()
{

    // Compteur
    int cpt = 1;

    // if(m_teletweezer.isRunning() && m_mutweez.isRunning() && cpt<=500)
    if(m_teletweezer.isRunning() && cpt<=500)
    {
        // Affichage de cpt
        qDebug() << "cpt = " << cpt << endl;

        // Côté maître TT
        // recuperation stringlist Ouverture de la pince
        // QStringList to QByteArray (en mm, de 0 a 10)
        QStringList OuvertureTT = m_teletweezer.getData();

        // on recupere le premier mot de la liste
        QString mot = OuvertureTT.at(0);

        // Remplissage de tableau d'ouverture TT
        OuvTT[cpt-1] = mot.toDouble();

        // Conversion qui depend de la micro pince elle-meme
        // MuT: -10 V a +10 V, [-10, +10]
        // TT : 0 mm a 10 mm, [0, 10]
        double ConsignePourMuT = (OuvTT[cpt-1]-(-10))/2; //en mm

        // double to QString
        QString DataToMuT = QString::number(ConsignePourMuT);

        // on declare le tableau QByteArray
        QByteArray tableau;

        // On rempli le QbyteArray lettre par lettre
        for(int i = 0 ; i < DataToMuT.size() ; ++i)
            tableau.push_back(DataToMuT.at(i).toLatin1());

        // Envoie de l'ouverture de TT à MuT (esclave)
        m_mutweez.sendData(tableau);

        // Affichage de l'ouverture TT sur ui et qDebug
        // 1 fois par 100 itérations
        if (cpt%100 == 0)
        {
            qDebug() << "Ouverture TT : " << OuvertureTT[0][0] << endl;
            qDebug() << "Ouverture TT : " << tableau << endl;
            qDebug() << "Consigne pour MuT : " << ConsignePourMuT << endl;
            ui->label_TTread->setText(mot); //ui
        }



        // Côté MuT (esclave)
        // Récupération de l'ouverture de MuT
        QString OuvertureMuT = m_mutweez.getData();

        //Remplissage de tableau d'ouverture MuT
        OuvMuT[cpt-1] = OuvertureMuT.toDouble();

        // Affichage de l'ouverture de MuT sur l'utilisateur interface
        // Affichage 1 fois par 100 itérations
        if (cpt%100 == 0)
        {
            qDebug() << "Ouverture recupreree du MuT : " << OuvertureMuT << endl;
            ui->label_muTweezConnected->setText(OuvertureMuT);
        }



        // Retour haptique ???
        // Envoie de feedback de MuT a TT
        // Conversion
        double ConsignePourTT = 2*OuvMuT[cpt-1]+(-10); // en V
        // double to QString
        QString DataToTT = QString::number(ConsignePourTT);
        // QString to QByteArray
        QByteArray tableau2;
        for(int i = 0 ; i < DataToTT.size() ; ++i)
            tableau2.push_back(DataToTT.at(i).toLatin1());
        // Envoie de OuvMuT a TT comme un feedback
        m_teletweezer.sendData(tableau2);
        // Affichage
        if (cpt%100 == 0)
        {
            qDebug() << "ConsignePourTT : " << ConsignePourTT << endl;
            qDebug() << "DataToTT : " << DataToTT << endl;
            qDebug() << "tableau2 : " << tableau2 << endl;
        }

        // Incrementation du compteur
        cpt++;

    }
    else{
        m_teletweezer.stop();
        m_mutweez.stop();
    }
}
*/


// Communication entre MuT et TT
// Ouverture de pince contrôle celle de micro-pince
void MainWindow::on_Couplage_TTToMuT_clicked()
{
    // cpt
    int cpt = 1;

    // Essaie UDP pour MPLAB
    while(cpt <= 20000)
    // while(1)
    {
        // QString pic = QString::number(5);
        QString pic = "c";
        QByteArray ToPIC;
        for(int i = 0 ; i < pic.size() ; ++i)
            ToPIC.push_back(pic.at(i).toLatin1());
        QByteArray ToPIC2 = pic.toUtf8();
        m_mutweez.sendData(ToPIC);
        m_mutweez.sendData(ToPIC2);
        m_mutweez.sendMessage(pic);
        qDebug() << "serie de nombre :" << pic << endl;
        qDebug() << "m_mutweez.isConnected() :" << m_mutweez.isConnected() << endl;
        QString pic1 = m_mutweez.getData();
        ui -> lineEdit_testZibo3 -> setText("pic1 :" + pic1);
        qApp -> processEvents();
        cpt++;

    }


    // Côté maître TT
    // boucle de couplage entre la pince et la micro pince
    while(m_teletweezer.isRunning())
    {

        // Ouverture de la pince
        QStringList OuvertureTT = m_teletweezer.getData();
        qDebug() << "1" << endl;
        QString mot = OuvertureTT.at(0);
        double OuvTT = mot.toDouble();
        qDebug() << "2" << endl;


        // Conversion qui depend de la micro pince elle-meme
        // MuT: -10 V a +10 V, [-10, +10]
        // TT : 0 mm a 10 mm, [0, 10]
        double ConsignePourMuT = (OuvTT-(-10))/2; // en mm
        qDebug() << "3" << endl;
        ui -> lineEdit_testZibo -> setText("Ouverture TT :" + mot);
        qDebug() << "4" << endl;


        // Double -> QString -> QByteArray
        QString DataToMuT = QString::number(ConsignePourMuT);
        qDebug() << "5" << endl;
        ui -> lineEdit_testZibo2 -> setText("ConsignePourMuT :" + DataToMuT);
        qDebug() << "6" << endl;
        QByteArray tableau;
        for(int i = 0 ; i < DataToMuT.size() ; ++i)
            tableau.push_back(DataToMuT.at(i).toLatin1());
        qDebug() << "7" << endl;



        // Côté MuT (esclave)
        // Envoie de l'ouverture de TT à MuT (esclave)
        qDebug() << "m_mutweez.isRunning() : " << m_mutweez.isRunning() << endl;
        qDebug() << "m_mutweez.isConnected() : " << m_mutweez.isConnected() << endl;

        m_mutweez.sendData(tableau);
        // Récupération de l'ouverture de MuT
        QString OuvertureMuT = m_mutweez.getData();
        qDebug() << "m_redadata.size() : " << OuvertureMuT.size() << endl;
        // qDebug() << "m_newDataToSend : " << m_mutweez.getNewDataToSend()<< endl;
        // ui -> lineEdit_testZibo3 -> setText("OuvertureMuT :" + OuvertureMuT);
        qDebug() << "Ouverture MuT = " << OuvertureMuT.toDouble() << endl;


        // Affichage
        if (cpt%1000 ==0)
        {
            // qDebug() << "cpt = " << cpt << endl;
            // ui -> lineEdit_testZibo -> setText(mot);
            // qDebug() << "mot = " << mot << endl;
            // qDebug() << "ConsignePourMuT = " << ConsignePourMuT << endl;
        }

        qDebug() << "8" << endl;
        qApp -> processEvents();
        cpt++;

    }

    // m_teletweezer.stop();
    qDebug() << "cpt : " << cpt << endl;
    qDebug() << "teletweezer stoped !" << endl;

}


/*
// Communication entre Optitrack et Smaract
// Position obtenue en X d'Optitrack controle celle de Smaract
void MainWindow::on_Couplage_OptitrackToSmaract_clicked()
{
    int cpt =1;

    while(m_optitrack.isTracked() && m_chronolab.isRunning())
    {

        // Récupération des données
        optitrack_data OptiData = m_optitrack.getData();
        // Position transformé en X
        ui -> lineEdit_testZibo4 -> setText("xT: " + QString::number(OptiData.posxT));

        // Sensor number, 2 qui contrôle translation plateau X
        // Vitesses AP2M: default speed = 1500000; //1.5mm/sec
        double sTarget[] = {1500000};
        // Pas besoin de mettre la limite???
        // case 2 limite [-50000000,50000000]
        double pTarget[] = {OptiData.posxT};

        m_chronolab.setTargetPos(pTarget, sTarget);

        // Get Position and speed
        double SpeedSmaract = m_chronolab.getSpeed(2);
        int PositionSmaract = m_chronolab.getPosition(2);

        ui -> lineEdit_testZibo5 -> setText("SpeedSmaract: " + QString::number(SpeedSmaract));
        ui -> lineEdit_testZibo6 -> setText("PositionSmaract: " + QString::number(PositionSmaract));

        cpt++;
        qApp->processEvents();
    }
*/

/*
    while(m_optitrack.isTracked() && m_chronolab.isRunning())
    {

        // Récupération des données
        optitrack_data OptiData = m_optitrack.getData();
        // Position transformé en X
        ui -> lineEdit_testZibo4 -> setText("xT: " + QString::number(OptiData.posxT));

        // Sensors contrôlent rotation, translation plateau Y X et micro pince translation Z Y
        // Vitesses AP2M: default speed = 1500000; //1.5mm/sec
        double sTarget[] = {1500000,1500000,1500000,1500000,0};
        // Pas besoin de mettre la limite???
        // case 2 limite [-50000000,50000000]
        double pTarget[] = {OptiData.yaw, OptiData.posyT,OptiData.posxT,OptiData.poszT,0};

        m_chronolab.setTargetPos(pTarget, sTarget);

        // Get Position and speed
        double SpeedSmaract = m_chronolab.getSpeed(2);
        int PositionSmaract = m_chronolab.getPosition(2);

        ui -> lineEdit_testZibo5 -> setText("SpeedSmaract: " + QString::number(SpeedSmaract));
        ui -> lineEdit_testZibo6 -> setText("PositionSmaract: " + QString::number(PositionSmaract));

        cpt++;
        qApp->processEvents();
    }
    */
//}
