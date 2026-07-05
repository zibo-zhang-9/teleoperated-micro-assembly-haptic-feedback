#include "chronolab.h"
#include "qpc.h"
#include <iostream>

Chronolab::Chronolab() : QThread()
{
    m_running = false;
    m_newTargetPos = false;
    m_newTargetSpeed = false;

    for(int i=0; i<AXISNB; i++)
    {
        m_targetPos[i] = 0.0;
        m_targetSpeed[i] = 0.0;

        m_sensorPos[i] = 0;
        m_distance[i] = 0.0;
        m_speed[i] = 0.0;
    }

    m_state = SMARACT_STATE::NODEVICE;

    static unsigned int devicesIDList[25];
    unsigned int sz = 25;
    SA_STATUS ret;

    ret =  SA_GetAvailableSystems(devicesIDList, &sz);
    // listSize holds the number of systems
    // idList array holds the systems IDs
    // idList (unsigned 32bit), output - This parameter
    // must be a pointer to an array of 32bit values.
    // If the function call was successful, the list
    // holds the system IDs of all connected systems
    // that are ready for acquisition.
    // idListSize (unsigned 32bit), input/output -
    // This parameter must hold the size of the array that
    // the idList pointer points to. When the function returns,
    // this parameter will always hold the number of system IDs that
    // have been written to the array. If an error occurred, it will be zero.

    if (ret != SA_OK || sz != 1)
    {
        emit(smaractError());
        return;
    }

    SA_ClearInitSystemsList();

    for(uint i = 0; i < sz; i++)
        SA_AddSystemToInitSystemsList(devicesIDList[i]);
        // systemId (unsigned 32bit), input -
        // ID of the system that is to be added to the system ID list.

    SA_InitSystems(SA_SYNCHRONOUS_COMMUNICATION);
    // All available systems were acquired.
    // Use SA_GetNumberOfSystems and SA_GetSystemID to see
    // how many and which systems these are.
    // configuration (unsigned 32bit), input -
    // This parameter selects between the synchronous and
    // the asynchronous communication. Possible values are
    // SA_SYNCHRONOUS_COMMUNICATION and SA_ASYNCHRONOUS_COMMUNICATION.

    m_deviceIndex = devicesIDList[1];

    m_state = SMARACT_STATE::READY;

    //std::cout << "deviceID " << devicesIDList[0] << " index " << m_deviceIndex  << std::endl;

    for (int i = 0; i < AXISNB; i++)
    {
        ret = SA_SetStepWhileScan_S(m_deviceIndex,i,SA_NO_STEP_WHILE_SCAN);
        // systemIndex (unsigned 32bit), input - Handle to an initialized system.
        // channelIndex (unsigned 32bit), input -
        // Selects the channel of the selected system. The index is zero based.
        // step (unsigned 32bit), input - Selects the mode. Must be either
        // SA_NO_STEP_WHILE_SCAN or SA_STEP_WHILE_SCAN. The latter is the default.
        // forbid to do correction steps while holding position (SA_NO_STEP_WHILE_SCAN)

        // This function is of interest in conjunction with closed-loop commands
        // (e.g. SA_GotoPositionAbsolute_S, see there)
        // and sets a flag that affects the behavior of a positioner.
        // If the positioner is instructed to hold the target position after reaching it,
        // the scanning mode will primarily be used to hold the position
    }

    uint type = 666;
    for (int i = 0; i < AXISNB; i++)
    {
        ret = SA_GetChannelType(m_deviceIndex, i, &type);
        // systemIndex (unsigned 32bit), input -  Handle to an initialized system.
        // channelIndex (unsigned 32bit), input -
        // Selects the channel of the selected system. The index is zero based.
        // type (unsigned 32bit), output - If the call was successful this parameter
        // holds the channel type of the selected channel. Possible values are
        // SA_POSITIONER_CHANNEL_TYPE and SA_END_EFFECTOR_CHANNEL_TYPE.

        if ( type == SA_POSITIONER_CHANNEL_TYPE)
        {
            ret = SA_GetSensorType_S(m_deviceIndex, i, &type);
            // systemIndex (unsigned 32bit), input - Handle to an initialized system.
            // channelIndex (unsigned 32bit), input -
            // Selects the channel of the selected system. The index is zero based.
            // type (unsigned 32bit), input - Specifies the type of the sensor
            // (see appendix 5.4 “Sensor Types“).

            if (ret == SA_OK)
            {
                // type holds the channel type of the first channel
                // of the first system
                m_sensorsType[i] = sensorType(type);
                m_sensorsTypeID[i] = type;
            }
            else
            {
                m_sensorsType[i] = sensorType(42);
                m_sensorsTypeID[i] = type;
            }
        }
        else
        {
            m_sensorsType[i] = sensorType(0);
            m_sensorsTypeID[i] = type;
        }
    }
    // enable sensors
    SA_SetSensorEnabled_S(m_deviceIndex,SA_SENSOR_ENABLED);
    // systemIndex (unsigned 32bit), input - Handle to an initialized system.
    // enabled (unsigned 32bit), input - Sets the mode. Must be either
    // SA_SENSOR_DISABLED, SA_SENSOR_ENABLED or SA_SENSOR_POWERSAVE.

}

