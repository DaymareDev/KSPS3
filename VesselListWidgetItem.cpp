#include "VesselListWidgetItem.h"
#include "vesseldata.h"


VesselListWidgetItem::VesselListWidgetItem(QSharedPointer<KSPS3::VesselData> element,
        QListWidget *parent, int type) :
    QListWidgetItem(parent, type), m_dataField(element)
{

}

VesselListWidgetItem::VesselListWidgetItem(QSharedPointer<KSPS3::VesselData> element, const QString &text,
                                       QListWidget *parent, int type) :
    QListWidgetItem(text, parent, type), m_dataField(element)
{

}

VesselListWidgetItem::VesselListWidgetItem(QSharedPointer<KSPS3::VesselData> element, const QIcon &icon, const QString &text,
                                       QListWidget *parent, int type) :
    QListWidgetItem(icon, text, parent, type), m_dataField(element)
{

}

VesselListWidgetItem::VesselListWidgetItem(VesselListWidgetItem &other)
    : QListWidgetItem(other), m_dataField(other.GetVesselData())
{

}

QSharedPointer<KSPS3::VesselData> VesselListWidgetItem::GetVesselData()
{
    return m_dataField;
}
