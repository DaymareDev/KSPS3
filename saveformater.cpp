#include "saveformater.h"

#include <QFile>
#include <QDir>



namespace KSPS3
{

SaveFormater::SaveFormater(QString saveFilePath, QObject *parent) :
    m_fromFilePath(saveFilePath),
    QObject(parent)
{

}

void SaveFormater::CreateKSPS3File()
{
    QString backupPath(m_fromFilePath);
    backupPath.append(QString(".backup"));
    createBackup(m_fromFilePath, backupPath);

}

void SaveFormater::createBackup(const QString& fromPath, const QString& toPath)
{
    QFile from(fromPath);
    QFile to(toPath);
    if(!from.exists())
    {
        QString error("Unable to find the file ");
        error.append(fromPath);
        return;
    }
    if(!from.open(QIODevice::ReadOnly))
    {
        QString error("Unable to open the file ");
        error.append(fromPath);
        createLogMessage(error);
        return;
    }
    if(!to.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        QString error("Unable to open the file ");
        error.append(toPath);
        createLogMessage(error);
        from.close();
        return;
    }
    to.write(from.readAll());
    from.close();
    to.close();
}

QString SaveFormater::GetMessage()
{
    QString message;
    m_logMutex.lock();
    if(m_logMessages.empty())
    {
        message = "";
    }
    else
    {
        message = QString(m_logMessages.front());
        m_logMessages.pop();
    }
    m_logMutex.unlock();
    return message;
}

void SaveFormater::createLogMessage(const QString &message)
{
    QMutexLocker locker(&m_logMutex);
    m_logMessages.push(message);
}

bool SaveFormater::HasMessage()
{
    bool hasMessage;
    m_logMutex.lock();
    hasMessage = !m_logMessages.empty();
    m_logMutex.unlock();
    return hasMessage;
}

bool SaveFormater::IsDone() const
{
    return m_isDone;
}

}
