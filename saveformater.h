#ifndef SAVEFORMATER_H
#define SAVEFORMATER_H

#include <QObject>
#include <QString>
#include <QMutex>

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
    QString m_logMessage;

    void createBackup(const QString& fromPath, const QString& toPath) const;

signals:
    
public slots:


    QString GetMessage();
    bool HasMessage();
    bool IsDone() const;
    
};

}
#endif // SAVEFORMATER_H
