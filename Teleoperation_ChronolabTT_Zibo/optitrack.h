#ifndef OPTITRACK_H
#define OPTITRACK_H

#include <QObject>
#include <QThread>

#include <inttypes.h>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <Eigen>

using Eigen::MatrixXd;
using Eigen::VectorXd;

#define MAX_NAMELENGTH              256

// NATNET message ids
#define NAT_CONNECT                 0
#define NAT_SERVERINFO              1
#define NAT_REQUEST                 2
#define NAT_RESPONSE                3
#define NAT_REQUEST_MODELDEF        4
#define NAT_MODELDEF                5
#define NAT_REQUEST_FRAMEOFDATA     6
#define NAT_FRAMEOFDATA             7
#define NAT_MESSAGESTRING           8
#define NAT_UNRECOGNIZED_REQUEST    100
#define UNDEFINED                   999999.9999

#define MAX_PACKETSIZE				100000	// max size of packet (actual packet size is dynamic)

// This should match the multicast address listed in Motive's streaming settings.
#define MULTICAST_ADDRESS		"239.255.42.99"

// NatNet Command channel
#define PORT_COMMAND            1510

// NatNet Data channel
#define PORT_DATA  			    1511

#define CALIB_NB 750

// sender
typedef struct
{
    char szName[MAX_NAMELENGTH];            // sending app's name
    unsigned char Version[4];               // sending app's version [major.minor.build.revision]
    unsigned char NatNetVersion[4];         // sending app's NatNet version [major.minor.build.revision]

} sSender;

typedef struct
{
    unsigned short iMessage;                // message ID (e.g. NAT_FRAMEOFDATA)
    unsigned short nDataBytes;              // Num bytes in payload
    union
    {
        unsigned char  cData[MAX_PACKETSIZE];
        char           szData[MAX_PACKETSIZE];
        unsigned long  lData[MAX_PACKETSIZE/4];
        float          fData[MAX_PACKETSIZE/4];
        sSender        Sender;
    } Data;                                 // Payload incoming from NatNet Server

} sPacket;

//QMutex g_optitrackMutex;
struct optitrack_data
{
    int frameNumber;
    int ID;
    double fTimestamp;
    float posx;
    float posy;
    float posz;
    float posxT;
    float posyT;
    float poszT;
    float oldPosx;
    float oldPosy;
    float oldPosz;
    float yaw;
    float pitch;
    float roll;
    float handRotation;
    float qx;
    float qy;
    float qz;
    float qw;
    bool tracked;
    float meanError;
};

class Optitrack : public QThread
{
    Q_OBJECT
public:
    Optitrack();
    ~Optitrack();

    void run();

    bool isRunning();

    void stop();

    bool isConnected();

    bool isTracked();

    bool isUpdating();

    void setTransformToTip(bool status);

    void calibrate();

    optitrack_data getData();

private:

    void unpackFrame(char* pData);

    bool timecodeStringify(unsigned int inTimecode, unsigned int inTimecodeSubframe, char *Buffer, int BufferSize);

    void decodeMarkerID(int sourceID, int* pOutEntityID, int* pOutMemberID);

    void computeTipMatrix();

    bool m_isRunning;
    bool m_isConnected;
    bool m_isUpdating;
    bool m_transFormToTip;
    bool m_firstFrame;
    bool m_onCalibrate;
    bool m_tipTransformComputed;

    QString m_ipAddress;

    QString m_serverAddress;

    int m_connectionType;

    int m_dataPort;

    int m_commmandPort;

    int m_lastFrameID;

    SOCKET m_dataSocket;

    optitrack_data m_rbData;

    VectorXd m_tipTransform;

    std::vector<float> m_positionsX ;
    std::vector<float> m_positionsY ;
    std::vector<float> m_positionsZ ;
    std::vector<float> m_yaws ;
    std::vector<float> m_pitchs ;
    std::vector<float> m_rolls ;

    unsigned int m_calibIndice;

signals:

    void calibrating(int pcentage);

public slots:
    bool connect();
};

#endif // OPTITRACK_H
