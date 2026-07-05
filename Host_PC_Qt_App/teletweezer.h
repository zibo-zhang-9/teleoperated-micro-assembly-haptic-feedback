#ifndef TELETWEEZER_H
#define TELETWEEZER_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QSerialPort>

class Teletweezer : public QThread
{
    Q_OBJECT
public:
    Teletweezer();
    ~Teletweezer();

    void run();

    bool connect(QString name);

    bool isRunning();

    void stop();

    QStringList getData();

    void sendData(QByteArray data);

private:
    QSerialPort m_serialPort;
    bool m_portConnected;
    bool m_isRunning;

    bool m_newDataToSend;

    QMutex m_readMutex;

    QByteArray m_readData;
    QByteArray m_writeData;

signals:

public slots:
};

#endif // TELETWEEZER_H
