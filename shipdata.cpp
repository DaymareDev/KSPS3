#include "shipdata.h"
namespace KSPS3
{

ShipData::ShipData(const QString& PID, const QString& name, const QString& fullShipText)
    : m_pid(PID), m_name(name), m_fullShipText(fullShipText)
{
}

QString ShipData::GetPID() const
{
    return m_pid;
}
QString ShipData::GetName() const
{
    return m_name;
}
const QString* ShipData::AccessFullText(){return 0;}


}
