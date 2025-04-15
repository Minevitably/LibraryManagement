#include <QMessageBox>
#include "widget.h"
#include "ui_widget.h"
#include "dbutil.h"
#include "userdao.h"
#include "book.h"
#include "bookdao.h"


/**
 * 构造函数
 * @param parent
 */
Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 登录按钮
    connect(ui->pushButtonLogin, &QPushButton::clicked,
            this, &Widget::onPushButtonLoginClicked);

    // 登出按钮
    connect(ui->btnLogout, &QPushButton::clicked,
            this, &Widget::onBtnLogoutClicked);

    // 图书检索按钮（切换到图书检索视图）
    connect(ui->btnSearchBook, &QPushButton::clicked,
            this, &Widget::onBtnSearchBookClicked);

    // 检索按钮（检索图书）
    connect(ui->btnSearch, &QPushButton::clicked,
            this, &Widget::onBtnSearchClicked);

    // 详情按钮
    connect(ui->btnDetail, &QPushButton::clicked,
            this, &Widget::onBtnDetailClicked);


    connect(ui->pushButton, &QPushButton::clicked,
            this, &Widget::onPushButtonClicked);

    // 默认位于登录页面
    ui->stackedWidget->setCurrentIndex(PAGE_LOGIN);

    this->onRememberMe();

}

/**
 * 析构函数
 */
Widget::~Widget() {
    delete ui;
}

/**
 * 记住密码
 * 在程序启动后执行一次，使用配置文件保存状态
 */
void Widget::onRememberMe() {
    // 如果resources/remember_me.ini文件存在，并且flag=1
    // 则说明上次用户选择了记住密码，
    // 此时页面会读取resources/remember_me.ini文件并自动填写账号密码
    const QString& configPath = "resources/remember_me.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    // 检查配置文件是否存在
    if (!QFile::exists(configPath)) {
        qDebug() << "Warning: remember me configuration file not found.";
        return ;
    }


    // 读取配置信息

    int flag = settings.value("remember_me/flag").toInt();
    if (!flag) {
        qDebug() << "Debug: remember me is not checked last time.";
        return;
    }
    ui->checkBoxRemeberMe->setChecked(true);

    QString username = settings.value("remember_me/username").toString();
    QString password = settings.value("remember_me/password").toString();
    ui->lineEditUserName->setText(username);
    ui->lineEditPassword->setText(password);

    qDebug() << "Debug: remember me is done.";
    qDebug() << "UserName" << "\t" << ui->lineEditUserName->text();
    qDebug() << "Password" << "\t" << ui->lineEditPassword->text();

}

/**
 * 登录按钮
 * 登录成功后跳转到图书检索视图
 */
void Widget::onPushButtonLoginClicked() {
    qDebug() << "Login clicked.";

    QString username = ui->lineEditUserName->text();
    QString password = ui->lineEditPassword->text();

    qDebug() << "UserName" << "\t" << username;
    qDebug() << "Password" << "\t" << password;

    QSettings settings("resources/remember_me.ini", QSettings::IniFormat);

    if (ui->checkBoxRemeberMe->isChecked()) {
        // 如果用户勾选记住密码，更新 INI 文件
        settings.setValue("remember_me/flag", 1);
        settings.setValue("remember_me/username", username);
        settings.setValue("remember_me/password", password);
        qDebug() << "Debug: Password remembered.";
    } else {
        // 否则更新 flag 为 0
        settings.setValue("remember_me/flag", 0);
        qWarning() << "Debug: Password not remembered.";
    }

    // 验证用户名以及密码
    bool valid = UserDao::validateUser(username, password);
    qDebug() << "Validate user:" << (valid ? "Valid" : "Invalid");
    if (!valid) {
        // 登录失败
        qDebug() << "username or password is wrong.";
        return;
    }
    // 获取用户身份
    User fetchedUser = UserDao::getUserByUsername(username);
    qDebug() << "Fetched user:" << fetchedUser.getName()
             << "Type:" << fetchedUser.getUserTypeString();
    if (fetchedUser.getUserType() == User::ADMIN) {
        this->setupAdminUI();
    } else {
        this->setupReaderUI();
    }

    // 从登录页面跳转到主页面
    ui->stackedWidget->setCurrentIndex(PAGE_MAIN);
    // 图书检索视图
    ui->stackedWidgetMain->setCurrentIndex(VIEW_SEARCH_BOOK);
}

/**
 * 设置登录成功后的读者视图
 */
void Widget::setupReaderUI() {
    ui->labelUserType->setText("reader");

}

