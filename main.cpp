#include "registertools.h"
#include <DApplication>
#include <DMainWindow>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <QApplication>
DWIDGET_USE_NAMESPACE


int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    DApplication a(argc, argv);
    a.setOrganizationName("YiAnKang");
    a.setApplicationName("hyw_yiankang_registertools");
    a.setApplicationVersion("0.4");
    a.setProductIcon(QIcon(":/RegisterTools.ico"));
    a.setProductName("HYW_RegisterTools");
    a.setApplicationDescription("用于申请设备ID及模拟数据上传\n"
                                "小程序码处理");

    a.loadTranslator();
    a.setApplicationDisplayName(QCoreApplication::translate("Main", "RegisterTools"));


//    DApplicationSettings as;
//    Q_UNUSED(as)

    RegisterTools R;
    R.show();

    Dtk::Widget::moveToCenter(&R);
    return a.exec();
}