Chronolab::~Chronolab()
{
    stopAll();
    //SA_SetSensorEnabled_S(m_deviceIndex,SA_SENSOR_DISABLED);
}

void Chronolab::stopAll()
{
    for (int i = 0; i < AXISNB; i++)
        SA_Stop_S(m_deviceIndex, i);
        // systemIndex (unsigned 32bit), input - Handle to an initialized system.
        // channelIndex (unsigned 32bit), input -
        // Selects the channel of the selected system. The index is zero based.

    m_state = SMARACT_STATE::CONNEXIONERROR;
}

void Chronolab::calibrate()
{
    for (int i = 0; i < AXISNB; i++)
    {
        SA_CalibrateSensor_S(m_deviceIndex, i);
    }
    // done calibrating

    m_state = SMARACT_STATE::READY;
}

void Chronolab::stop()
{
    m_running = false;
}

void Chronolab::speedControlStop()
{
    m_newTargetSpeed = false;
    m_newTargetPos = false;
}

int Chronolab::getPosition(unsigned int i)
{
    return m_sensorPos[i];
}

double Chronolab::getDistance(unsigned int i)
{
    return m_distance[i];
}

double Chronolab::getSpeed(unsigned int i)
{
    return m_speed[i];
}

SMARACT_STATE Chronolab::getState()
{
    return m_state;
}

