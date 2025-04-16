//
// Created by 1 on 2025/4/16.
//

#ifndef LIBRARYMANAGEMENT_ADDUSER_H
#define LIBRARYMANAGEMENT_ADDUSER_H

#include <QWidget>
#include <QDebug>
#include "user.h"
#include "userdao.h"

QT_BEGIN_NAMESPACE
namespace Ui { class adduser; }
QT_END_NAMESPACE

class adduser : public QWidget {
Q_OBJECT

public:
    explicit adduser(QWidget *parent = nullptr);

    ~adduser() override;
signals:
    void userAdded();  // 新增信号，当用户成功添加时发射
private slots:
    void onBtnOKClicked();
    void onBtnCancelClicked();

private:
    Ui::adduser *ui;
};


#endif //LIBRARYMANAGEMENT_ADDUSER_H
