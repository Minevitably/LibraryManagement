//
// Created by 1 on 2025/4/16.
//

// You may need to build the project (run Qt uic code generator) to get "ui_adduser.h" resolved

#include "adduser.h"
#include "ui_adduser.h"


adduser::adduser(QWidget *parent) :
        QWidget(parent), ui(new Ui::adduser) {
    ui->setupUi(this);

    connect(ui->btnOK, &QPushButton::clicked,
            this, &adduser::onBtnOKClicked);
    connect(ui->btnCancel, &QPushButton::clicked,
            this, &adduser::onBtnCancelClicked);
}

adduser::~adduser() {
    delete ui;
}

void adduser::onBtnOKClicked() {
    qDebug() << "用户确认";

    // 可以在这里添加确认后的处理逻辑
    // 例如保存修改、发送信号等
    QString username = ui->lineEditUsername->text();
    QString name = ui->lineEditName->text();
    QString password = ui->lineEditPassword->text();
    User::UserType usertype = ui->comboBoxUserType->currentIndex() == 0 ?
            User::UserType::ADMIN : User::UserType::READER;
    // 创建借阅记录
    QDateTime createDate = QDateTime::currentDateTime();
    QDateTime updateDate = QDateTime::currentDateTime();
    User user = User(-1, username, password, name, usertype, createDate,
                     updateDate);
    if(UserDao::addUser(user)) {
        emit userAdded();  // 添加成功才发射信号
    }
    // 关闭窗口并返回Accepted状态
//    this->accept();
    this->close();

}

void adduser::onBtnCancelClicked() {
    qDebug() << "用户取消";

    // 可以在这里添加取消后的处理逻辑

    // 关闭窗口并返回Rejected状态
//    this->reject();
    this->close();
}
