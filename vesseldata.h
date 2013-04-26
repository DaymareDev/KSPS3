#ifndef SHIPDATA_H
#define SHIPDATA_H

#include <QString>

namespace KSPS3
{

class VesselData
{

public:
    VesselData(const QString& PID, const QString& name, const QString& fullShipText);
    VesselData();
    QString GetPID() const;
    QString GetName() const;
    const QString* AccessFullText() const;

private:

    QString m_pid;
    QString m_name;
    QString m_fullShipText;

};

}
#endif // SHIPDATA_H
