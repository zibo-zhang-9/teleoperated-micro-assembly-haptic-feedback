#ifndef MUTWEEZER_H
#define MUTWEEZER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QElapsedTimer>

class MuTweezer : public QThread
{
    Q_OBJECT

public:
    MuTweezer();
    ~MuTweezer();

    void run();

    void stop();

    QString getData();
    void sendData(QByteArray data);

    bool connectTo(QString IP, int rcvPort, int sndPort = 1234);

    bool isRunning();
    bool isConnected();



private:

    bool m_isRunning;
    bool m_newDataToSend;

    QString m_readData;
    QByteArray m_writeData;

    QString m_IP;
    int m_port;

    bool m_isConnected;

    QHostAddress m_hostAddr;
    QUdpSocket m_imuSndSocket;
    QUdpSocket m_imuRcvSocket;

    quint16 m_portSnd;
    quint16 m_portRcv;

    QElapsedTimer t;

signals:
    void dataReceived();

public slots:
    //void processDataReceived();
    void sendMessage(QString m);
    void recu();
};

#endif // MUTWEEZER_H
