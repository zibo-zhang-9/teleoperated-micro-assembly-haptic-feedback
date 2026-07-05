#include "optitrack.h"
#include "qpc.h"
#include <iostream>
#include <QDateTime>
#include "NATUtils.h"

Optitrack::Optitrack() : QThread()
{
    m_isRunning = false;

    m_isConnected = false;

    m_isUpdating = false;

    m_lastFrameID = -1;

    WSADATA WsaData;
    if (WSAStartup(0x202, &WsaData) == SOCKET_ERROR)
    {
        std::cout << "Optitrack error: Unable to start WSA" << std::endl;
        WSACleanup();
    }

    m_rbData.ID = -1;
    m_rbData.meanError = 0;
    m_rbData.posx = 0;
    m_rbData.posy = 0;
    m_rbData.posz = 0;
    m_rbData.qw = 0;
    m_rbData.qx = 0;
    m_rbData.qy = 0;
    m_rbData.qz = 0;
    m_rbData.yaw = 0;
    m_rbData.pitch = 0;
    m_rbData.roll = 0;
    m_rbData.tracked = false;

    m_tipTransform.resize(4);
    m_tipTransform << 0, 0, 0, 1;

    m_positionsX.reserve(CALIB_NB);
    m_positionsY.reserve(CALIB_NB);
    m_positionsZ.reserve(CALIB_NB);
    m_yaws.reserve(CALIB_NB);
    m_pitchs.reserve(CALIB_NB);
    m_rolls.reserve(CALIB_NB);

    m_calibIndice = 0;

    m_onCalibrate = false;

    m_tipTransformComputed = false;

    m_firstFrame = true;
}

Optitrack::~Optitrack()
{
    stop();
}

void Optitrack::stop()
{
    m_isRunning = false;
    closesocket(m_dataSocket);
}

bool Optitrack::isRunning()
{
    return m_isRunning;
}

bool Optitrack::isConnected()
{
    return m_isConnected;
}

bool Optitrack::isUpdating()
{
    return m_isUpdating;
}

bool Optitrack::isTracked()
{
    return m_rbData.tracked;
}

void Optitrack::calibrate()
{
    m_calibIndice = 0;
    m_tipTransformComputed = false;
    m_onCalibrate = true;
}

optitrack_data Optitrack::getData()
{
    return m_rbData;
    /*data.frameNumber = m_rbData.frameNumber;
    data.posx = m_rbData.posx;
    data.posy = m_rbData.posy;
    data.posz = m_rbData.posz;
    data.posxT = m_rbData.posxT;
    data.posyT = m_rbData.posyT;
    data.poszT = m_rbData.poszT;
    data.yaw = m_rbData.yaw;
    data.pitch = m_rbData.pitch;
    data.roll = m_rbData.roll;*/
}

void Optitrack::setTransformToTip(bool status)
{
    m_transFormToTip = status;
}

