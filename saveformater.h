#ifndef SAVEFORMATER_H
#define SAVEFORMATER_H

#include "shipdata.h"

#include <QObject>
#include <QString>
#include <QMutex>

#include <queue>
#include <map>

namespace KSPS3
{

class SaveFormater : public QObject
{
    Q_OBJECT
public:
    explicit SaveFormater(QString saveFilePath, QObject *parent = 0);
    void CreateKSPS3File();

private:
    QString m_fromFilePath;
    QString m_toFilePath;

    QMutex m_logMutex;
    std::queue<QString> m_logMessages;
    std::map<QString, ShipData> m_ships;

    volatile bool m_isDone;


    void createBackup(const QString& fromPath, const QString& toPath);
    void createLogMessage(const QString& message);


signals:
    
public slots:


    QString GetMessage();
    bool HasMessage();
    bool IsDone() const;
    
};

}
#endif // SAVEFORMATER_H
