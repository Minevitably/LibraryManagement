#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
Q_OBJECT

public:
    // 视图
    enum ViewType {
        VIEW_SEARCH_BOOK, // 图书检索视图
        VIEW_BORROW_DETAIL, // 借阅详情视图
        VIEW_SELF_INFO, // 个人信息视图
        VIEW_ADD_BOOK // 图书入库视图
    };
    // 页面
    // 系统只包含两个页面
    enum PageType {
        PAGE_LOGIN, // 登录页面/视图
        PAGE_MAIN // 主页面
    };

    // 检索选项
    // 可以使用三种方式检索图书
    enum SearchOption {
        OPTION_TITLE, // 书名
        OPTION_AUTHOR, //作者
        OPTION_ISBN // ISBN
    };

    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    void onRememberMe();
    void setupReaderUI();
    void setupAdminUI();


private slots:
    void onPushButtonLoginClicked();
    void onBtnSearchBookClicked();
    void onBtnSearchClicked();
    void onBtnDetailClicked();

    void onBtnLogoutClicked();
    void onPushButtonClicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H