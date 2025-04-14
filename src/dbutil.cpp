#include "dbutil.h"

// 初始化静态成员变量
QString DBUtil::driver;
QString DBUtil::host;
int DBUtil::port;
QString DBUtil::dbname;
QString DBUtil::username;
QString DBUtil::password;
bool DBUtil::isInitialized = false;

bool DBUtil::initConfig(const QString& configPath) {
    if (isInitialized) {
        return true;
    }

    QSettings settings(configPath, QSettings::IniFormat);

    // 检查配置文件是否存在
    if (!QFile::exists(configPath)) {
        qDebug() << "Warning: Database configuration file not found, using default values";
        isInitialized = true;
        return false;
    }


    // 读取配置信息（不再提供默认值）
    driver = settings.value("database/driver").toString();
    host = settings.value("database/host").toString();
    port = settings.value("database/port").toInt();
    dbname = settings.value("database/dbname").toString();
    username = settings.value("database/username").toString();
    password = settings.value("database/password").toString();

    isInitialized = true;
    return true;
}

QSqlDatabase DBUtil::getConnection() {
    if (!isInitialized && !initConfig()) {
        return QSqlDatabase(); // 返回无效的数据库对象
    }

    // 创建唯一连接名称，避免重复连接问题
    QString connectionName = QString("db_connection_%1").arg(quintptr(QThread::currentThreadId()));

    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase(driver, connectionName);
        db.setHostName(host);
        db.setPort(port);
        db.setDatabaseName(dbname);
        db.setUserName(username);
        db.setPassword(password);

        if (!db.open()) {
            qCritical() << "Failed to connect to database:" << db.lastError().text();
            return QSqlDatabase(); // 返回无效的数据库对象
        }
    }

    return db;
}