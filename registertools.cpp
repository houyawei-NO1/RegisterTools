#include "registertools.h"
#include "ui_registertools.h"
#include <DWidgetUtil>
#include <DSearchEdit>
#include <DTitlebar>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QNetworkAccessManager>
#include <QStandardPaths>


RegisterTools::RegisterTools(DMainWindow *parent)
    : DMainWindow(parent)
{
        setCentralWidget(w);
        moveToCenter(this);
        RegisterTools::resize(700,600);

//        DLabel *label=new DLabel;
//        label->setParent(w);
//        label->setText("Hello World");
//        label->show();

        DSearchEdit *searchEdit = new DSearchEdit;
        titlebar()->setCustomWidget(searchEdit);
        searchEdit->setFixedWidth(200);
        searchEdit->show();

        QMenu *menu=new QMenu;
        QAction *action=new QAction("action");
        menu->addAction(action);
        titlebar()->setMenu(menu);

        DPushButton *filelocation = new DPushButton();
        DPushButton *getid = new DPushButton();
        DPushButton *hefeisync = new DPushButton();
        DPushButton *updata =new DPushButton();

//        titlebar()->setIcon(QIcon::fromTheme("deepin-launcher"));

        connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,this,&RegisterTools::setTheme);

        QVBoxLayout *vlayout =new QVBoxLayout(w);
        QHBoxLayout *h1layout = new QHBoxLayout(w);
        QHBoxLayout *h2layout = new QHBoxLayout(w);
        QHBoxLayout *h3layout = new QHBoxLayout(w);
        QHBoxLayout *h4layout = new QHBoxLayout(w);
        QHBoxLayout *h5layout = new QHBoxLayout(w);

        DLabel *text1 = new DLabel;
        text1->setText("1:选择MAC表格");
        text1->setAlignment(Qt::AlignLeft);
        DLineEdit *locationLineEdit = new DLineEdit;
        filelocation->setText("选择");
        h1layout->addWidget(text1,1);
        h1layout->addWidget(locationLineEdit,2);
        h1layout->addWidget(filelocation,1);

        auto progressbarid = new DProgressBar();
        progressbarid->setTextVisible(true);
        progressbarid->setValue(0);  // 当前进度
        progressbarid->setMaximum(100);
        DLabel *text2 = new DLabel;
        text2->setText("2:获取设备ID");
        text2->setAlignment(Qt::AlignLeft);
        getid->setText("获取");
        h2layout->addWidget(text2,1);
        h2layout->addWidget(progressbarid,2);
        h2layout->addWidget(getid,1);

        DLabel *text3 = new DLabel;
        text3->setText("3:手动绑定设备");
        text3->setAlignment(Qt::AlignLeft);
        h3layout->addWidget(text3,1);
        h3layout->addStretch(3);


        DLabel *text4 = new DLabel;
        text4->setText("4:合肥同步");
        text4->setAlignment(Qt::AlignLeft);
        hefeisync->setText("同步");
        h4layout->addWidget(text4,1);
        h4layout->addStretch(2);
        h4layout->addWidget(hefeisync,1);


        auto progressbar = new DProgressBar();
        progressbar->setTextVisible(true);
        progressbar->setValue(0);  // 当前进度
        progressbar->setMaximum(100);
        DLabel *text5 = new DLabel;
        text5->setText("5:数据上传");
        text5->setAlignment(Qt::AlignLeft);
        updata->setText("模拟");
        h5layout->addWidget(text5,1);
        h5layout->addWidget(progressbar,2);
        h5layout->addWidget(updata,1);

        connect(progressbar, &DProgressBar::valueChanged, this, [=](int value){
                progressbar->setFormat(QString("已完成%1%").arg(value));
            });
        connect(progressbarid, &DProgressBar::valueChanged, this, [=](int value){
                progressbarid->setFormat(QString("已完成%1%").arg(value));
            });


        vlayout->addStretch(1);
        vlayout->addLayout(h1layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h2layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h3layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h4layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h5layout);
        vlayout->addStretch(1);

        qnam = new QNetworkAccessManager();
        qnamup = new QNetworkAccessManager();
        QObject::connect(qnam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishedSlot(QNetworkReply*)));

        //file location choose button
        connect(filelocation, &DPushButton::clicked, this, [ = ] {
        fileName = QFileDialog::getOpenFileName(this, tr("打开表格"),
                                                              "/home",
                                                               tr("打开表格 (*.xlsx)"));
        locationLineEdit->setText(fileName);

        });

        //getid button
        connect(getid,&DPushButton::clicked,this,[ = ]{
            QXlsx::Document xlsx(fileName);
            qDebug()<<"打开文件"<<fileName<<endl;
            for(Row=1;Row<110;Row++)
            {
            progressbarid->setValue(Row);
            A_Row = "A" + QString::number(Row);
            B_Row = "B" + QString::number(Row);
            C_Row = "C" + QString::number(Row);

            MAC = xlsx.read(A_Row).toString().remove(":").remove("\"").remove(" ");
            if(!(MAC.isNull()))
            {
             qDebug()<<MAC;
             CheckDeviceID(MAC);
             QEventLoop loop;
             QTimer::singleShot(1500,&loop,SLOT(quit()));
             loop.exec();
            }
            else
            {
            qDebug()<<"设备ID获取结束";
            return;
            }
            }
        });

        //hefei sync button
        connect(hefeisync, &DPushButton::clicked, this, [ = ] {
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
              qDebug() << "开始同步";
            connect(manager, &QNetworkAccessManager::finished,
                    this, [=](QNetworkReply *reply) {
                if (reply->error()) {
                    qDebug() << reply->errorString();
                    return;
                }

                QString answer = reply->readAll();

                qDebug() <<"同步结束"<<answer;
            }
        );

//            manager->get(QNetworkRequest(QUrl("https://www.baidu.com")));
            manager->get(QNetworkRequest(QUrl("http://p.hibao789.com/index/api/devicebasicinfo")));
        });

        //updata button
        connect(updata,&DPushButton::clicked,this,[ = ]{
            QXlsx::Document xlsx(fileName);
            qDebug()<<"打开文件"<<fileName<<endl;
            for(Row=1;Row<110;Row++)
            {
            progressbar->setValue(Row);
            A_Row = "A" + QString::number(Row);
            B_Row = "B" + QString::number(Row);
            C_Row = "C" + QString::number(Row);
            D_Row = "D" + QString::number(Row);
            DEV = xlsx.read(C_Row).toString();
            if(!(C_Row.isNull()))
            {
             qDebug()<<DEV;
             getBasicSettings(DEV);
             QEventLoop loop;
             QTimer::singleShot(1500,&loop,SLOT(quit()));
             loop.exec();
             WuhanHefei(DEV,DEV);
            }
            else
            {
            qDebug()<<"设备ID获取结束";
            return;
            }
            }
        });

}

