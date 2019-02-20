#ifndef DEVICETREEWIDGETITEM_H
#define DEVICETREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include "GB28181Server/GB28181Server.h"

class DeviceTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit DeviceTreeWidgetItem(QObject *parent = 0);
    ~DeviceTreeWidgetItem();

    DeviceNode getDeviceNode(){return mDeviceNode;}
    void setDeviceNode(DeviceNode node){mDeviceNode = node;}

private:
    DeviceNode mDeviceNode;

};

#endif // DEVICETREEWIDGETITEM_H
