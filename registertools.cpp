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
        QAction *action=new QAction("YiAnKang");
        menu->addAction(action);
        titlebar()->setMenu(menu);

        DPushButton *filelocation = new DPushButton();
        DPushButton *getid = new DPushButton();
        DPushButton *hefeisync = new DPushButton();
        DPushButton *updata =new DPushButton();

//        titlebar()->setIcon(QIcon::fromTheme("deepin-launcher"));
        titlebar()->setIcon(QIcon(":/RegisterTools.ico"));

        connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,this,&RegisterTools::setTheme);
        QVBoxLayout *vlayout =new QVBoxLayout(w);
        QHBoxLayout *h1layout = new QHBoxLayout(w);
        QHBoxLayout *h2layout = new QHBoxLayout(w);
        QHBoxLayout *h3layout = new QHBoxLayout(w);
        QHBoxLayout *h4layout = new QHBoxLayout(w);
        QHBoxLayout *h5layout = new QHBoxLayout(w);
        QHBoxLayout *h6layout = new QHBoxLayout(w);
        QHBoxLayout *h7layout = new QHBoxLayout(w);
        QHBoxLayout *h8layout = new QHBoxLayout(w);

        DLabel *text1 = new DLabel;
        text1->setText("1:选择MAC表格");
        text1->setAlignment(Qt::AlignLeft);
        DLineEdit *locationLineEdit = new DLineEdit;
        filelocation->setText("选择");
        h1layout->addWidget(text1,2);
        h1layout->addWidget(locationLineEdit,4);
        h1layout->addWidget(filelocation,2);

        auto progressbarid = new DProgressBar();
        progressbarid->setTextVisible(true);
        progressbarid->setValue(0);  // 当前进度
        progressbarid->setMaximum(100);
        DLabel *text2 = new DLabel;
        text2->setText("2:获取设备ID");
        text2->setAlignment(Qt::AlignLeft);
        getid->setText("获取");
        h2layout->addWidget(text2,2);
        h2layout->addWidget(progressbarid,4);
        h2layout->addWidget(getid,2);

        DLabel *text3 = new DLabel;
        text3->setText("3:手动绑定设备");
        text3->setAlignment(Qt::AlignLeft);
        h3layout->addWidget(text3,2);
        h3layout->addStretch(6);


        DLabel *text4 = new DLabel;
        text4->setText("4:合肥同步");
        text4->setAlignment(Qt::AlignLeft);
        hefeisync->setText("同步");
        h4layout->addWidget(text4,2);
        h4layout->addStretch(4);
        h4layout->addWidget(hefeisync,2);


        auto progressbar = new DProgressBar();
        progressbar->setTextVisible(true);
        progressbar->setValue(0);  // 当前进度
        progressbar->setMaximum(100);
        DLabel *text5 = new DLabel;
        text5->setText("5:数据上传");
        text5->setAlignment(Qt::AlignLeft);
        updata->setText("模拟");
        h5layout->addWidget(text5,2);
        h5layout->addWidget(progressbar,4);
        h5layout->addWidget(updata,2);

        DLabel *text6 = new DLabel();
        text6->setText("NO1:二维码处理");
        text6->setAlignment(Qt::AlignLeft);
        DLineEdit *picLineEdit = new DLineEdit;
        DPushButton *choosebutton =new DPushButton();
        choosebutton->setText("选择");
        DPushButton *chooseokbutton =new DPushButton();
        chooseokbutton->setText("生成");
        h6layout->addWidget(text6,2);
        h6layout->addWidget(picLineEdit,4);
        h6layout->addWidget(choosebutton,1);
        h6layout->addWidget(chooseokbutton,1);

        DLabel *text7 = new DLabel();
        text7->setText("NO2:批量二维码");
        text7->setAlignment(Qt::AlignLeft);
        DLineEdit *picallLineEdit = new DLineEdit;
        DPushButton *chooseallbutton =new DPushButton();
        chooseallbutton->setText("选择");
        DPushButton *chooseallokbutton =new DPushButton();
        chooseallokbutton->setText("生成");
        h7layout->addWidget(text7,2);
        h7layout->addWidget(picallLineEdit,4);
        h7layout->addWidget(chooseallbutton,1);
        h7layout->addWidget(chooseallokbutton,1);

        DLabel *text8 = new DLabel();
        text8->setText("NO3:盒子MAC转设备号");
        text8->setAlignment(Qt::AlignLeft);
        DLineEdit *macLineEdit = new DLineEdit();
        DPushButton *macbutton = new DPushButton();
        macbutton->setText("获取");
        h8layout->addWidget(text8,2);
        h8layout->addWidget(macLineEdit,4);
        h8layout->addWidget(macbutton,2);


        connect(progressbar, &DProgressBar::valueChanged, this, [=](int value){
                progressbar->setFormat(QString("已完成%1%").arg(value));
            });
        connect(progressbarid, &DProgressBar::valueChanged, this, [=](int value){
                progressbarid->setFormat(QString("已完成%1%").arg(value));
            });


        vlayout->addStretch(2);
        vlayout->addLayout(h1layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h2layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h3layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h4layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h5layout);
        vlayout->addStretch(3);
        vlayout->addLayout(h6layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h7layout);
        vlayout->addStretch(1);
        vlayout->addLayout(h8layout);
        vlayout->addStretch(2);


        qnam = new QNetworkAccessManager();
        qnamup = new QNetworkAccessManager();
        QObject::connect(qnam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishedSlot(QNetworkReply*)));

        //file location choose button
        connect(filelocation, &DPushButton::clicked, this, [ = ] {
        fileName = QFileDialog::getOpenFileName(this, tr("打开表格"),
                                                              "/home/houyawei/Desktop",
                                                               tr("打开表格 (*.xlsx)"));
        locationLineEdit->setText(fileName);

        });

        //getid button
        connect(getid,&DPushButton::clicked,this,[ = ]{
            QTimer *timer;
            timer = new QTimer(this);
            timer->setInterval(1500);
            timer->start();
            Row=0;
            connect(timer,&QTimer::timeout,this,[ = ]{
            QXlsx::Document xlsx(fileName);
            Row++;
            A_Row = "A" + QString::number(Row);
            B_Row = "B" + QString::number(Row);
            C_Row = "C" + QString::number(Row);

            MAC = xlsx.read(A_Row).toString().remove(":").remove("\"").remove(" ");
            if(!(MAC.isNull()))
            {
             progressbarid->setValue(qRound(double(Row/110.0)*100));
             qDebug()<<MAC;
             CheckDeviceID(MAC);
            }
            else
            {
             progressbarid->setValue(100);
             timer->stop();
             delete  timer;
             qDebug()<<"设备ID获取结束";
            return;
            }
            });
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

            qDebug()<<"打开文件"<<fileName<<endl;
            QTimer *timer;
            timer = new QTimer(this);
            timer->setInterval(1500);
            timer->start();
            Row=0;
            connect(timer,&QTimer::timeout,this,[ = ]{
            QXlsx::Document xlsx(fileName);
            Row++;
            A_Row = "A" + QString::number(Row);
            B_Row = "B" + QString::number(Row);
            C_Row = "C" + QString::number(Row);
            D_Row = "D" + QString::number(Row);
            DEV = xlsx.read(C_Row).toString();
            if(!(DEV.isNull()))
            {
             progressbar->setValue(qRound(double(Row/110.0)*100));
             qDebug()<<DEV;
             getBasicSettings(DEV);
             WuhanHefei(DEV,xlsx.read(D_Row).toString());
            }
            else
            {
            progressbar->setValue(100);
            qDebug()<<"数据上传结束";
            timer->stop();
            delete  timer;
            return;
            }
        });
      });

        //picchoosebutton
        connect(choosebutton,&DPushButton::clicked,this,[ = ]{

            picfileName = QFileDialog::getOpenFileName(this, tr("打开图片"),
                                                                  "/home/houyawei/Desktop",
                                                                   tr("打开图片 (*.png *.jpg)"));
            picLineEdit->setText(picfileName);
        });

        //picchooseokbutton
        connect(chooseokbutton,&DPushButton::clicked,this,[ = ]{

            editImages(picLineEdit->text());
            editlongImages(picLineEdit->text());

        });
        //picchooseallbutton
        connect(chooseallbutton,&DPushButton::clicked,this,[ = ]{

            picfileallName = QFileDialog::getExistingDirectory(this, tr("打开图片文件夹"),
                                                                  "/home/houyawei/Desktop");
            picallLineEdit->setText(picfileallName);
        });
        //todo picchooseallokbutton
        connect(chooseallokbutton,&DPushButton::clicked,this,[ = ]{

            qDebug()<<getfiles(picallLineEdit->text());
            //editImages(picallLineEdit->text());

        });
        //getid signle button
        connect(macbutton,&DPushButton::clicked,this,[ = ]{
            SignleDev = true;
            MAC=macLineEdit->text().toUpper().remove(":").remove("\"").remove(" ").remove("\r").remove("\n");
            qDebug()<<MAC;
            CheckDeviceID(MAC);
             QTimer::singleShot(1.5 * 1000,this,[=]{
                 macLineEdit->setText(DEV);
                 SignleDev = false;
             });

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
                         if(obj.contains(QString("picStream")) && SignleDev == false)
                         {
 //                            qDebug() << "in picStream...";

                             QString m_strDeviceID = obj.value("deviceId").toString();
                             QString picStream = obj.value("picStream").toString();
                             qDebug()<<A_Row<<" "<<B_Row<<" 此设备号为："<<m_strDeviceID;
                             xlsx.write(B_Row,MAC);
                             xlsx.write(C_Row,m_strDeviceID);
                             xlsx.save();
                         }
                         else if(obj.contains(QString("picStream")) && SignleDev == true)
                         {
                              QString m_strDeviceID = obj.value("deviceId").toString();
                              DEV = m_strDeviceID;
                               qDebug()<<"signal dev 此设备号为："<<DEV;
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
     if (reply->isFinished())
         reply->deleteLater();
     if (reply2->isFinished())
         reply2->deleteLater();
}
QStringList RegisterTools::getfiles(const QString &dir_path)
{
    QStringList get_files;
    QDir dir(dir_path);
    if(!dir.exists())
    {
        qDebug() << "it is not true dir_path";
    }

    /*设置过滤参数，QDir::NoDotAndDotDot表示不会去遍历上层目录*/
    QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
     qDebug() << "dir_path num:"<<info_list.count();
    for(int i= 0 ; i< info_list.count(); i++)
    {
        QFileInfo info = info_list.at(i);
        if(info.isDir())
        {
            /*找出下层目录的绝对路径*/
            QString sub_dir  = info.absoluteFilePath();
            /*递归*/
            QStringList get_next_files = getfiles(sub_dir);

            get_files.append(get_next_files);
        }
        else
        {
            QString file_name = info.absoluteFilePath();
            editImages(file_name);
            editlongImages(file_name);
            get_files.append(file_name);
        }
    }

    return get_files;
}

void RegisterTools::editImages(const QString &imagefile)
{
    QImage image(imagefile);
    QPainter p;
    if(!p.begin(&image)) qDebug()<<"open failed"<<endl;

    p.setPen(QPen(Qt::red));
    p.setFont(QFont("Times", 30, QFont::Light));
    QString deviceid = imagefile.mid(imagefile.lastIndexOf("/")+1,10);
    QString deviceid_left =deviceid.left(5);
    QString deviceid_right =deviceid.right(5);
    p.drawText(0, 25, deviceid_left);
    p.drawText(198, 25, deviceid_right);

    p.end();

    QString newimage = imagefile.mid(0,imagefile.lastIndexOf("."))+"_new.jpg";
    qDebug()<<newimage<<endl;
    image.save(newimage);
}
void RegisterTools::editlongImages(const QString &imagefile)
{
    QImage *saveImage = new QImage(QSize(280,500),QImage::Format_RGB888);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    saveImage->fill(QColor(Qt::white).rgb());
    saveImage->save(&buffer,"PNG");
    buffer.close();

    QImage image(imagefile);
    QPainter p(saveImage);
    if(!p.begin(&image)) qDebug()<<"open failed"<<endl;

    p.setPen(QPen(Qt::red));
    p.setFont(QFont("Times", 38, QFont::Light));
    QString deviceid = imagefile.mid(imagefile.lastIndexOf("/")+1,10);
    p.drawText(0, 300+25, deviceid);
    QString devicename = "YAK-40  4g";
    p.drawText(20, 360+25, devicename);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0,image);

    p.end();

    QString newimage = imagefile.mid(0,imagefile.lastIndexOf("."))+"_newlong.jpg";
    qDebug()<<newimage<<endl;
    saveImage->save(newimage);

}
