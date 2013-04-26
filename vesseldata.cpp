#include "vesseldata.h"
namespace KSPS3
{

VesselData::VesselData(const QString& PID, const QString& name, const QString& fullShipText)
    : m_pid(PID), m_name(name), m_fullShipText(fullShipText)
{
}

QString VesselData::GetPID() const
{
    return m_pid;
}
QString VesselData::GetName() const
{
    return m_name;
}
const QString* VesselData::AccessFullText() const
{
    return &m_fullShipText;
}


}