RegisterTools::~RegisterTools()
{
    delete w;
}

void RegisterTools::setTheme(DGuiApplicationHelper::ColorType theme)
{
    if(theme==DGuiApplicationHelper::LightType){
        qDebug()<<"Light";
    }else {
        qDebug()<<"Dark";
    }
}
void RegisterTools::CheckDeviceID(const QString &strmac)
{

    QString url("http://h.hibao789.com/sleep/public/index.php/device/monitoringdata/getDeviceQRcode");
    const QUrl aurl(url);
    QNetworkRequest qnr(aurl);
    qnr.setRawHeader("Content-Type", "application/json;charset=utf8");

    QString md5Str = QCryptographicHash::hash(strmac.toLatin1(),QCryptographicHash::Md5).toHex();

    QString strData;
    QString strCPU = md5Str.mid(0,12);
    QString strMAC = md5Str.mid(20,12);
    qDebug()<<"strCPU"<<strCPU;
    qDebug()<<"strMAc"<<strMAC;
    strData.append(strCPU);
    strData.append(strMAC);
    QString pwd = strCPU.mid(1, 3) + strMAC.mid(1,3);
    QString md5;
    QByteArray ba, bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(pwd);
    md.addData(ba);
    bb = md.result();
    md5.append(bb.toHex());
    strData.append(md5);

    QJsonObject obj;
    obj.insert("cpuId", strCPU);
    obj.insert("mac", strMAC);
    obj.insert("key", md5);
    QJsonDocument jsonDoc(obj);

    QString str(jsonDoc.toJson());




    QNetworkReply *reply = qnam->post(qnr, jsonDoc.toJson());//*updata*CheckDeviceID:    cpuId,mac,key
       qDebug()<<jsonDoc;
    if (reply->isFinished())
        reply->deleteLater();
}

