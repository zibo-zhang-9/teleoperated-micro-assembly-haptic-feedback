#include "teletweezer.h"
#include "qpc.h"

Teletweezer::Teletweezer() : QThread()
{
    m_isRunning = false;

    m_newDataToSend = false;
}

Teletweezer::~Teletweezer()
{
    stop();
}

bool Teletweezer::connect(QString name)
{
    m_serialPort.setPortName(name);
    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setParity(QSerialPort::NoParity);

    bool ret = m_serialPort.open(QIODevice::ReadWrite);

    m_serialPort.setDataTerminalReady(true);
    m_serialPort.setRequestToSend(true);

    return ret;
}

void Teletweezer::stop()
{
    m_isRunning = false;
    QThread::msleep(20);
}

bool Teletweezer::isRunning()
{
    return m_isRunning;
}

QStringList Teletweezer::getData()
{
    QStringList data;
    if(m_readMutex.tryLock(2))
    {
        QByteArray readData = m_readData;
        m_readMutex.unlock();
        if(readData.contains(';'))
        {
            QStringList packets = QString(readData).split(';');

            data = packets[0].split('|');
            // Ouverture de la pince, data[0][0]
            return data;
        }
    }

    return data;
}




void Teletweezer::sendData(QByteArray data)
{
    m_writeData = data;
    m_newDataToSend = true;
}

void Teletweezer::run()
// Boucle run() sert à calculer la fréquence à laquelle tourne le code
{
    m_isRunning = true;

    QPC timer;
    double oldTime = timer.getms();

    double loopDuration = 0;

    int cpt = 0; 

    while(m_isRunning)
    {
        if(timer.getms() - oldTime < 10.0 - loopDuration)
            continue;
        timer.tic();

        //std::cout << timer.getms() << std::endl;

        // read
        m_serialPort.waitForReadyRead(-1);
        m_readMutex.lock();
        m_readData = m_serialPort.readAll();
        m_readMutex.unlock();
        m_serialPort.flush();
        // write
        if(m_newDataToSend)
        {
          int wd = m_serialPort.write(m_writeData);
          m_newDataToSend = false;
        }
        //QByteArray writeData;
        //m_serialPort.write(writeData);

        cpt++;

        loopDuration = timer.toc()*1000.0;
    }

    std::cout << "mean time for " << cpt << " loops: " << timer.getms()/cpt << std::endl;

    m_serialPort.close();
}
