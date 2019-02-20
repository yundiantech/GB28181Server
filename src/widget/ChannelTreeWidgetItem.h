#ifndef CHANNELTREEWIDGETITEM_H
#define CHANNELTREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include "GB28181Server/GB28181Server.h"

class ChannelTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit ChannelTreeWidgetItem(QObject *parent = 0);
    ~ChannelTreeWidgetItem();

    ChannelNode getChannelNode(){return mChannelNode;}
    void setChannelNode(ChannelNode node){mChannelNode = node;}

private:
    ChannelNode mChannelNode;

};

#endif // CHANNELTREEWIDGETITEM_H
