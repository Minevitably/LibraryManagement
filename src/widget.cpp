#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 初始化时连接数据库并查询
    if(connectToDatabase()) {
//        queryUsers();
    } else {
        qDebug() << "Failed to connect to database!";
    }

    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::onPushButtonClicked);
}

Widget::~Widget()
{
    // 关闭数据库连接
    if(db.isOpen()) {
        db.close();
    }
    delete ui;
}

bool Widget::connectToDatabase()
{
    // 配置数据库连接
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("172.24.160.10");      // 数据库服务器地址
    db.setDatabaseName("library_management");  // 数据库名
    db.setUserName("root");           // 数据库用户名
    db.setPassword("123456");       // 数据库密码

    if(!db.open()) {
        QSqlError error = db.lastError();
        qDebug() << "Database error occurred:" << error.text();
        qDebug() << "Database error occurred:" << db.lastError().text();
        return false;
    }

    qDebug() << "Connected to database successfully!";
    return true;
}

void Widget::queryUsers()
{
    QSqlQuery query;
    QString sql = "SELECT id, username, name, user_type FROM users";

    if(!query.exec(sql)) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    qDebug() << "User list:";
    qDebug() << "ID\tUsername\tName\tType";

    while(query.next()) {
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString name = query.value(2).toString();
        QString type = query.value(3).toString();

        qDebug() << id << "\t" << username << "\t" << name << "\t" << type;
    }
}

void Widget::onPushButtonClicked()
{
    ui->label->setText("Database Connected!");
    queryUsers();  // 点击按钮时重新查询
}