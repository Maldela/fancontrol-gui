#include "sensors.h"
#include <QFile>
#include <QDir>

Sensor::Sensor(Hwmon *parent, uint index) : QObject(parent)
{
    m_parent = parent;
    m_index = index;
    emit indexChanged();
}

Fan::Fan(Hwmon *parent, uint index) : Sensor(parent, index)
{
    if (QDir(parent->path()).isReadable())
    {
        QFile *rpmFile = new QFile(parent->path() + "/fan" + QString::number(index) + "_input", this);

        if (rpmFile->open(QFile::ReadOnly))
        {
            m_rpmStream.setDevice(rpmFile);
            m_rpmStream >> m_rpm;
            emit rpmChanged();

            setName("fan"+ QString::number(index));
        }
        else
        {
            qDebug() << "Can't open rpmFile " << parent->path() + "/fan" + QString::number(index) + "_input";
        }
    }
}

void Fan::update()
{
    m_rpmStream.seek(0);
    m_rpmStream >> m_rpm;
    emit rpmChanged();
}



PwmFan::PwmFan(Hwmon *parent, uint index) : Fan(parent, index)
{
    m_temp = nullptr;
    m_hasTemp = false;
    m_minTemp = 0;
    m_maxTemp = 100;
    m_minPwm = 255;
    m_maxPwm = 255;
    m_minStart = 255;
    m_minStop = 255;

    connect(this, SIGNAL(tempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(hasTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(maxTempChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minPwmChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(maxPwmChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minStartChanged()), parent, SLOT(updateConfig()));
    connect(this, SIGNAL(minStopChanged()), parent, SLOT(updateConfig()));

    if (QDir(parent->path()).isReadable())
    {
        QFile *pwmFile = new QFile(parent->path() + "/pwm" + QString::number(index), this);

        if (pwmFile->open(QFile::ReadOnly)) //TODO ReadWrite with su rights
        {
            m_pwmStream.setDevice(pwmFile);
            m_pwmStream >> m_pwm;
            emit pwmChanged();
        }
        else
        {
            qDebug() << "Can't open pwmFile " << parent->path() + "/pwm" + QString::number(index);
        }
    }
}

void PwmFan::update()
{
    Fan::update();
    m_pwmStream.seek(0);
    m_pwmStream >> m_pwm;
    emit pwmChanged();
}

void PwmFan::writePwm()
{
    m_pwmStream << m_pwm;
}

void PwmFan::reset()
{
    setTemp(nullptr);
    setMinTemp(0);
    setMaxTemp(100);
    setMinPwm(255);
    setMaxPwm(255);
    setMinStart(255);
    setMinStop(255);
}


Temp::Temp(Hwmon *parent, uint index) : Sensor(parent, index)
{
    if (QDir(parent->path()).isReadable())
    {
        QFile *valueFile = new QFile(parent->path() + "/temp" + QString::number(index) + "_input", this);
        QFile labelFile(parent->path() + "/temp" + QString::number(index) + "_label");

        if (valueFile->open(QFile::ReadOnly))
        {
            m_valueStream.setDevice(valueFile);
            m_valueStream >> m_value;
            m_value /= 1000;
            emit valueChanged();
        }
        else
            qDebug() << "Can't open valueFile " << parent->path() + "/temp" + QString::number(index) + "_input";

        if (labelFile.open(QFile::ReadOnly))
        {
            m_label = QTextStream(&labelFile).readLine();
        }
        else
        {
            m_label = "temp" + QString::number(index);
            qDebug() << "Can't open labelFile " << parent->path() + "/temp" + QString::number(index) + "_label";
        }
        emit labelChanged();
    }
}

void Temp::update()
{
    m_valueStream.seek(0);
    m_valueStream >> m_value;
    m_value /= 1000;
    emit valueChanged();
}
