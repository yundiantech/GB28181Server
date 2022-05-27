#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDateTime>

#include "widget/DeviceTreeWidgetItem.h"
#include "widget/ChannelTreeWidgetItem.h"

#include <QDebug>

Q_DECLARE_METATYPE(MessageType);
Q_DECLARE_METATYPE(CameraDevice);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mGB28181Server = nullptr;

    connect(ui->pushButton_start, &QPushButton::clicked, this, &MainWindow::slotBtnClicked);
    connect(ui->pushButton_stop,  &QPushButton::clicked, this, &MainWindow::slotBtnClicked);

    connect(ui->pushButton_catlog, &QPushButton::clicked, this, &MainWindow::slotBtnClicked);
    connect(ui->pushButton_invite, &QPushButton::clicked, this, &MainWindow::slotBtnClicked);

    ui->pushButton_stop->setEnabled(false);

    mCurrentSelectedItem = NULL;

    mDevicePopMenu = new QMenu;
    mCatalogAction = new QAction(QIcon("images/open.png"), QStringLiteral("请求设备目录"),this);
    mDevicePopMenu->addAction(mCatalogAction);
    connect(mCatalogAction, &QAction::triggered, this, &MainWindow::slotActionTriggered);

    mChannelPopMenu = new QMenu;
    mPlayVideoAction = new QAction(QIcon("images/open.png"), QStringLiteral("播放视频流"),this);
    mChannelPopMenu->addAction(mPlayVideoAction);
    connect(mPlayVideoAction, &QAction::triggered, this, &MainWindow::slotActionTriggered);

    connect(ui->treeWidget, &QTreeWidget::itemPressed, this, &MainWindow::slotItemClicked);

    qRegisterMetaType<MessageType>();
    qRegisterMetaType<CameraDevice>();

    connect(this, &MainWindow::sig_deviceRegisted, this, &MainWindow::slotDeviceRegisted);
    connect(this, &MainWindow::sig_deviceUpdate, this, &MainWindow::slotDeviceUpdate);
    connect(this, &MainWindow::sig_receiveMessage, this, &MainWindow::slotReceiveMessage);

    ui->tableWidget->setColumnWidth(0, 140);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 200);
    ui->tableWidget->setColumnWidth(3, 160);

    ui->pushButton_catlog->setEnabled(false);
    ui->pushButton_invite->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

///设备注册成功
void MainWindow::onDeviceRegisted(const CameraDevice &deviceNode)
{
    qDebug("%s %s %s \n", __FUNCTION__, deviceNode.DeviceID.c_str(), deviceNode.IPAddress.c_str());

    emit sig_deviceRegisted(deviceNode);

}

///设备更新，catalog请求返回的设备信息更新
void MainWindow::onDeviceUpdate(const CameraDevice &deviceNode)
{
    emit sig_deviceUpdate(deviceNode);
}

///接收到消息
void MainWindow::onReceiveMessage(const char *deviceID, const MessageType &type, const char *msgBody)
{
//    qDebug()<<__FUNCTION__<<deviceID<<type<<msgBody;
    emit sig_receiveMessage(QString(deviceID), type, QString(msgBody));
}

///设备注册成功
void MainWindow::slotDeviceRegisted(const CameraDevice &deviceNode)
{
    DeviceTreeWidgetItem *item = new DeviceTreeWidgetItem;
    item->setText(0, QString("%1(%2)").arg(QString::fromStdString(deviceNode.DeviceID)).arg(QString::fromStdString(deviceNode.IPAddress)));
    item->setDeviceNode(deviceNode);

    ui->treeWidget->addTopLevelItem(item);
}

///设备更新，catalog请求返回的设备信息更新
void MainWindow::slotDeviceUpdate(const CameraDevice &deviceNode)
{
    int count = ui->treeWidget->topLevelItemCount();

    for (int i=0; i < count; i++)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);

        DeviceTreeWidgetItem *deviceItem = (DeviceTreeWidgetItem*)item;

        CameraDevice node = deviceItem->getDeviceNode();

        if (node == deviceNode)
        {
            ///先清空流列表
            while(deviceItem->childCount() > 0)
            {
                QTreeWidgetItem *item = deviceItem->takeChild(0);
                delete item;
            }

            ///再新建流列表
            for (const VideoChannel* videoChannelNode : deviceNode.channelList)
            {
                ChannelTreeWidgetItem *videoChannelItem = new ChannelTreeWidgetItem;
                videoChannelItem->setText(0, QString("%1(%2)").arg(QString::fromStdString(videoChannelNode->DeviceID)).arg(QString::fromStdString(videoChannelNode->DeviceName)));
                videoChannelItem->setChannelNode(videoChannelNode);
                item->addChild(videoChannelItem);
            }

            ///展开全部流列表
            ui->treeWidget->expandItem(deviceItem);
        }
    }

}

