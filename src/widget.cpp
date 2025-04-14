#include "widget.h"
#include "ui_widget.h"
#include "dbutil.h"
#include "userdao.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 登录按钮
    connect(ui->pushButtonLogin, &QPushButton::clicked,
            this, &Widget::onPushButtonLoginClicked);

    // 登出按钮
    connect(ui->btnLogout, &QPushButton::clicked,
            this, &Widget::onbtnLogoutClicked);
    connect(ui->pushButton, &QPushButton::clicked,
            this, &Widget::onPushButtonClicked);

    // 默认位于登录界面
    ui->stackedWidget->setCurrentIndex(0);

    this->onRememberMe();

}

Widget::~Widget() {
    delete ui;
}

void Widget::onRememberMe() {
    // 如果resources/remember_me.ini文件存在，并且flag=1
    // 则说明上次用户选择了记住密码，
    // 此时页面会读取resources/remember_me.ini文件并自动填写账号密码
    const QString& configPath = "resources/remember_me.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    // 检查配置文件是否存在
    if (!QFile::exists(configPath)) {
        qDebug() << "Warning: remember me configuration file not found.";
        return ;
    }


    // 读取配置信息

    int flag = settings.value("remember_me/flag").toInt();
    if (!flag) {
        qDebug() << "Debug: remember me is not checked last time.";
        return;
    }
    ui->checkBoxRemeberMe->setChecked(true);

    QString username = settings.value("remember_me/username").toString();
    QString password = settings.value("remember_me/password").toString();
    ui->lineEditUserName->setText(username);
    ui->lineEditPassword->setText(password);

    qDebug() << "Debug: remember me is done.";
    qDebug() << "UserName" << "\t" << ui->lineEditUserName->text();
    qDebug() << "Password" << "\t" << ui->lineEditPassword->text();

}


void Widget::onPushButtonLoginClicked() {
    qDebug() << "Login clicked.";

    QString username = ui->lineEditUserName->text();
    QString password = ui->lineEditPassword->text();

    qDebug() << "UserName" << "\t" << username;
    qDebug() << "Password" << "\t" << password;

    QSettings settings("resources/remember_me.ini", QSettings::IniFormat);

    if (ui->checkBoxRemeberMe->isChecked()) {
        // 如果用户勾选记住密码，更新 INI 文件
        settings.setValue("remember_me/flag", 1);
        settings.setValue("remember_me/username", username);
        settings.setValue("remember_me/password", password);
        qDebug() << "Debug: Password remembered.";
    } else {
        // 否则更新 flag 为 0
        settings.setValue("remember_me/flag", 0);
        qWarning() << "Debug: Password not remembered.";
    }

    // 验证用户名以及密码
    bool valid = UserDao::validateUser(username, password);
    qDebug() << "Validate user:" << (valid ? "Valid" : "Invalid");
    if (!valid) {
        // 登录失败
        qDebug() << "username or password is wrong.";
        return;
    }
    // 获取用户身份
    User fetchedUser = UserDao::getUserByUsername(username);
    qDebug() << "Fetched user:" << fetchedUser.getName()
             << "Type:" << fetchedUser.getUserTypeString();
    if (fetchedUser.getUserType() == User::ADMIN) {
        this->setupAdminUI();
    } else {
        this->setupReaderUI();
    }

    // 跳转到对应视图
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::setupReaderUI() {
    ui->labelUserType->setText("reader");
}

void Widget::setupAdminUI() {
    ui->labelUserType->setText("admin");

}

void Widget::onbtnLogoutClicked() {
    ui->stackedWidget->setCurrentIndex(0);

}

void Widget::onPushButtonClicked() {
    ui->label->setText("Database Connected!");
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

                qDebug() << id << "\t" << username << "\t" << name << "\t"
                         << type;
            }
        } else {
            qDebug() << "Query error:" << query.lastError().text();
        }
        db.close();
    } else {
        qDebug() << "Failed to get database connection";
    }
}