void Optitrack::run()
{
    char  szData[20000];
    int addr_len = sizeof(struct sockaddr);
    sockaddr_in TheirAddress;

    m_isRunning = true;

    QPC timer;
    double oldTime = timer.getms();

    double loopDuration = 0;

    while(m_isRunning)
    {
        if(timer.getms() - oldTime < 10.0 - loopDuration)
            continue;
        timer.tic();
        m_isUpdating = true;

        //m_rbData.tracked = false;

        int nDataBytesReceived = recvfrom(m_dataSocket, szData, sizeof(szData), 0, (sockaddr *)&TheirAddress, &addr_len);
        if(nDataBytesReceived > 0)
        {
            unpackFrame(szData);
        }

        //std::cout << "pos " << m_rbData.posx << " " << m_rbData.posy << " " << m_rbData.posz << std::endl;
        //std::cout << "error " << m_rbData.meanError << std::endl;

        if (m_onCalibrate && m_rbData.meanError < 0.0003 && m_rbData.tracked)
        {
            m_positionsX[m_calibIndice] = m_rbData.posx;
            m_positionsY[m_calibIndice] = m_rbData.posy;
            m_positionsZ[m_calibIndice] = m_rbData.posz;
            m_yaws[m_calibIndice] = m_rbData.yaw;
            m_pitchs[m_calibIndice] = m_rbData.pitch;
            m_rolls[m_calibIndice] = m_rbData.roll;

            m_calibIndice++;

            int pcentage =  round(float((m_calibIndice*1.0)/CALIB_NB)*100);
            emit(calibrating(pcentage));

            if(m_calibIndice >= CALIB_NB)
            {
                m_onCalibrate = false;
                computeTipMatrix();
            }
        }

        if(m_transFormToTip && m_rbData.tracked)
        {
            std::cout << "tip" << std::endl;
            MatrixXd tipPos(4,4);
            //VectorXd tipXYZ(3);
            VectorXd tipTransformed(3);

            tipPos = MatrixXd::Identity(4,4);

            float cosa = cos(m_rbData.yaw);
            float sina = sin(m_rbData.yaw);
            float cosb = cos(m_rbData.pitch);
            float sinb = sin(m_rbData.pitch);
            float cosg = cos(m_rbData.roll);
            float sing = sin(m_rbData.roll);

            MatrixXd Rx(3,3); //pitch
            MatrixXd Ry(3,3); //yaw
            MatrixXd Rz(3,3); //roll

            Rx(0,0) = 1 ; Rx(0,1) = 0    ; Rx(0,2) = 0     ;
            Rx(1,0) = 0 ; Rx(1,1) = cosb ; Rx(1,2) = -sinb ;
            Rx(2,0) = 0 ; Rx(2,1) = sinb ; Rx(2,2) = cosb  ;

            Ry(0,0) = cosa  ; Ry(0,1) = 0 ; Ry(0,2) = sina  ;
            Ry(1,0) = 0     ; Ry(1,1) = 1 ; Ry(1,2) = 0     ;
            Ry(2,0) = -sina ; Ry(2,1) = 0 ; Ry(2,2) = cosa  ;

            Rz(0,0) = cosg ; Rz(0,1) =-sing ; Rz(0,2) = 0  ;
            Rz(1,0) = sing ; Rz(1,1) = cosg ; Rz(1,2) = 0  ;
            Rz(2,0) = 0    ; Rz(2,1) = 0    ; Rz(2,2) = 1  ;

            MatrixXd M(3,3);
            M = Rx*Ry*Rz;

            //tipXYZ << m_rbData.posx, m_rbData.posy, m_rbData.posz;

            tipPos.block<3,3>(0,0) = M;
            tipPos(0,3) = m_rbData.posx;
            tipPos(1,3) = m_rbData.posy;
            tipPos(2,3) = m_rbData.posz;
            //tipPos.block<3,1>(3,0) = tipXYZ;

            tipTransformed << 0, 0, 0;

            tipTransformed = (tipPos*m_tipTransform).block<3,1>(0,0);//tipPos*m_tipTransform;

            m_rbData.posxT = tipTransformed[0];//tipTransformed[0];//x+ m_tipTransform[0];
            m_rbData.posyT = tipTransformed[1];//tipTransformed[1];//y+ m_tipTransform[1];
            m_rbData.poszT = tipTransformed[2];//tipTransformed[2];//z+ m_tipTransform[2];
        }


        m_isUpdating = false;

        loopDuration = timer.toc()*1000.0;

    }
}

