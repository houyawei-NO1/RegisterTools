#ifndef REGISTERTOOLS_H
#define REGISTERTOOLS_H

#include <QWidget>
#include <DMainWindow>
#include <DGuiApplicationHelper>
#include <DLabel>
#include <DPushButton>
#include <DTextEdit>
#include <DGroupBox>
#include <DComboBox>
#include <DCheckBox>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <DDialog>
#include <QNetworkReply>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include <QJsonArray>
#include <QTimer>
#include <QtXlsx>
#include <DProgressBar>
#include <QImage>
#include <QPainter>

DWIDGET_USE_NAMESPACE
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class RegisterTools : public DMainWindow
{
    Q_OBJECT

public:
    RegisterTools(DMainWindow *parent = nullptr);
    QString fileName,picfileName,picfileallName;
    int Row,Col;
    void CheckDeviceID(const QString &strmac);
    void getBasicSettings(const QString &deviceid);
    void WuhanHefei(const QString &deviceid,const QString &comid);
    void editImages(const QString &imagefile);
    void editlongImages(const QString &imagefile);
    QStringList getfiles(const QString &dir_path);
    QString A_Row,B_Row,C_Row,D_Row,MAC,DEV;
    ~RegisterTools();

private:
    QWidget *w=new QWidget;
    void setTheme(DGuiApplicationHelper::ColorType);
    QNetworkAccessManager* qnam;
    QNetworkAccessManager* qnamup;
    bool SignleDev = false;

private slots:
    void finishedSlot(QNetworkReply* reply);


};
#endif // REGISTERTOOLS_H
