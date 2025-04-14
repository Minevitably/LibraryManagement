#include "dbutil.h"

// 初始化静态成员变量
QString DBUtil::driver = "QMYSQL";
QString DBUtil::host = "localhost";
int DBUtil::port = 3306;
QString DBUtil::dbname = "teacher_scheduling_system";
QString DBUtil::username = "root";
QString DBUtil::password = "123456";
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

    // 读取配置信息
    driver = settings.value("database/driver", "QMYSQL").toString();
    host = settings.value("database/host", "localhost").toString();
    port = settings.value("database/port", 3306).toInt();
    dbname = settings.value("database/dbname", "teacher_scheduling_system").toString();
    username = settings.value("database/username", "root").toString();
    password = settings.value("database/password", "123456").toString();

    isInitialized = true;
    return true;
}

QSqlDatabase DBUtil::getConnection() {
    if (!isInitialized) {
        initConfig();
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
            qDebug() << "Failed to connect to database:" << db.lastError().text();
            return QSqlDatabase(); // 返回无效的数据库对象
        }
    }

    return db;
}