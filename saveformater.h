#ifndef SAVEFORMATER_H
#define SAVEFORMATER_H

#include <QObject>
#include <QString>
#include <QMutex>

#include <queue>
#include <map>


namespace KSPS3
{

class VesselData;

class SaveFormater : public QObject
{
    Q_OBJECT
public:
    explicit SaveFormater(QString saveFilePath, QObject *parent = 0);
    void CreateKSPS3File();

private:
    QString m_fromFilePath;
    QString m_toFilePath;
    QString m_originalSave;

    QMutex m_logMutex;
    std::queue<QString> m_logMessages;
    std::map<QString, VesselData*> m_ships;

    volatile bool m_isDone;

    void readOriginalSave(const QString& fromPath);
    void createBackup(const QString& fullText, const QString& toPath);
    void createLogMessage(const QString& message);
    void isolateVessels(const QString &saveData, std::map<QString, VesselData*> &toPopulate);


signals:
    
public slots:


    QString GetMessage();
    bool HasMessage();
    bool IsDone() const;
    
};

}
#endif // SAVEFORMATER_H