void RegisterTools::getBasicSettings(const QString &deviceid)
{
    qDebug()<<"getBasicSettings"<<deviceid;
    QString url("http://h.hibao789.com/sleep/public/index.php/device/monitoringdata/getDeviceBasicSettings");

    const QUrl aurl(url);
    QNetworkRequest qnr(aurl);
    qnr.setRawHeader("Content-Type", "application/json;charset=utf8");

    QString strData;
    QString md5;
    QByteArray ba, bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(deviceid);
    md.addData(ba);
    bb = md.result();
    md5.append(bb.toHex());
    strData.append(deviceid);
    strData.append(md5);

    QJsonObject obj;
    obj.insert("deviceId", deviceid);
    obj.insert("key", md5);
    QJsonDocument jsonDoc(obj);
    QString str(jsonDoc.toJson());

    QNetworkReply *reply = qnam->post(qnr, jsonDoc.toJson());//*updata*CheckDeviceID:    cpuId,mac,key
       qDebug()<<jsonDoc;
    if (reply->isFinished())
        reply->deleteLater();
}
void RegisterTools::finishedSlot(QNetworkReply *reply)
{
    QXlsx::Document xlsx(fileName);
    if (reply->error() == QNetworkReply::NoError)
         {
             QByteArray bytes = reply->readAll();
             QJsonParseError e;
             QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &e);
             if (e.error == QJsonParseError::NoError && !jsonDoc.isNull())
             {

                 if((!jsonDoc.isNull()) || (!jsonDoc.isEmpty()))
                 {

                     if(jsonDoc.isObject())

                     {
                         QJsonObject obj = jsonDoc.object();
                         if(obj.contains(QString("picStream")))
                         {
 //                            qDebug() << "in picStream...";

                             QString m_strDeviceID = obj.value("deviceId").toString();
                             QString picStream = obj.value("picStream").toString();
                             qDebug()<<A_Row<<" "<<B_Row<<" 此设备号为："<<m_strDeviceID;
                             xlsx.write(B_Row,MAC);
                             xlsx.write(C_Row,m_strDeviceID);
                             xlsx.save();
                         }
                         if(obj.contains(QString("companyCode")))
                         {
                             QString m_strCompanyCode = obj.value("companyCode").toString();
                             qDebug()<<D_Row<<" 此机构号为："<<m_strCompanyCode;
                             xlsx.write(D_Row,m_strCompanyCode);
                             xlsx.save();
                         }

                  }
             }

         }
         else
         {
             qDebug() << "finishedSlot errors here";
             qDebug( "found error .... code: %d\n", (int)reply->error());
             qDebug(qPrintable(reply->errorString()));
         }
         reply->deleteLater();

}}
void RegisterTools::WuhanHefei(const QString &deviceid,const QString &comid)
{

     QString url("http://h.hibao789.com/sleep/public/index.php/device/monitoringdata/dataCollection");
     QNetworkRequest qnr1(url);
     qnr1.setRawHeader("Content-Type", "application/json;charset=utf8");
     QString url2("http://t.hibao789.com/api/dataCollection.php");
     QNetworkRequest qnr2(url2);
     qnr2.setRawHeader("Content-Type", "application/json;charset=utf8");
     QDateTime current_date_time = QDateTime::currentDateTime();
     QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
     QJsonObject obj,obj1;
     obj.insert("companyCode", comid);
     QJsonArray arrayObj;
     obj1.insert("uploadTime", current_date);
     obj1.insert("deviceId",  deviceid);
     obj1.insert("status", QString::number(5));
     obj1.insert("pkgNo", QString::number(1));
     obj1.insert("heartRate", QString::number(66));
     obj1.insert("breathRate", QString::number(18));
     obj1.insert("distance", QString::number(88));
     arrayObj.push_back(obj1);
     QJsonDocument arrayDoc(arrayObj);
     QString str(arrayDoc.toJson());
     QString strDataList;
     strDataList = str;
     obj.insert("monitorDataList", strDataList);

     QJsonDocument jsonDoc_whhf(obj);

     qDebug()<<jsonDoc_whhf<<endl;
     QNetworkReply *reply = qnamup->post(qnr1, jsonDoc_whhf.toJson());
     QNetworkReply *reply2 = qnamup->post(qnr2, jsonDoc_whhf.toJson());
}
