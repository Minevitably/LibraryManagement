#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::onPushButtonClicked);

}

Widget::~Widget()
{
    delete ui;
}

// 槽函数，处理按钮点击事件
void Widget::onPushButtonClicked()
{
    ui->label->setText("Hello World");
}