void Chronolab::setTargetPos(double targetPos[], double targetSpeed[])
{
    /*// TODO: LIMITS
    for(int i=0; i<AXISNB; i++)
    {
        m_targetPos[i] = targetPos[i];

        m_targetSpeed[i] = targetSpeed[i];
        if(fabs(m_targetSpeed[i]) > 100000000)
            m_targetSpeed[i] = 100000000 * m_targetSpeed[i]/fabs(m_targetSpeed[i]);
    }*/

    for(int i=0; i<AXISNB;i++)
    {
        m_targetPos[i] = targetPos[i];
        m_targetSpeed[i] = targetSpeed[i];

        switch ( i )
        {
            //Rotation
            case 0:
                if(m_targetSpeed[i] > 2000000)
                    m_targetSpeed[i] = 2000000;
                if(m_targetSpeed[i] < -2000000)
                    m_targetSpeed[i] = -2000000;

                //limite rotation à refaire ?
                if(m_targetPos[i] > 360000000)
                    m_targetPos[i] = 360000000;
                if(m_targetPos[i] < 0)
                    m_targetPos[i] = 0;

                break;

            //Plateau Translation Y
            case 1:
                if(m_targetSpeed[i] > 10000000)
                    m_targetSpeed[i] = 10000000;
                if(m_targetSpeed[i] < -10000000)
                    m_targetSpeed[i] = -10000000;

                if(m_targetPos[i] > 40000000)
                    m_targetPos[i] = 40000000;
                if(m_targetPos[i] < -40000000)
                    m_targetPos[i] = -40000000;

                break;

            //Plateau Translation X
            case 2:
                if(m_targetSpeed[i] > 10000000)
                    m_targetSpeed[i] = 10000000;
                if(m_targetSpeed[i] < -10000000)
                    m_targetSpeed[i] = -10000000;

                if(m_targetPos[i] > 50000000)
                    m_targetPos[i] = 50000000;
                if(m_targetPos[i] < -50000000)
                    m_targetPos[i] = -50000000;

                break;

            //Micro Pince Translation Z
            case 3:
                if(m_targetSpeed[i] > 1000000)
                    m_targetSpeed[i] = 1000000;
                if(m_targetSpeed[i] < -1000000)
                    m_targetSpeed[i] = -1000000;

                if(m_targetPos[i] > 7000000)
                    m_targetPos[i] = 7000000;
                if(m_targetPos[i] < -6000000)
                    m_targetPos[i] = -6000000;

               break;

            //Micro Pince Translation Y
            case 4:
                if(m_targetSpeed[i] > 1000000)
                    m_targetSpeed[i] = 1000000;
                if(m_targetSpeed[i] < -1000000)
                    m_targetSpeed[i] = -1000000;

                if(m_targetPos[i] > 13000000)
                    m_targetPos[i] = 12000000;
                if(m_targetPos[i] < -5000000)
                    m_targetPos[i] = -5000000;

               break;

            default:
                break;
        }
    }


    m_newTargetPos = true;
}

void Chronolab::setTargetSpeed(double targetSpeed[])
{
    // TODO: LIMITS
    // AP2M: default speed = 1500000; //1.5mm/sec

    for(int i=0; i<AXISNB; i++)
    {
        m_targetSpeed[i] = targetSpeed[i];
        if(i==0) //rotary
        {
            if(m_targetSpeed[i] > 2000000)
                m_targetSpeed[i] = 2000000;
            if(m_targetSpeed[i] < -2000000)
                m_targetSpeed[i] = -2000000;
        }
        else if (i==1 || i==2) //translation platforme
        {
            if(m_targetSpeed[i] > 10000000)
                m_targetSpeed[i] = 10000000;
            if(m_targetSpeed[i] < -10000000)
                m_targetSpeed[i] = -10000000;
        }
        else //translation micro-pince
        {
            if(m_targetSpeed[i] > 1000000)
                m_targetSpeed[i] = 1000000;
            if(m_targetSpeed[i] < -1000000)
                m_targetSpeed[i] = -1000000;
        }


    }
    m_newTargetSpeed = true;
}

SMARACT_SENSOR_TYPE Chronolab::sensorType(uint sensor)
{
    SMARACT_SENSOR_TYPE type;
    switch (sensor)
    {
    case 1:
    case 5:
    case 6:
    case 9:
    case 18:
    case 21:
    case 24:
    case 32:
    case 35:
    case 38:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
        type = SMARACT_SENSOR_TYPE::LINEAR;
        break;
    case 2:
    case 4:
    case 7:
    case 8:
    case 13:
    case 14:
    case 15:
    case 20:
    case 22:
    case 23:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 36:
    case 37:
    case 39:
        type = SMARACT_SENSOR_TYPE::ROTARY;
        break;
    case 0:
        type = SMARACT_SENSOR_TYPE::UNCONNECTED;
        break;
    default:
        type = SMARACT_SENSOR_TYPE::UNKNOWN;
        break;
    }
    return type;
}

