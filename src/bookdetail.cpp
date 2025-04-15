//
// Created by 1 on 2025/4/15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_bookdetail.h" resolved

#include "bookdetail.h"
#include "ui_bookdetail.h"


bookdetail::bookdetail(QWidget *parent) :
        QWidget(parent), ui(new Ui::bookdetail) {
    ui->setupUi(this);

    connect(ui->btnOK, &QPushButton::clicked,
            this, &bookdetail::onBtnOKClicked);
    connect(ui->btnCancel, &QPushButton::clicked,
            this, &bookdetail::onBtnCancelClicked);
}

bookdetail::~bookdetail() {
    delete ui;
}
void bookdetail::setBookData(const Book &book) {
    // 设置文本字段
    ui->labelTitle->setText(book.getTitle());
    ui->labelAuthor->setText(book.getAuthor());
    ui->labelIsbn->setText(book.getIsbn());
    ui->labelPublisher->setText(book.getPublisher());
    ui->labelDescription->setText(book.getDescription());
    ui->labelLanguage->setText(book.getLanguage());

    // 处理日期字段 - 转换为字符串
    QDate publishDate = book.getPublishDate();
    if (publishDate.isValid()) {
        ui->labelPublishDate->setText(publishDate.toString("yyyy-MM-dd"));
    } else {
        ui->labelPublishDate->setText("未知日期");
    }

    // 处理数量字段 - 转换为字符串
    ui->labelQuantity->setText(QString::number(book.getQuantity()));
}

void bookdetail::onBtnOKClicked() {
    qDebug() << "用户确认";

    // 可以在这里添加确认后的处理逻辑
    // 例如保存修改、发送信号等

    // 关闭窗口并返回Accepted状态
//    this->accept();
    this->close();

}

void bookdetail::onBtnCancelClicked() {
    qDebug() << "用户取消";

    // 可以在这里添加取消后的处理逻辑

    // 关闭窗口并返回Rejected状态
//    this->reject();
    this->close();
}