/**
 * 设置登录成功后的管理员视图
 */
void Widget::setupAdminUI() {
    ui->labelUserType->setText("admin");

}

/**
 * 退出登录按钮
 * 返回到登录页面
 */
void Widget::onBtnLogoutClicked() {
    ui->stackedWidget->setCurrentIndex(PAGE_LOGIN);

}

/**
 * 没什么用的按钮，用来测试数据库是否可用
 */
void Widget::onPushButtonClicked() {
    ui->label->setText("Database Connected!");
    // 获取数据库连接
    QSqlDatabase db = DBUtil::getConnection();
    if (db.isValid()) {
        QSqlQuery query(db);
        if (query.exec("SELECT * FROM users")) {
            while (query.next()) {
                // 处理查询结果...
                int id = query.value(0).toInt();
                QString username = query.value(1).toString();
                QString name = query.value(2).toString();
                QString type = query.value(3).toString();

                qDebug() << id << "\t" << username << "\t" << name << "\t"
                         << type;
            }
        } else {
            qDebug() << "Query error:" << query.lastError().text();
        }
        db.close();
    } else {
        qDebug() << "Failed to get database connection";
    }
}

/**
 * 图书检索按钮
 * 切换到图书检索视图
 */
void Widget::onBtnSearchBookClicked() {
    ui->stackedWidgetMain->setCurrentIndex(VIEW_SEARCH_BOOK);
}


/**
 * 检索图书
 */
void Widget::onBtnSearchClicked() {
    int option = ui->comboBoxSearch->currentIndex();
    QString searchText = ui->lineEditSearch->text().trimmed();

    QList<Book> bookList;

    // 如果检索文本为空，则检索所有书籍
    if (searchText.isEmpty()) {
        bookList = BookDao::getAllBooks();
    } else {
        // 根据下拉框选项进行检索
        switch (option) {
            case OPTION_TITLE:
                bookList = BookDao::searchBooksByTitle(searchText);
                break;
            case OPTION_AUTHOR:
                bookList = BookDao::searchBooksByAuthor(searchText);
                break;
            case OPTION_ISBN:
            {
                Book book = BookDao::getBookByIsbn(searchText);
                if (book.getId() != -1) {
                    bookList.append(book);
                }
            }
                break;
            default:
                qDebug() << "Wrong Option.";
                return;
        }
    }

    // 清空表格
    ui->tableWidgetSearch->clearContents();
    ui->tableWidgetSearch->setRowCount(0);

    // 设置表格列数和标题
    ui->tableWidgetSearch->setColumnCount(4);
    QStringList headers;
    headers << "书名" << "作者" << "ISBN" << "剩余数量";
    ui->tableWidgetSearch->setHorizontalHeaderLabels(headers);

    // 填充表格数据
    for (const Book &book : bookList) {
        int row = ui->tableWidgetSearch->rowCount();
        ui->tableWidgetSearch->insertRow(row);

        ui->tableWidgetSearch->setItem(row, 0, new QTableWidgetItem(book.getTitle()));
        ui->tableWidgetSearch->setItem(row, 1, new QTableWidgetItem(book.getAuthor()));
        ui->tableWidgetSearch->setItem(row, 2, new QTableWidgetItem(book.getIsbn()));
        ui->tableWidgetSearch->setItem(row, 3, new QTableWidgetItem(QString::number(book.getQuantity())));
    }

    // 自动调整列宽
    ui->tableWidgetSearch->resizeColumnsToContents();

    // 显示检索结果数量
    qDebug() << "Found" << bookList.size() << "books.";
}


#include "bookdetail.h"
/**
 * 详情按钮
 * 如果在列表中选中了某项图书则弹出新窗口，显示该图书的所有信息
 * 否则弹窗提示没有选中图书
 */
void Widget::onBtnDetailClicked() {
    // 获取当前选中的行
    int currentRow = ui->tableWidgetSearch->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择一本书籍");
        return;
    }

    // 从表格中获取ISBN
    QString isbn = ui->tableWidgetSearch->item(currentRow, 2)->text();

    // 获取图书详细信息
    Book fetchedBook = BookDao::getBookByIsbn(isbn);
    if (fetchedBook.getId() == -1) {
        QMessageBox::warning(this, "错误", "获取图书信息失败");
        return;
    }

    // 创建并显示详情窗口
    bookdetail *detailDialog = new bookdetail();
    detailDialog->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    detailDialog->setBookData(fetchedBook);
    detailDialog->show();
}





