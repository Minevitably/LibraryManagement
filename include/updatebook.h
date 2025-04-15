//
// Created by 1 on 2025/4/15.
//

#ifndef LIBRARYMANAGEMENT_UPDATEBOOK_H
#define LIBRARYMANAGEMENT_UPDATEBOOK_H

#include <QDialog>
#include "book.h"
#include <QDebug>
#include "bookdao.h"

QT_BEGIN_NAMESPACE
namespace Ui { class updatebook; }
QT_END_NAMESPACE

class updatebook : public QDialog {
Q_OBJECT

public:
    explicit updatebook(QWidget *parent = nullptr);

    ~updatebook() override;

    void setBookData(const Book &book);

private slots:

    void onBtnOKClicked();

    void onBtnCancelClicked();

private:
    Ui::updatebook *ui;
    Book m_originalBook; // 保存原始图书信息
};


#endif //LIBRARYMANAGEMENT_UPDATEBOOK_H
