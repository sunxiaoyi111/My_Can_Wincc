#include "m_canconfig.h"

m_canconfig::m_canconfig()
{

}
m_canconfig::~m_canconfig()
{

}
void m_canconfig::m_canconfig_init()
{
    // 获取程序的路径
    QString appDir = QCoreApplication::applicationDirPath();

    // 创建QSettings对象，参数1是公司名称，参数2是应用程序名称
    // 这将在用户的主目录下创建一个配置文件
    QSettings settings(QDir(appDir).filePath("cansettings/candevice.ini"), QSettings::IniFormat);

    // 启动时读取配置
    QString username = settings.value("username1").toString(); // 读取用户名
    QString password = settings.value("password1").toString(); // 读取密码

//     ... 这里是你的程序主体部分 ...
    if(username == "sunxiaoyi")
    {
        password = "123456";
    }
    username = "sunxiaoyi";
    // 关闭时保存配置
    settings.setValue("username1", username); // 保存用户名
    settings.setValue("password1", password); // 保存密码
}
