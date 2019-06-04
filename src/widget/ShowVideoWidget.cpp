/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#include "ShowVideoWidget.h"
#include "ui_ShowVideoWidget.h"

#include <QPainter>

ShowVideoWidget::ShowVideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowVideoWidget)
{
    ui->setupUi(this);

    connect(this, &ShowVideoWidget::sig_DisPlayVideo, this, &ShowVideoWidget::slotDisPlayVideo);
}

ShowVideoWidget::~ShowVideoWidget()
{
    delete ui;
}

void ShowVideoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	
	painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
	
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height()); //先画成黑色

    if (mImage.size().width() <= 0) return;

    ///将图像按比例缩放成和窗口一样大小
    QImage img = mImage.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); //这里效率比较低下  还不知道如何优化

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x,y),img); //画出图像

}

void ShowVideoWidget::inputOneFrame(const QImage &img)
{
    if (!this->isVisible()) this->show();

    mImage = img;
    update(); //调用update将执行 paintEvent函数
}

void ShowVideoWidget::clear()
{
    mImage = QImage(0, 0);
    update();
}

void ShowVideoWidget::onDisPlayVideoData(const uint8_t *rgb32Buffer, const int bufferSize,  const int width, const int height)
{
    //把这个RGB数据 用QImage加载
    QImage tmpImg((uchar *)rgb32Buffer, width, height, QImage::Format_RGB32);
//  QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
    QImage image = tmpImg.convertToFormat(QImage::Format_RGB888,Qt::NoAlpha); //去掉透明的部分 有些奇葩的视频会透明

    emit sig_DisPlayVideo(image);

}

void ShowVideoWidget::slotDisPlayVideo(const QImage &img)
{
    inputOneFrame(img);
}