void Optitrack::computeTipMatrix()
{
    int indice = 0;

    MatrixXd A(3*CALIB_NB,6);

    VectorXd b(3*CALIB_NB);

    MatrixXd I = -MatrixXd::Identity(3,3);

    for(unsigned int i=0; i< CALIB_NB; i++)
    {
        // calcul matrice rotation M
        float cosa = cos(m_yaws[i]);
        float sina = sin(m_yaws[i]);
        float cosb = cos(m_pitchs[i]);
        float sinb = sin(m_pitchs[i]);
        float cosg = cos(m_rolls[i]);
        float sing = sin(m_rolls[i]);

        /*MatrixXd M(3,3);
        M(0,0) = cosa*cosb ; M(0,1) = cosa*sinb*sing-sina*cosg ; M(0,2) = cosa*sinb*cosg+sina*sing ;
        M(1,0) = sina*cosb ; M(1,1) = sina*sinb*sing+cosa*cosg ; M(1,2) = sina*sinb*cosg-cosa*sing ;
        M(2,0) = -sinb     ; M(2,1) = cosb*sing                ; M(2,2) = cosb*cosg                ;*/

        MatrixXd Rx(3,3); //pitch
        MatrixXd Ry(3,3); //yaw
        MatrixXd Rz(3,3); //roll

        Rx(0,0) = 1 ; Rx(0,1) = 0    ; Rx(0,2) = 0     ;
        Rx(1,0) = 0 ; Rx(1,1) = cosb ; Rx(1,2) = -sinb ;
        Rx(2,0) = 0 ; Rx(2,1) = sinb ; Rx(2,2) = cosb  ;

        Ry(0,0) = cosa  ; Ry(0,1) = 0 ; Ry(0,2) = sina  ;
        Ry(1,0) = 0     ; Ry(1,1) = 1 ; Ry(1,2) = 0     ;
        Ry(2,0) = -sina ; Ry(2,1) = 0 ; Ry(2,2) = cosa  ;

        Rz(0,0) = cosg ; Rz(0,1) =-sing ; Rz(0,2) = 0  ;
        Rz(1,0) = sing ; Rz(1,1) = cosg ; Rz(1,2) = 0  ;
        Rz(2,0) = 0    ; Rz(2,1) = 0    ; Rz(2,2) = 1  ;

        MatrixXd M(3,3);
        M = Rx*Ry*Rz;

        // remplissage de A avec [M -identity(3)]
        A.block<3,3>(indice,0) = M;
        A.block<3,3>(indice,3) = I;

        // remplissage de b = -coordXYZ

        b(indice,0) = -m_positionsX[i];
        b(indice+1,0) = -m_positionsY[i];
        b(indice+2,0) = -m_positionsZ[i];

        indice += 3;
    }

    // solution = pinv(A)*b

    MatrixXd pinvA = A.completeOrthogonalDecomposition().pseudoInverse();

    MatrixXd solution = pinvA * b;

    m_tipTransform[0] = solution(0,0);
    m_tipTransform[1] = solution(1,0);
    m_tipTransform[2] = solution(2,0);

    //std::cout << m_tipTransform<< std::endl;

    m_tipTransformComputed = true;

    //std::cout << "solution : " << solution << std::endl;

    //std::cout << b << std::endl;
}

bool Optitrack::connect()
{
    if(m_isConnected)
        return true;

    in_addr MyAddress;
    in_addr MultiCastAddress;
    MyAddress.S_un.S_addr = inet_addr("127.0.0.1");
    MultiCastAddress.S_un.S_addr = inet_addr(MULTICAST_ADDRESS);

    // create a "Data" socket
    m_dataSocket = socket(AF_INET, SOCK_DGRAM, 0);
    // allow multiple clients on same machine to use address/port
    int value = 1;
    int retval = setsockopt(m_dataSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value));
    if (retval == SOCKET_ERROR)
    {
        closesocket(m_dataSocket);
        m_isConnected = false;
        return false;
    }

    struct sockaddr_in MySocketAddr;
    memset(&MySocketAddr, 0, sizeof(MySocketAddr));
    MySocketAddr.sin_family = AF_INET;
    MySocketAddr.sin_port = htons(PORT_DATA);
    MySocketAddr.sin_addr = MyAddress;
    if (bind(m_dataSocket, (struct sockaddr *)&MySocketAddr, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        std::cout << "Optitrack error: Unable to bind data socket" << std::endl;
        WSACleanup();
        m_isConnected = false;
        return false;
    }

    // join multicast group
    struct ip_mreq Mreq;
    Mreq.imr_multiaddr = MultiCastAddress;
    Mreq.imr_interface = MyAddress;
    retval = setsockopt(m_dataSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&Mreq, sizeof(Mreq));
    if (retval == SOCKET_ERROR)
    {
        std::cout << "Optitrack error: Unable to join multicast group" << std::endl;
        WSACleanup();
        m_isConnected = false;
        return false;
    }
    // create a 1MB buffer
    int optval = 0x100000;
    int optval_size = 4;
    setsockopt(m_dataSocket, SOL_SOCKET, SO_RCVBUF, (char *)&optval, 4);
    getsockopt(m_dataSocket, SOL_SOCKET, SO_RCVBUF, (char *)&optval, &optval_size);

    int timeoutinms = 10;
    setsockopt(m_dataSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeoutinms, 4);

    m_isConnected = true;
    return true;
}

