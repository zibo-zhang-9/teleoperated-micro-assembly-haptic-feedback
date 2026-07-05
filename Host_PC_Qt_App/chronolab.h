#ifndef CHRONOLAB_H
#define CHRONOLAB_H

#include <QObject>
#include <QThread>

#include "MCSControl.h"

#define AXISNB 5 //6
#define MAXSPEED 20 // speed max of linear positioner

enum SMARACT_STATE{ NODEVICE, CALIBRATIONNEED, INCALIBRATION, READY, CONNEXIONERROR };

enum SMARACT_SENSOR_TYPE{ UNCONNECTED, UNKNOWN, LINEAR, ROTARY };

enum SMARACT_CONTROL_TYPE{ POSITION, SPEED };

class Chronolab : public QThread
{
    Q_OBJECT
public:
    Chronolab();
    ~Chronolab();

    void run();

    void stop();

    void speedControlStop();

    void setTargetPos(double targetPos[AXISNB], double targetSpeed[AXISNB]);

    void setTargetSpeed(double targetSpeed[AXISNB]);

    QString positionerTypeToString(uint sensor);

    int getPosition(unsigned int i);
    double getDistance(unsigned int i);
    double getSpeed(unsigned int i);
    SMARACT_STATE getState();

private:
    void stopAll();

    SMARACT_SENSOR_TYPE sensorType(uint sensor);

    unsigned int m_deviceIndex;

    SMARACT_STATE m_state;

    bool m_running;
    bool m_newTargetPos;
    bool m_newTargetSpeed;

    SMARACT_CONTROL_TYPE m_currentControlType;

    unsigned int m_sensorsType[AXISNB];
    unsigned int m_sensorsTypeID[AXISNB];

    int m_targetPos[AXISNB];
    int m_targetSpeed[AXISNB];

    int m_sensorPos[AXISNB];
    double m_distance[AXISNB];
    double m_speed[AXISNB];


    //////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief calculate PID
     *
     * @param setValue
     * @param actualValue
     */
    //float PIDcal(float setValue,float actualValue);
    //void resetPid();

signals:
    void smaractError();

public slots:

    /**
     * @brief Submit request for calibration
     *
     */
    void calibrate();
};

#endif // CHRONOLAB_H
