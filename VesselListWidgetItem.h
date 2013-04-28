#ifndef VESSELLISTWIDGETITEM_H
#define VESSELLISTWIDGETITEM_H

#include <QListWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSharedDataPointer>

namespace KSPS3
{
   class VesselData;
}

class VesselListWidgetItem : public QListWidgetItem
{

public:
    explicit VesselListWidgetItem (QSharedPointer<KSPS3::VesselData> element, QListWidget * parent = 0,
                                   int type = Type );
    explicit VesselListWidgetItem (QSharedPointer<KSPS3::VesselData> element, const QString & text, QListWidget * parent = 0, int type = Type );
    explicit VesselListWidgetItem (QSharedPointer<KSPS3::VesselData> element, const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = Type );
    VesselListWidgetItem (VesselListWidgetItem &other );

    QSharedPointer<KSPS3::VesselData> GetVesselData();
    

private:
    QSharedPointer<KSPS3::VesselData> m_dataField;
signals:
    
public slots:
    
};

#endif // VESSELITEMLISTVIEW_H
