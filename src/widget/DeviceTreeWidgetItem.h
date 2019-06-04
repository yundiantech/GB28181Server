#ifndef DEVICETREEWIDGETITEM_H
#define DEVICETREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include "GB28181Server/GB28181Server.h"

class DeviceTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit DeviceTreeWidgetItem(QObject *parent = 0);
    ~DeviceTreeWidgetItem();

    CameraDevice getDeviceNode(){return mDeviceNode;}
    void setDeviceNode(CameraDevice node){mDeviceNode = node;}

private:
    CameraDevice mDeviceNode;

};

#endif // DEVICETREEWIDGETITEM_H
