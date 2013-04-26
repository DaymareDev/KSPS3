#include "saveformater.h"
#include "vesseldata.h"

#include <QFile>
#include <QDir>
#include <sstream>
#include <iostream>


namespace KSPS3
{

SaveFormater::SaveFormater(QString saveFilePath, QObject *parent) :
    m_fromFilePath(saveFilePath),
    QObject(parent)
{

}

void SaveFormater::GetVesselManifests(std::vector<VesselData*> &vesselList)
{
    QMutexLocker lock(&m_shipsMutex);
    std::map<QString, VesselData*>::iterator it =  m_vessels.begin();
    while(it != m_vessels.end())
    {
        vesselList.push_back((*it).second);
        it++;
    }
}

void SaveFormater::AddVessel(const VesselData* const toAdd)
{
   addVesselData(toAdd->GetPID(), toAdd->GetName(), *(toAdd->AccessFullText()));
}

QString SaveFormater::GetPath()
{
    return m_fromFilePath;
}

void SaveFormater::addVesselData(const QString& vesselPID, const QString& vesselName, const QString& fullVesselData)
{
    QMutexLocker lock(&m_shipsMutex);
    VesselData* vessel = new VesselData(vesselPID,vesselName, fullVesselData);
    m_vessels.insert(std::pair<QString, VesselData*>(vesselPID,vessel));
}


void SaveFormater::isolateVessels(const QString &saveData)
{
    QString vesselKey = QString("VESSEL");
    QString actionGroupKey("ACTIONGROUPS");
    int startOfVessel = saveData.indexOf(vesselKey);
    int endOfVessel = saveData.indexOf(vesselKey, startOfVessel + vesselKey.length());
    int numberOfVessels = 0;
    while(startOfVessel != -1 && startOfVessel < saveData.length())
    {
        std::ostringstream progressLogStream;
        QString vesselText = saveData.mid(startOfVessel, endOfVessel-startOfVessel);
        int pidLocation = vesselText.indexOf("pid = ") + 6;
        QString vesselPID = vesselText.mid(pidLocation, vesselText.indexOf('\n',pidLocation) - pidLocation);
        int nameLocation = vesselText.indexOf("name = ") + 7;
        QString vesselName = vesselText.mid(nameLocation, vesselText.indexOf('\n', nameLocation) - nameLocation);

        addVesselData(vesselPID, vesselName, vesselText);

        ++numberOfVessels;

        progressLogStream << "Done isolating vessel " << vesselName.toStdString()
                          << " with pid: " << vesselPID.toStdString() << std::endl;

        createLogMessage(QString(progressLogStream.str().c_str()));

        startOfVessel = saveData.indexOf(vesselKey, endOfVessel-1);
        endOfVessel = saveData.indexOf(vesselKey, startOfVessel + vesselKey.length());
        if(endOfVessel == -1)
        {
            endOfVessel = saveData.indexOf(actionGroupKey, startOfVessel + vesselKey.length());
            endOfVessel = saveData.indexOf('}', endOfVessel+1);
            endOfVessel = saveData.indexOf('}', endOfVessel+1);
        }
    }
    std::ostringstream completionLogStream;
    completionLogStream << "Done isolating " << numberOfVessels << " vessels.";
    createLogMessage(QString(completionLogStream.str().c_str()));

}

void SaveFormater::CreateKSPS3File()
{
    QString backupPath(m_fromFilePath);
    backupPath.append(QString(".backup"));
    readOriginalSave(m_fromFilePath);
    createBackup(m_originalSave, backupPath);

    isolateVessels(m_originalSave);

}

void SaveFormater::readOriginalSave(const QString &fromPath)
{
    QString logMessage("Reading file from ");
    logMessage.append(fromPath);
    createLogMessage(logMessage);
    QFile original(fromPath);
    if(!original.exists())
    {
        QString error("Unable to find the file ");
        error.append(fromPath);
        return;
    }
    if(!original.open(QIODevice::ReadOnly))
    {
        QString error("Unable to open the file ");
        error.append(fromPath);
        createLogMessage(error);
        return;
    }
    m_originalSave = QString(original.readAll());
    original.close();
    logMessage = QString("Successfully read savefile from harddrive..");
    createLogMessage(logMessage);
}

void SaveFormater::createBackup(const QString& dataToWrite, const QString& toPath)
{
    QFile to(toPath);


    if(!to.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        QString error("Unable to open the file ");
        error.append(toPath);
        createLogMessage(error);
        return;
    }
    to.write(dataToWrite.toLocal8Bit());
    to.close();
    QString logSuccess("Succesfully created backup file at ");
    logSuccess.append(toPath);
    createLogMessage(logSuccess);
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
