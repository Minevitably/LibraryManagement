//
// Created by 1 on 2025/4/15.
//

#ifndef LIBRARYMANAGEMENT_BOOKDETAIL_H
#define LIBRARYMANAGEMENT_BOOKDETAIL_H

#include <QWidget>
#include <QDebug>
#include "book.h"


QT_BEGIN_NAMESPACE
namespace Ui { class bookdetail; }
QT_END_NAMESPACE

class bookdetail : public QWidget {
Q_OBJECT

public:
    explicit bookdetail(QWidget *parent = nullptr);

    ~bookdetail() override;
    void setBookData(const Book &book);

private slots:
    void onBtnOKClicked();
    void onBtnCancelClicked();
private:
    Ui::bookdetail *ui;
};


#endif //LIBRARYMANAGEMENT_BOOKDETAIL_H