///接收到消息
void MainWindow::slotReceiveMessage(const QString &deviceID, const MessageType &type, const QString &msgBody)
{
//    qDebug()<<__FUNCTION__<<deviceID<<type<<msgBody;
    MessageNode node;
    node.deviceID = deviceID;
    node.type = type;
    node.msgBody = msgBody;
    mMessageList.append(node);

    QString msgType;

    if (type == MessageType_Register)
    {
        msgType = "Register";
    }
    else if (type == MessageType_KeepAlive)
    {
        msgType = "KeepAlive";
    }
    else if (type == MessageType_Catalog)
    {
        msgType = "Catalog";
    }
    else if (type == MessageType_CallAnswer)
    {
        msgType = "CallAnswer";
    }
    else if (type == MessageType_CallFailed)
    {
        msgType = "CallFailed";
    }

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(msgType));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(deviceID));

    QTableWidgetItem *msgBodyItem = new QTableWidgetItem(msgBody);
    msgBodyItem->setToolTip(msgBody);
    ui->tableWidget->setItem(0, 3, msgBodyItem);

}


void MainWindow::slotBtnClicked(bool isChecked)
{
    if (QObject::sender() == ui->pushButton_start)
    {
        mGB28181Server = new GB28181Server();

        mGB28181Server->setEventHandle(this);

        char ip[36] = {0};
        strcpy(ip, ui->lineEdit_ip->text().toUtf8().data());
        int port = ui->lineEdit_port->text().toInt();
        mGB28181Server->setLocalIp(ip, port);

        char sipId[256] = {0};
        strcpy(sipId, ui->lineEdit_sip_id->text().toUtf8().data());
        char passwd[36] = {0};
        strcpy(passwd, ui->lineEdit_password->text().toUtf8().data());
        char realm[36] = {0};
        strcpy(realm, ui->lineEdit_realm->text().toUtf8().data());
        mGB28181Server->setGBServerInfo(sipId, passwd, realm);

        mGB28181Server->start(); //启动GB28181监听服务

        ui->widget_param->setEnabled(false);
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_stop->setEnabled(true);
    }
    else if (QObject::sender() == ui->pushButton_stop)
    {
        mGB28181Server->stop();
        delete mGB28181Server;
        mGB28181Server = nullptr;

        ui->treeWidget->clear();

        ui->widget_param->setEnabled(true);
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_stop->setEnabled(false);
    }
    else if (QObject::sender() == ui->pushButton_catlog)
    {
        DeviceTreeWidgetItem *item = (DeviceTreeWidgetItem*)mCurrentSelectedItem;
        mGB28181Server->doSendCatalog(item->getDeviceNode());
    }
    else if (QObject::sender() == ui->pushButton_invite)
    {
        ChannelTreeWidgetItem *item = (ChannelTreeWidgetItem*)mCurrentSelectedItem;
        mGB28181Server->doSendInvitePlay(item->getChannelNode());
    }
}

void MainWindow::slotItemClicked(QTreeWidgetItem *item, int column)
{
//    qDebug()<<__FUNCTION__<<column<<item->parent();

    QTreeWidgetItem *item1 = item->parent();

    if (item1 == NULL) //选中的是设备
    {
        mCurrentSelectedItem = item;
        ui->pushButton_catlog->setEnabled(true);
        ui->pushButton_invite->setEnabled(false);
    }
    else
    {
        mCurrentSelectedItem = item;
        ui->pushButton_catlog->setEnabled(false);
        ui->pushButton_invite->setEnabled(true);
    }

    if (QGuiApplication::mouseButtons() & Qt::RightButton)
    {
        QTreeWidgetItem *item1 = item->parent();

        if (item1 == NULL) //选中的是设备
        {
            mCurrentSelectedItem = item;
            mDevicePopMenu->exec(QCursor::pos());
        }
        else
        {
            mCurrentSelectedItem = item;
            mChannelPopMenu->exec(QCursor::pos());

            ui->pushButton_catlog->setEnabled(false);
            ui->pushButton_invite->setEnabled(true);
        }
    }

}

void MainWindow::slotActionTriggered(bool checked)
{
    if (QObject::sender() == mCatalogAction)
    {
        DeviceTreeWidgetItem *item = (DeviceTreeWidgetItem*)mCurrentSelectedItem;
        mGB28181Server->doSendCatalog(item->getDeviceNode());
        mCurrentSelectedItem = NULL;
    }
    else if (QObject::sender() == mPlayVideoAction)
    {
        ChannelTreeWidgetItem *item = (ChannelTreeWidgetItem*)mCurrentSelectedItem;
        mGB28181Server->doSendInvitePlay(item->getChannelNode());
        mCurrentSelectedItem = NULL;
    }
}
