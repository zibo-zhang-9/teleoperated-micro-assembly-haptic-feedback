#include "mutweezer.h"
#include "qpc.h"
#include <QNetworkProxy>

MuTweezer::MuTweezer()
{
    m_newDataToSend = false;
    m_isRunning = false;
    m_isConnected = false;

    WSADATA WsaData;
    if (WSAStartup(0x202, &WsaData) == SOCKET_ERROR)
    {
        std::cout << "MuTweezer error: Unable to start WSA" << std::endl;
        WSACleanup();
    }
}

MuTweezer::~MuTweezer()
{
    m_isRunning = false;
    stop();
}

void MuTweezer::recu(){
    std::cout << "recu" << std::endl;
}
bool MuTweezer::connectTo(QString IP, int rcvPort, int sndPort)
{
    m_port = rcvPort;

    m_hostAddr = QHostAddress::LocalHost;
    m_portRcv = rcvPort;
    m_portSnd = sndPort;
    m_IP = IP;

    if(!m_imuRcvSocket.bind(m_hostAddr, rcvPort))
   {
       std::cout << "Unable to bind to rcv port!" << std::endl;
       return false;
   }
    if(!m_imuSndSocket.bind(m_hostAddr, sndPort))
    {
        std::cout << "Unable to bind to snd port!" << std::endl;
        return false;
    }
    connect(&m_imuRcvSocket, SIGNAL(readyRead()), this, SLOT(recu()));
    m_isConnected = true;

    return true;
}

void MuTweezer::stop()
{
    m_isRunning = false;
    m_imuRcvSocket.close();
    m_imuSndSocket.close();
    QThread::msleep(2);
    //closesocket(m_tcpSocket);
}

bool MuTweezer::isRunning()
{
    return m_isRunning;
}

bool MuTweezer::isConnected()
{
    return m_isConnected;
}

QString MuTweezer::getData()
{
    /*
    QByteArray rcvData = m_imuRcvSocket.read(32);
    m_readData = QString::fromUtf8(rcvData);
    qDebug() << "m_readData :" << m_readData << endl;
    */
    /*
    if(m_imuRcvSocket.hasPendingDatagrams())
    {
        int pendingBytes = m_imuRcvSocket.pendingDatagramSize();

        if(pendingBytes == 0)
            return m_readData;


        QByteArray rcvData;
        rcvData.resize(pendingBytes);
        m_imuRcvSocket.readDatagram(rcvData.data(),rcvData.size());
        // m_readData = QString::fromUtf8(rcvData);
        std::cout << "rcvData.toStdString()" << rcvData.toStdString()<< std::endl;
    }
    */
    return m_readData;
}

void MuTweezer::sendData(QByteArray data)
{
    std::cout << "data.toStdString() 1: " << data.toStdString() << std::endl;
    // m_socket.write(data.toLatin1());
    // m_socket.flush();
    m_imuSndSocket.write(data);
    m_imuSndSocket.flush();

//    qint64 number = m_imuSndSocket.writeDatagram(data,m_hostAddr,m_portSnd);
//    std::cout << "writeDatagram: " << number << std::endl;

    qint64 number3 = m_imuSndSocket.writeDatagram((QString::number(44)).toStdString().c_str(),(QString::number(44)).size(),QHostAddress("200.0.0.3"),m_portSnd);
    std::cout << "writeDatagram : " << number3 << std::endl;

    // qint64 number2 = m_imuSndSocket.write(data);
    // std::cout << "write : " << number2 << std::endl;

    QString strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() << "Local Host: " << strIpAddress << endl;
    // Loacl Host: 127.0.0.1

    qint64 number4 = m_imuSndSocket.writeDatagram(data, QHostAddress::Broadcast,1234);
    std::cout << "writeDatagram : " << number4 << std::endl;

    m_writeData = data;
    m_newDataToSend = true;
}

void MuTweezer::sendMessage(QString m)
{
    if(m[m.size()-1] != ';')
        m.append(';');
    m_imuSndSocket.writeDatagram(m.toStdString().c_str(),m.size(),m_hostAddr,m_portSnd);
}

void MuTweezer::run()
{    

    t.start();

    m_isRunning = true;

    QPC timer;
    double oldTime = timer.getms();

    double loopDuration = 0;

    int cpt = 0;

    double timerAcc = 0;

    while(m_isRunning)
    {
        if(timer.getms() - oldTime < 10.0 - loopDuration)
            continue;
        timer.tic();

        QPC timer2;

        if(m_imuRcvSocket.hasPendingDatagrams())
        {
            // std::cout << "je suis ici" << std::endl;
            int pendingBytes = m_imuRcvSocket.pendingDatagramSize();
            if(pendingBytes == 0)
                continue;

            QByteArray datagram;
            datagram.resize(pendingBytes);
            m_imuRcvSocket.readDatagram(datagram.data(),datagram.size());
            std::cout << "datagram.toStdString()" << datagram.toStdString()<< std::endl;
            std::cout << "pendingBytes :" << pendingBytes << std::endl;
        }
        // std::cout << "je suis la" << std::endl;

        loopDuration = timer.toc()*1000.0;

        timerAcc += timer2.getms();
        cpt++;
    }
    std::cout << "Mean time for " << cpt << " " << (timerAcc/cpt) << std::endl;
    std::cout << "loop duration" << loopDuration << std :: endl;

}

