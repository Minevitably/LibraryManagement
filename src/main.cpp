#include "widget.h"
#include "dbutil.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // 使用默认配置路径初始化
    // DBUtil::initConfig(); // 可选，getConnection()会自动初始化

    // 获取数据库连接
    QSqlDatabase db = DBUtil::getConnection();
    if (db.isValid()) {
        QSqlQuery query(db);
        if (query.exec("SELECT * FROM users")) {
            while (query.next()) {
                // 处理查询结果...
                int id = query.value(0).toInt();
                QString username = query.value(1).toString();
                QString name = query.value(2).toString();
                QString type = query.value(3).toString();

                qDebug() << id << "\t" << username << "\t" << name << "\t" << type;
            }
        } else {
            qDebug() << "Query error:" << query.lastError().text();
        }
        db.close();
    } else {
        qDebug() << "Failed to get database connection";
    }


    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
