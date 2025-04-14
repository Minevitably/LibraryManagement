#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool connectToDatabase();  // 数据库连接方法
    void queryUsers();         // 查询用户方法

private slots:
    void onPushButtonClicked();

private:
    Ui::Widget *ui;
    QSqlDatabase db;  // 数据库连接对象
};
#endif // WIDGET_H