#ifndef CHANNELTREEWIDGETITEM_H
#define CHANNELTREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include "GB28181Server/GB28181Server.h"
#include "ShowVideoWidget.h"

class ChannelTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit ChannelTreeWidgetItem(QObject *parent = 0);
    ~ChannelTreeWidgetItem();

    VideoChannel* getChannelNode(){return mChannelNode;}
    void setChannelNode(const VideoChannel* node);

private:
    VideoChannel *mChannelNode;

    ShowVideoWidget *mShowVideoWidget;

};

#endif // CHANNELTREEWIDGETITEM_H