QString Chronolab::positionerTypeToString(uint sensor)
{
    QString name;
    switch (m_sensorsType[sensor])
    {
    case LINEAR:
        name += "Linear";
        break;
    case ROTARY:
        name += "Rotary";
        break;
    case UNCONNECTED:
        name += "Unconnected";
        break;
    default:
        name += "Unknown";
        break;
    }

    name += " ";
    switch (m_sensorsTypeID[sensor])
    {
    case 1:
        name += "S";
        break;
    case 2:
        name += "SR";
        break;
    case 4:
        name += "MR";
        break;
    case 5:
        name += "SP";
        break;
    case 6:
        name += "SC";
        break;
    case 7:
        name += "M25";
        break;
    case 8:
        name += "SR20";
        break;
    case 9:
        name += "M";
        break;
    case 10:
        name += "GC";
        break;
    case 11:
        name += "GD";
        break;
    case 12:
        name += "GE";
        break;
    case 13:
        name += "RA";
        break;
    case 14:
        name += "GF";
        break;
    case 15:
        name += "RB";
        break;
    case 16:
        name += "G605S";
        break;
    case 17:
        name += "G775S";
        break;
    case 18:
        name += "SC500";
        break;
    case 19:
        name += "G955S";
        break;
    case 20:
        name += "SR77";
        break;
    case 21:
        name += "SD";
        break;
    case 22:
        name += "R20ME";
        break;
    case 23:
        name += "SR2";
        break;
    case 24:
        name += "SCD";
        break;
    case 25:
        name += "SRC";
        break;
    case 26:
        name += "SR36M";
        break;
    case 27:
        name += "SR36ME";
        break;
    case 28:
        name += "SR50M";
        break;
    case 29:
        name += "SR50ME";
        break;
    case 30:
        name += "G1045S";
        break;
    case 31:
        name += "G1395S";
        break;
    case 32:
        name += "MD";
        break;
    case 33:
        name += "G935M";
        break;
    case 34:
        name += "SHL20";
        break;
    case 35:
        name += "SCT";
        break;
    case 36:
        name += "SR77T";
        break;
    case 37:
        name += "SR120";
        break;
    case 38:
        name += "LC";
        break;
    case 39:
        name += "LR";
        break;
    case 40:
        name += "LCD";
        break;
    case 41:
        name += "L";
        break;
    case 42:
        name += "LD";
        break;
    case 43:
        name += "LE";
        break;
    case 44:
        name += "LED";
        break;
    case 45:
        name += "GDD";
        break;
    case 46:
        name += "GED";
        break;
    case 47:
        name += "G935S";
        break;
    case 48:
        name += "G605DS";
        break;
    case 49:
        name += "G775DS";
        break;
    }
    return name;
}

