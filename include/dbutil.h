#ifndef DBUTIL_H
#define DBUTIL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSettings>
#include <QFile>
#include <QThread>

class DBUtil {
public:
    // 获取数据库连接
    static QSqlDatabase getConnection();

    // 初始化数据库配置
    static bool initConfig(const QString& configPath = "dbconfig.ini");

private:
    static QString driver;
    static QString host;
    static int port;
    static QString dbname;
    static QString username;
    static QString password;
    static bool isInitialized;
};

#endif // DBUTIL_H