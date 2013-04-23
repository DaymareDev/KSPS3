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

void SaveFormater::createBackup(const QString& fromPath, const QString& toPath) const
{
    QFile from(fromPath);
    QFile to(toPath);
    if(!from.exists())
    {
        QString error("Unable to find the file ");
        error.append(fromPath);
        throw error;
    }
    if(!from.open(QIODevice::ReadOnly))
    {
        QString error("Unable to open the file ");
        error.append(fromPath);
        throw error;
    }
    if(!to.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        QString error("Unable to open the file ");
        error.append(toPath);
        throw error;
    }
    to.write(from.readAll());
    from.close();
    to.close();
}

QString SaveFormater::GetMessage()
{

    return QString();
}

bool SaveFormater::HasMessage()
{

    return false;
}

bool SaveFormater::IsDone() const
{
    return true;
}

}