void Chronolab::run()
{

    if(m_state != SMARACT_STATE::READY)
        return;

    m_running = true;

    QPC timer;
    double oldTime = timer.getms();

    double loopDuration = 0;

    while(m_running && m_state == SMARACT_STATE::READY)
    {
        if(timer.getms() - oldTime < 20.0 - loopDuration)
            continue;
        //std::cout << "time " << timer.getms() - oldTime << std::endl;
        timer.tic();

        // get target pos/speed
        SA_STATUS ret;
        int lastSensorPos[AXISNB];
        // position sensor check
        for (int i = 0; i < AXISNB; i++)
        {
            lastSensorPos[i] = m_sensorPos[i];
            switch (m_sensorsType[i])
            {
            case LINEAR:
                ret = SA_GetPosition_S(m_deviceIndex, i, &m_sensorPos[i]);
                // Returns the current position of a positioner
                // position (signed 32bit), output - If the call was successful
                // this value holds the current position of the positioner
                // in NANO meters.

                if(ret != SA_OK && ret!= SA_NO_SENSOR_PRESENT_ERROR)
                {
                    m_state = CONNEXIONERROR;
                    emit(smaractError());
                }
                //std::cout << i << m_sensorPos[i] << std::endl;
                m_distance[i] = m_sensorPos[i] - lastSensorPos[i];
                break;

            case ROTARY:
                uint angle=0;
                int revolution;
                ret = SA_GetAngle_S(m_deviceIndex, i, &angle, &revolution);
                // Returns the current angle of a positioner.
                // One revolution equals a full 360° turn.
                // The angle value returned will be in the range 0..359,999,999
                if(ret != SA_OK && ret!= SA_NO_SENSOR_PRESENT_ERROR)
                {
                    m_state = CONNEXIONERROR;
                    emit(smaractError());
                }
                m_sensorPos[i] = angle ; //+ revolution*360000000;
                m_distance[i] = m_sensorPos[i] - lastSensorPos[i];
                break;
            }
            //std::cout << m_sensorPos[i] << " " << lastSensorPos[i] << std::endl;

        }
        //std::cout << "-----------------" << std::endl;

        // calculate speed
        double t = (timer.getms() - oldTime) / 1000.0;
        oldTime = timer.getms();
        //std::cout << "t " << t << std::endl;
        for (int i = 0; i < AXISNB; i++)
        {
            m_speed[i] = m_distance[i] / t;
        }

        // send target pos/speed

        if(m_newTargetPos)
        {
            for (int i = 0; i < AXISNB; i++)
            {
                switch (m_sensorsType[i])
                {
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

                /*int movStatus;
                unsigned int status;
                movStatus = SA_GetStatus_S(m_deviceIndex,i, &status);
                std::cout<< i << " " <<movStatus << " " << status <<std::endl;*/
            }
            //m_newTargetPos = false;
            //std::cout<<"----------------------------- " <<std::endl;
        }
        else if(m_newTargetSpeed)
        {
            for (int i = 0; i < AXISNB; i++)
            {
               // std::cout<<"vitesse " << i << " " <<m_targetSpeed[i]<<std::endl;
                switch (m_sensorsType[i])
                {
                case LINEAR:
                {
                    m_currentControlType = SMARACT_CONTROL_TYPE::SPEED;

                    if(m_targetSpeed[i] == 0)
                    {
                        SA_Stop_S(m_deviceIndex, i);
                        // Stops any ongoing movement of a positioner or end effector.
                        // Note that if a stepping movement is performed with a positioner
                        // the current step is completed before the positioner is stopped
                        // A positioner or end effector that is stopped will have
                        // a movement status of SA_STOPPED_STATUS
                    }
                    else
                    {
                        int direction = (m_targetSpeed[i]>0)?1:-1;

                        /*if (direction != lastDirection)
                           {
                               SA_Stop_S(deviceIndex, i);
                           }*/

                        // targetSpeed en millimetre
                        if (SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i])) == SA_OK)
                            // speed (unsigned 32bit), input - Sets the movement speed for closed-loop commands
                            // which is given in nanometers per second for linear positioners and
                            // in micro degrees per second for rotary positioners.
                            // The valid range is 0..100,000,000. A value of 0 (default) deactivates the speed control feature.
                            SA_GotoPositionAbsolute_S(m_deviceIndex, i, (direction>0 ? 2147483647 : -2147483648), 0);
                            // position (signed 32bit), input - Absolute position to move to in nano meters.
                            // holdTime (unsigned 32bit), input - Specifies how long (in milliseconds)
                            // the position is actively held after reaching the target.
                            // The valid range is 0..60,000. A 0 deactivates this feature, a value of 60,000 is infinite
                    }
                    int movStatus;
                    unsigned int status;
                    movStatus = SA_GetStatus_S(m_deviceIndex,i, &status);
                    // status (unsigned 32bit), output - If the call was successful this value
                    // holds the current status of the positioner.

                    std::cout<< i << " " <<movStatus << " " << status <<std::endl;

                    break;
                }
                case ROTARY:
                {
                    m_currentControlType = SMARACT_CONTROL_TYPE::SPEED;

                    if(m_targetSpeed[i] == 0)
                    {
                        SA_Stop_S(m_deviceIndex, i);
                    }
                    else
                    {
                        int direction = (m_targetSpeed[i]>0)?1:-1;

                        /*if (direction != lastDirection)
                           {
                               SA_Stop_S(deviceIndex, i);
                           }*/

                        // targetSpeed en millimetre
                        if (SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i])) == SA_OK)
                            SA_GotoAngleRelative_S(m_deviceIndex, i, 0, direction, 0);
                        break;
                    }
                }
                }
            }
            //m_newTargetSpeed = false;
        }

        else
        {
            for (int i = 0; i < AXISNB; i++)
                SA_Stop_S(m_deviceIndex, i);
        }


        /*for (int i = 0; i < AXISNB; i++)
        {
            switch (m_sensorsType[i])
            {
            case LINEAR:
            {
                if(m_newTargetPos)
                {
                    m_newTargetPos = false;
                    std::cout<<"positions :"<<m_targetPos[i]<<std::endl;

                    if(m_currentControlType == SMARACT_CONTROL_TYPE::SPEED)
                    {
                        SA_Stop_S(m_deviceIndex, i);
                    }
                    m_currentControlType = SMARACT_CONTROL_TYPE::POSITION;

                    SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i]));
                    SA_GotoPositionAbsolute_S(m_deviceIndex,i,m_targetPos[i],0);
                }
                else if(m_newTargetSpeed)
                {
                    m_currentControlType = SMARACT_CONTROL_TYPE::SPEED;

                    if(m_targetSpeed[i] == 0)
                    {
                        SA_Stop_S(m_deviceIndex, i);
                    }
                    else
                    {
                        int direction = (m_targetSpeed[i]>0)?1:-1;


        // targetSpeed en millimetre
        if (SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i])) == SA_OK)
            SA_GotoPositionAbsolute_S(m_deviceIndex, i, (direction>0 ? 2147483647 : -2147483648), 0);
    }
}
else
{
SA_Stop_S(m_deviceIndex, i);
}
break;
}
case ROTARY:
{
    if(m_newTargetPos)
    {
        m_newTargetPos = false;

        if(m_currentControlType == SMARACT_CONTROL_TYPE::SPEED)
        {
            SA_Stop_S(m_deviceIndex, i);
        }
        m_currentControlType = SMARACT_CONTROL_TYPE::POSITION;

        SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i]));
        uint angle = m_targetPos[i]%360000000;
        int revolution = m_targetPos[i]/360000000;

        SA_GotoAngleAbsolute_S(m_deviceIndex, i, angle, revolution, 0);
    }
    else if(m_newTargetSpeed)
    {
        m_currentControlType = SMARACT_CONTROL_TYPE::SPEED;

        if(m_targetSpeed[i] == 0)
        {
            SA_Stop_S(m_deviceIndex, i);
        }
        else
        {
            int direction = (m_targetSpeed[i]>0)?1:-1;


            // targetSpeed en millimetre
            if (SA_SetClosedLoopMoveSpeed_S(m_deviceIndex, i, abs(m_targetSpeed[i])) == SA_OK)
                SA_GotoAngleRelative_S(m_deviceIndex, i, 0, direction, 0);
        }
    }
    else
    {
        SA_Stop_S(m_deviceIndex, i);
    }
    break;
}
}
}*/

        loopDuration = timer.toc()*1000.0;


        /*oldTime = timer.getms();
std::cout << "debug" << std::endl;
        double timeToWait = (20.0 - t*1000.0)*1000.0;
        QPC waitTimer;
        waitTimer.usleep(timeToWait);
        std::cout << m_state << " " << timeToWait << std::endl;*/


        /*if (typeCodeTotypPositioner(positionerType[i]))
           pos = (int)pos % 360000000;*/
    }

    stopAll();
}
