#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "chronolab.h"
#include "teletweezer.h"
#include "optitrack.h"
#include "mutweezer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Chronolab m_chronolab;

    Teletweezer m_teletweezer;

    Optitrack m_optitrack;

    MuTweezer m_mutweez;

    QTimer timerUpdateGUI;


private slots:
    void updateGUI();

    void onSMARACTError();

    void setSmaractPosition();
    void setSmaractSpeed();
    void setSmaractDebugValues();
    void doEmergencyStop();
    void doControlSpeedStop();

    // TT
    void onConnectTT();
    void onSendDataTT();

    // Optitrack
    void onConnectOptitrack();
    void onCalibrateOptitrack();
    void optiCalibrating(int pcentage);
    void onTipTransform();

    //micro Pince
    void onConnectMutweez();
    void onSendDataMuT();


    /*
    // Communication entre les blocs

    // Position obtenue par OptiTrack contrôle celle de Smaract
    // Declaration des positions/rotations, variables générales
    // Positions de marker
    float OptiPosX;
    float OptiPosY;
    float OptiPosZ;
    // Positions de l'avant de pince après la transformation homogène
    float OptiPosXT;
    float OptiPosYT;
    float OptiPosZT;
    // Rotations
    float OptiYaw;
    float OptiPitch;
    float OptiRoll;

    // Autres variables
    // float targetPos[3]; // Plateau translation Y + X et plateau rotation Yaw
    bool newTargetPos;
    double distance[AXISNB];
    unsigned int deviceIndex[3] = {1,2,3};
    int sensorPos[AXISNB];
    unsigned int sensorsType[3]={ROTARY,LINEAR,LINEAR};


    void Couplage_OptitrackToSmaract();
    */
    /*
    // Ouverture de la pince contrôle celle de la micro-pince
    // Tableaux d'ouvertures, taille arbitaire (500)
    double OuvTT[500];      // en mm
    double OuvMuT[500];     // en V
    void Couplage_TTToMuT();
    */

    void on_Couplage_TTToMuT_clicked();
    /*void on_Couplage_OptitrackToSmaract_clicked();*/
};

#endif // MAINWINDOW_H