bool Optitrack::timecodeStringify(unsigned int inTimecode, unsigned int inTimecodeSubframe, char *Buffer, int BufferSize)
{
    int hour, minute, second, frame, subframe;

    bool bValid = true;

    hour = (inTimecode>>24)&255;
    minute = (inTimecode>>16)&255;
    second = (inTimecode>>8)&255;
    frame = inTimecode&255;
    subframe = inTimecodeSubframe;

    sprintf_s(Buffer,BufferSize,"%2d:%2d:%2d:%2d.%d",hour, minute, second, frame, subframe);
    for(unsigned int i=0; i<strlen(Buffer); i++)
        if(Buffer[i]==' ')
            Buffer[i]='0';

    return bValid;
}

void Optitrack::decodeMarkerID(int sourceID, int* pOutEntityID, int* pOutMemberID)
{
    if (pOutEntityID)
        *pOutEntityID = sourceID >> 16;

    if (pOutMemberID)
        *pOutMemberID = sourceID & 0x0000ffff;
}

void Optitrack::unpackFrame(char* pData)
{
    char *ptr = pData;

    //printf("Begin Packet\n-------\n");

    // First 2 Bytes is message ID
    int MessageID = 0;
    memcpy(&MessageID, ptr, 2); ptr += 2;
    //printf("Message ID : %d\n", MessageID);

    // Second 2 Bytes is the size of the packet
    int nBytes = 0;
    memcpy(&nBytes, ptr, 2); ptr += 2;
    //printf("Byte count : %d\n", nBytes);

    if(MessageID == 7)      // FRAME OF MOCAP DATA packet
    {
        // Next 4 Bytes is the frame number
        int frameNumber = 0; memcpy(&frameNumber, ptr, 4); ptr += 4;
        //printf("Frame # : %d\n", frameNumber);

        // Next 4 Bytes is the number of data sets (markersets, rigidbodies, etc)
        int nMarkerSets = 0; memcpy(&nMarkerSets, ptr, 4); ptr += 4;
        //printf("Marker Set Count : %d\n", nMarkerSets);

        // Loop through number of marker sets and get name and data
        for (int i=0; i < nMarkerSets; i++)
        {
            // Markerset name
            char szName[256];
            strcpy_s(szName, ptr);
            int nDataBytes = (int) strlen(szName) + 1;
            ptr += nDataBytes;
            //printf("Model Name: %s\n", szName);

            // marker data
            int nMarkers = 0; memcpy(&nMarkers, ptr, 4); ptr += 4;
            //printf("Marker Count : %d\n", nMarkers);

            for(int j=0; j < nMarkers; j++)
            {
                float x = 0; memcpy(&x, ptr, 4); ptr += 4;
                float y = 0; memcpy(&y, ptr, 4); ptr += 4;
                float z = 0; memcpy(&z, ptr, 4); ptr += 4;
                //printf("\tMarker %d : [x=%3.2f,y=%3.2f,z=%3.2f]\n",j,x,y,z);
            }
        }

        // Loop through unlabeled markers
        int nOtherMarkers = 0; memcpy(&nOtherMarkers, ptr, 4); ptr += 4;
        // OtherMarker list is Deprecated
        //printf("Unidentified Marker Count : %d\n", nOtherMarkers);
        for(int j=0; j < nOtherMarkers; j++)
        {
            float x = 0.0f; memcpy(&x, ptr, 4); ptr += 4;
            float y = 0.0f; memcpy(&y, ptr, 4); ptr += 4;
            float z = 0.0f; memcpy(&z, ptr, 4); ptr += 4;

            // Deprecated
            //printf("\tMarker %d : pos = [%3.2f,%3.2f,%3.2f]\n",j,x,y,z);
        }

        // Loop through rigidbodies
        int nRigidBodies = 0;
        memcpy(&nRigidBodies, ptr, 4); ptr += 4;
        //std::cout << "rb: " << nRigidBodies << std::endl;
        for (int j=0; j < nRigidBodies; j++)
        {
            // Rigid body position and orientation
            memcpy(&m_rbData.ID, ptr, 4); ptr += 4;
            memcpy(&m_rbData.posx, ptr, 4); ptr += 4;
            memcpy(&m_rbData.posy, ptr, 4); ptr += 4;
            memcpy(&m_rbData.posz, ptr, 4); ptr += 4;
            memcpy(&m_rbData.qx, ptr, 4); ptr += 4;
            memcpy(&m_rbData.qy, ptr, 4); ptr += 4;
            memcpy(&m_rbData.qz, ptr, 4); ptr += 4;
            memcpy(&m_rbData.qw, ptr, 4); ptr += 4;
            if(m_firstFrame)
            {
                m_rbData.oldPosx = m_rbData.posx;
                m_rbData.oldPosy = m_rbData.posy;
                m_rbData.oldPosz = m_rbData.posz;
                m_firstFrame = false;
            }
            else
            {
                if(m_rbData.oldPosx == m_rbData.posx &&
                        m_rbData.oldPosy == m_rbData.posy &&
                        m_rbData.oldPosz == m_rbData.posz)
                {
                    m_rbData.tracked = false;
                }
                else
                {
                    m_rbData.tracked = true;
                }
                m_rbData.oldPosx = m_rbData.posx;
                m_rbData.oldPosy = m_rbData.posy;
                m_rbData.oldPosz = m_rbData.posz;
            }

            int order = EulOrdXYZr;
            Quat q;
            q.w = m_rbData.qw;
            q.x = m_rbData.qx;
            q.y = m_rbData.qy;
            q.z = m_rbData.qz;
            EulerAngles ea = Eul_FromQuat(q, order);
            m_rbData.yaw = ea.y;
            m_rbData.pitch = ea.x;
            m_rbData.roll = ea.z;

            memcpy(&m_rbData.meanError, ptr, 4); ptr += 4;
            /*short params = 0; memcpy(&params, ptr, 2); ptr += 2;
            m_rbData.tracked = params & 0x01; // 0x01 : rigid body was successfully tracked in this frame*/

        } // Go to next rigid body


        // Skeletons (NatNet version 2.1 and later)
        /*if( ((major == 2)&&(minor>0)) || (major>2))
        {
            int nSkeletons = 0;
            memcpy(&nSkeletons, ptr, 4); ptr += 4;
            printf("Skeleton Count : %d\n", nSkeletons);

            // Loop through skeletons
            for (int j=0; j < nSkeletons; j++)
            {
                // skeleton id
                int skeletonID = 0;
                memcpy(&skeletonID, ptr, 4); ptr += 4;

                // Number of rigid bodies (bones) in skeleton
                int nRigidBodies = 0;
                memcpy(&nRigidBodies, ptr, 4); ptr += 4;
                printf("Rigid Body Count : %d\n", nRigidBodies);

                // Loop through rigid bodies (bones) in skeleton
                for (int j=0; j < nRigidBodies; j++)
                {
                    // Rigid body position and orientation
                    int ID = 0; memcpy(&ID, ptr, 4); ptr += 4;
                    float x = 0.0f; memcpy(&x, ptr, 4); ptr += 4;
                    float y = 0.0f; memcpy(&y, ptr, 4); ptr += 4;
                    float z = 0.0f; memcpy(&z, ptr, 4); ptr += 4;
                    float qx = 0; memcpy(&qx, ptr, 4); ptr += 4;
                    float qy = 0; memcpy(&qy, ptr, 4); ptr += 4;
                    float qz = 0; memcpy(&qz, ptr, 4); ptr += 4;
                    float qw = 0; memcpy(&qw, ptr, 4); ptr += 4;
                    printf("ID : %d\n", ID);
                    printf("pos: [%3.2f,%3.2f,%3.2f]\n", x,y,z);
                    printf("ori: [%3.2f,%3.2f,%3.2f,%3.2f]\n", qx,qy,qz,qw);

                    float fError = 0.0f; memcpy(&fError, ptr, 4); ptr += 4;
                    short params = 0; memcpy(&params, ptr, 2); ptr += 2;
                    //bool bTrackingValid = params & 0x01; // 0x01 : rigid body was successfully tracked in this frame
                } // next rigid body

            } // next skeleton
        }

        // labeled markers (NatNet version 2.3 and later)
        // labeled markers - this includes all markers: Active, Passive, and 'unlabeled' (markers with no asset but a PointCloud ID)
        if( ((major == 2)&&(minor>=3)) || (major>2))
        {
            int nLabeledMarkers = 0;
            memcpy(&nLabeledMarkers, ptr, 4); ptr += 4;
            printf("Labeled Marker Count : %d\n", nLabeledMarkers);

            // Loop through labeled markers
            for (int j=0; j < nLabeledMarkers; j++)
            {
                // id
                // Marker ID Scheme:
                // Active Markers:
                //   ID = ActiveID, correlates to RB ActiveLabels list
                // Passive Markers:
                //   If Asset with Legacy Labels
                //      AssetID 	(Hi Word)
                //      MemberID	(Lo Word)
                //   Else
                //      PointCloud ID
                int ID = 0; memcpy(&ID, ptr, 4); ptr += 4;
                int modelID, markerID;
                decodeMarkerID(ID, &modelID, &markerID);


                // x
                float x = 0.0f; memcpy(&x, ptr, 4); ptr += 4;
                // y
                float y = 0.0f; memcpy(&y, ptr, 4); ptr += 4;
                // z
                float z = 0.0f; memcpy(&z, ptr, 4); ptr += 4;
                // size
                float size = 0.0f; memcpy(&size, ptr, 4); ptr += 4;
            }
        }

        int nForcePlates;
        memcpy(&nForcePlates, ptr, 4); ptr += 4;
        for (int iForcePlate = 0; iForcePlate < nForcePlates; iForcePlate++)
        {
            // ID
            int ID = 0; memcpy(&ID, ptr, 4); ptr += 4;

            // Channel Count
            int nChannels = 0; memcpy(&nChannels, ptr, 4); ptr += 4;

            // Channel Data
            for (int i = 0; i < nChannels; i++)
            {
                int nFrames = 0; memcpy(&nFrames, ptr, 4); ptr += 4;
                for (int j = 0; j < nFrames; j++)
                {
                    float val = 0.0f;  memcpy(&val, ptr, 4); ptr += 4;
                }
                printf("\n");
            }
        }

        float softwareLatency = 0.0f; memcpy(&softwareLatency, ptr, 4);	ptr += 4;

        // timecode
        unsigned int timecode = 0; 	memcpy(&timecode, ptr, 4);	ptr += 4;
        unsigned int timecodeSub = 0; memcpy(&timecodeSub, ptr, 4); ptr += 4;
        char szTimecode[128] = "";
        timecodeStringify(timecode, timecodeSub, szTimecode, 128);

        // end of data tag
        int eod = 0; memcpy(&eod, ptr, 4); ptr += 4;*/
    }
    /*else
    {
        printf("Unrecognized Packet Type.\n");
    }*/
}
