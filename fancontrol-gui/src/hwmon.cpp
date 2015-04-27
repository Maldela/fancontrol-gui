#include "hwmon.h"
#include <QDir>
#include <QTextStream>
#include <QDebug>

Hwmon::Hwmon(const QString &path) : QObject()
{
    m_path = path;
    m_index = path.split('/').last().remove("hwmon").toInt();
    QFile nameFile(path + "/name");
    if (nameFile.open(QFile::ReadOnly))
        m_name = QTextStream(&nameFile).readLine();
    else
        m_name = "Nameless hwmon";
    emit pathChanged();

    QDir dir(m_path);
    QStringList entrys = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (QString entry, entrys)
    {
        QString str = entry;
        int index = str.remove(QRegExp("\\D+")).toInt();
        if (entry.contains("fan") && entry.contains("input"))
        {
            if (QFile::exists(m_path + "/pwm" + QString::number(index)))
            {
                PwmFan *newPwmFan = new PwmFan(this, index);
                newPwmFan->setName(m_name + "/pwm" + QString::number(index));
                connect(this, SIGNAL(sensorsUpdateNeeded()), newPwmFan, SLOT(update()));
                m_pwmFans << newPwmFan;
                emit pwmFansChanged();
                m_fans << qobject_cast<Fan *>(newPwmFan);
                emit fansChanged();
            }
            else
            {
                Fan *newFan = new Fan(this, index);
                connect(this, SIGNAL(sensorsUpdateNeeded()), newFan, SLOT(update()));
                m_fans << newFan;
                emit fansChanged();
            }
        }

        if (entry.contains("temp") && entry.contains("input"))
        {
            Temp *newTemp = new Temp(this, index);
            newTemp->setName(m_name + "/" + newTemp->label());
            connect(this, SIGNAL(sensorsUpdateNeeded()), newTemp, SLOT(update()));
            m_temps << newTemp;
            emit tempsChanged();
        }
    }
//    qDebug() << "New Hwmon" << m_temps.size() << m_pwmFans.size();
}

QList<QObject *> Hwmon::fans() const
{
    QList<QObject *> list;
    foreach (Fan *fan, m_fans)
    {
        list << qobject_cast<QObject *>(fan);
    }
    return list;
}

QList<QObject *> Hwmon::pwmFans() const
{
    QList<QObject *> list;
    foreach (PwmFan *pwmFan, m_pwmFans)
    {
        list << qobject_cast<QObject *>(pwmFan);
    }
    return list;
}

QList<QObject *> Hwmon::temps() const
{
    QList<QObject *> list;
    foreach (Temp *temp, m_temps)
    {
        list << qobject_cast<QObject *>(temp);
    }
    return list;
}
