//
// Created by 1 on 2025/4/15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_updatebook.h" resolved

#include <QMessageBox>
#include "updatebook.h"
#include "ui_updatebook.h"


updatebook::updatebook(QWidget *parent) :
        QDialog(parent), ui(new Ui::updatebook) {
    ui->setupUi(this);

    connect(ui->btnOK, &QPushButton::clicked,
            this, &updatebook::onBtnOKClicked);
    connect(ui->btnCancel, &QPushButton::clicked,
            this, &updatebook::onBtnCancelClicked);
}

updatebook::~updatebook() {
    delete ui;
}
void updatebook::setBookData(const Book &book) {
    // 保存原始图书信息
    m_originalBook = book;

    // 设置文本字段
    ui->lineEditTitle->setText(book.getTitle());
    ui->lineEditAuthor->setText(book.getAuthor());
    ui->lineEditIsbn->setText(book.getIsbn());
    ui->lineEditPublisher->setText(book.getPublisher());
    ui->textEditDescription->setText(book.getDescription());
    ui->lineEditLanguage->setText(book.getLanguage());

    // 处理日期字段 - 转换为字符串
    QDate publishDate = book.getPublishDate();
    if (publishDate.isValid()) {
        ui->lineEditPublishDate->setText(publishDate.toString("yyyy-MM-dd"));
    } else {
        ui->lineEditPublishDate->setText("");
        ui->lineEditPublishDate->setPlaceholderText("请输入日期 (yyyy-MM-dd)");
    }

    // 处理数量字段 - 转换为字符串
    ui->lineEditQuantity->setText(QString::number(book.getQuantity()));
}

void updatebook::onBtnOKClicked() {
    qDebug() << "用户确认";

    // 创建更新后的图书对象
    Book updatedBook = m_originalBook;

    // 获取并验证标题
    QString title = ui->lineEditTitle->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "错误", "书名不能为空");
        return;
    }
    updatedBook.setTitle(title);

    // 获取并验证作者
    QString author = ui->lineEditAuthor->text().trimmed();
    if (author.isEmpty()) {
        QMessageBox::warning(this, "错误", "作者不能为空");
        return;
    }
    updatedBook.setAuthor(author);

    // 获取并验证ISBN
    QString isbn = ui->lineEditIsbn->text().trimmed();
    if (isbn.isEmpty()) {
        QMessageBox::warning(this, "错误", "ISBN不能为空");
        return;
    }
    updatedBook.setIsbn(isbn);

    // 获取其他可选字段
    updatedBook.setPublisher(ui->lineEditPublisher->text().trimmed());
    updatedBook.setDescription(ui->textEditDescription->toPlainText().trimmed());
    updatedBook.setLanguage(ui->lineEditLanguage->text().trimmed());

    // 处理出版日期
    QString dateStr = ui->lineEditPublishDate->text().trimmed();
    if (!dateStr.isEmpty()) {
        QDate publishDate = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (!publishDate.isValid()) {
            QMessageBox::warning(this, "错误", "日期格式不正确，请使用yyyy-MM-dd格式");
            return;
        }
        updatedBook.setPublishDate(publishDate);
    } else {
        updatedBook.setPublishDate(QDate()); // 设置为无效日期
    }

    // 处理库存数量
    bool ok;
    int quantity = ui->lineEditQuantity->text().toInt(&ok);
    if (!ok || quantity < 0) {
        QMessageBox::warning(this, "错误", "库存数量必须是非负整数");
        return;
    }
    updatedBook.setQuantity(quantity);

    // 更新图书信息
    if (!BookDao::updateBook(updatedBook)) {
        QMessageBox::critical(this, "错误", "更新图书信息失败");
        return;
    }
    QMessageBox::information(this, "提示", "图书信息更新成功");



    // 发送信号通知父窗口更新成功
//    emit bookUpdated(updatedBook);

    // 关闭窗口并返回Accepted状态
    this->accept();
}

void updatebook::onBtnCancelClicked() {
    qDebug() << "用户取消";

    // 可以在这里添加取消后的处理逻辑

    // 关闭窗口并返回Rejected状态
    this->reject();
//    this->close();
}
