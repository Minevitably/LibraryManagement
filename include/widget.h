#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "user.h"
#include "book.h"
#include "borrowrecord.h"

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
        VIEW_ADD_BOOK, // 图书入库视图
        VIEW_USER_MANAGEMENT // 用户管理视图
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
    Book getSelectedBook();
    BorrowRecord getSelectedBorrowRecord();
    User getSelectedUser();
    void refreshBookList();
    void refreshUserList();


private slots:
    void onPushButtonLoginClicked();

    // 视图切换
    void onBtnSearchBookClicked();
    void onBtnBorDetailClicked();
    void onBtnSelfClicked();
    void onBtnAddBookClicked();
    void onBtnUserMgmtClicked();

    // 视图内的按钮
    void onBtnSearchClicked();
    void onBtnDetailClicked();
    void onBtnBorrowClicked();
    void onBtnDeleteBookClicked();
    void onBtnUpdateBookClicked();
    void onBtnBookDetailClicked();
    void onBtnReturnClicked();
    void onBtnUpdateSelfInfoClicked();
    void onBtnDoAddBookClicked();
    void onBtnDeleteUserClicked();
    void onBtnAddUserClicked();

    void onBtnLogoutClicked();
    void onPushButtonClicked();

private:
    Ui::Widget *ui;
    User m_currentUser;
};
#endif // WIDGET_H