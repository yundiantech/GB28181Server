#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "GB28181Server/GB28181Server.h"

namespace Ui {
class MainWindow;
}

struct MessageNode
{
    QString deviceID;
    MessageType type;
    QString msgBody;
};

class MainWindow : public QMainWindow, GB28181Server
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void deviceRegisted(DeviceNode deviceNode); //设备注册成功
    void deviceUpdate(DeviceNode deviceNode);   //设备更新，catalog请求返回的设备信息更新
    void receiveMessage(const char *deviceID, MessageType type, char *msgBody);  //接收到消息

    /// 以上三个函数是在子线程中调用的，因此不能在函数中直接操作界面，
    /// 需要转移到主线程函数中操作，采用信号槽的方式来转移到主线程
signals:
    void sig_deviceRegisted(DeviceNode deviceNode); //设备注册成功
    void sig_deviceUpdate(DeviceNode deviceNode);   //设备更新，catalog请求返回的设备信息更新
    void sig_receiveMessage(QString deviceID, MessageType type, QString msgBody);  //接收到消息

private slots:
    void slotDeviceRegisted(DeviceNode deviceNode); //设备注册成功
    void slotDeviceUpdate(DeviceNode deviceNode);   //设备更新，catalog请求返回的设备信息更新
    void slotReceiveMessage(QString deviceID, MessageType type, QString msgBody);  //接收到消息

private:
    Ui::MainWindow *ui;

    QMenu* mDevicePopMenu;
    QAction *mCatalogAction;

    QTreeWidgetItem *mCurrentSelectedItem; //记录当前选中的条目

    QList<MessageNode> mMessageList;

private slots:
    void slotBtnClicked(bool isChecked);
    void slotItemClicked(QTreeWidgetItem *item, int column);
    void slotActionTriggered(bool checked);

};

#endif // MAINWINDOW_H
