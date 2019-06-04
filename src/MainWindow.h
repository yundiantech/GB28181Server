#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "GB28181Server/GB28181Server.h"
#include "EventHandle/EventHandle.h"

namespace Ui {
class MainWindow;
}

struct MessageNode
{
    QString deviceID;
    MessageType type;
    QString msgBody;
};

class MainWindow : public QMainWindow, GB28181ServerEventHandle
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void onDeviceRegisted(const CameraDevice &deviceNode); //设备注册成功
    void onDeviceUpdate(const CameraDevice &deviceNode);   //设备更新，catalog请求返回的设备信息更新
    void onReceiveMessage(const char *deviceID, const MessageType &type, const char *msgBody);  //接收到消息

    /// 以上三个函数是在子线程中调用的，因此不能在函数中直接操作界面，
    /// 需要转移到主线程函数中操作，采用信号槽的方式来转移到主线程
signals:
    void sig_deviceRegisted(const CameraDevice &deviceNode); //设备注册成功
    void sig_deviceUpdate(const CameraDevice &deviceNode);   //设备更新，catalog请求返回的设备信息更新
    void sig_receiveMessage(const QString &deviceID, const MessageType &type, const QString &msgBody);  //接收到消息

private slots:
    void slotDeviceRegisted(const CameraDevice &deviceNode); //设备注册成功
    void slotDeviceUpdate(const CameraDevice &deviceNode);   //设备更新，catalog请求返回的设备信息更新
    void slotReceiveMessage(const QString &deviceID, const MessageType &type, const QString &msgBody);  //接收到消息

private:
    GB28181Server *mGB28181Server; //28181服务

private:
    Ui::MainWindow *ui;

    ///设备操作的弹出菜单
    QMenu* mDevicePopMenu;
    QAction *mCatalogAction;

    ///设备通道操作的弹出菜单
    QMenu* mChannelPopMenu;
    QAction *mPlayVideoAction;

    ///按下菜单的时候，当前选中的条目
    QTreeWidgetItem *mCurrentSelectedItem; //记录当前选中的条目

    QList<MessageNode> mMessageList; //收到的消息列表

private slots:
    void slotBtnClicked(bool isChecked);
    void slotItemClicked(QTreeWidgetItem *item, int column);
    void slotActionTriggered(bool checked);

};

#endif // MAINWINDOW_H
