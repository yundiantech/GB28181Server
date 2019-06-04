#include "ChannelTreeWidgetItem.h"

ChannelTreeWidgetItem::ChannelTreeWidgetItem(QObject *parent)
{
    mChannelNode = nullptr;

    mShowVideoWidget = new ShowVideoWidget;
    mShowVideoWidget->hide();
}

ChannelTreeWidgetItem::~ChannelTreeWidgetItem()
{
    mChannelNode->setEventHandle(nullptr);
    delete mShowVideoWidget;
    mShowVideoWidget = nullptr;
}

void ChannelTreeWidgetItem::setChannelNode(const VideoChannel* node)
{
    mChannelNode = (VideoChannel*)node;

    mChannelNode->setEventHandle(mShowVideoWidget);

    QString titleStr = QString("%1(%2)")
                 .arg(QString::fromStdString(node->DeviceID))
                 .arg(QString::fromStdString(node->IPAddress));

    mShowVideoWidget->setWindowTitle(titleStr);
}
