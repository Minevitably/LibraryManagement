#include <QMessageBox>
#include "widget.h"
#include "ui_widget.h"
#include "dbutil.h"
#include "userdao.h"
#include "book.h"
#include "bookdao.h"
#include "bookdetail.h"
#include "borrowrecorddao.h"
#include "updatebook.h"

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
    connect(ui->btnDetailAdmin, &QPushButton::clicked,
            this, &Widget::onBtnDetailClicked);

    // 借阅按钮
    connect(ui->btnBorrow, &QPushButton::clicked,
            this, &Widget::onBtnBorrowClicked);

    // 图书删除按钮
    connect(ui->btnDeleteBook, &QPushButton::clicked,
            this, &Widget::onBtnDeleteBookClicked);

    // 更新图书信息按钮
    connect(ui->btnUpdateBook, &QPushButton::clicked,
            this, &Widget::onBtnUpdateBookClicked);

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
    const QString &configPath = "resources/remember_me.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    // 检查配置文件是否存在
    if (!QFile::exists(configPath)) {
        qDebug() << "Warning: remember me configuration file not found.";
        return;
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
        // 登录失败，弹出对话框提示
        qDebug() << "username or password is wrong.";
        QMessageBox::warning(this, "登录失败", "用户名或密码不正确"
                                               ".");
        return;
    }

    // 获取用户身份
    User fetchedUser = UserDao::getUserByUsername(username);
    qDebug() << "Fetched user:" << fetchedUser.getName()
             << "Type:" << fetchedUser.getUserTypeString();
    m_currentUser = fetchedUser;

    if (fetchedUser.getUserType() == User::ADMIN) {
        this->setupAdminUI();
    } else {
        this->setupReaderUI();
    }

    // 登录成功，弹出对话框提示
    QMessageBox::information(this, "登录成功",
                             "欢迎，" + fetchedUser.getName() + "!");

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
    // 普通用户的图书检索视图底部两个按钮为详情和借阅
    ui->stackedWidgetSearch->setCurrentIndex(0);

}

/**
 * 设置登录成功后的管理员视图
 */
void Widget::setupAdminUI() {
    ui->labelUserType->setText("admin");
    // 管理员的的图书检索视图底部两个按钮为删除和更新图书信息
    ui->stackedWidgetSearch->setCurrentIndex(1);

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


void Widget::refreshBookList() {
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
            case OPTION_ISBN: {
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
    for (const Book &book: bookList) {
        int row = ui->tableWidgetSearch->rowCount();
        ui->tableWidgetSearch->insertRow(row);

        ui->tableWidgetSearch->setItem(row, 0,
                                       new QTableWidgetItem(book.getTitle()));
        ui->tableWidgetSearch->setItem(row, 1,
                                       new QTableWidgetItem(book.getAuthor()));
        ui->tableWidgetSearch->setItem(row, 2,
                                       new QTableWidgetItem(book.getIsbn()));
        ui->tableWidgetSearch->setItem(row, 3, new QTableWidgetItem(
                QString::number(book.getQuantity())));
    }

    // 自动调整列宽
    ui->tableWidgetSearch->resizeColumnsToContents();

    // 显示检索结果数量
    qDebug() << "Found" << bookList.size() << "books.";
}

/**
 * 检索图书
 */
void Widget::onBtnSearchClicked() {
    this->refreshBookList();
}


Book Widget::getSelectedBook() {
    // 获取当前选中的行
    int currentRow = ui->tableWidgetSearch->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择一本书籍");
        return Book(); // 返回一个无效的 Book 对象
    }

    // 从表格中获取 ISBN
    QString isbn = ui->tableWidgetSearch->item(currentRow, 2)->text();
    // 获取图书详细信息
    Book fetchedBook = BookDao::getBookByIsbn(isbn);
    if (fetchedBook.getId() == -1) {
        QMessageBox::warning(this, "错误", "获取图书信息失败");
        return Book(); // 返回一个无效的 Book 对象
    }

    return fetchedBook; // 返回有效的 Book 对象
}

/**
 * 详情按钮
 * 如果在列表中选中了某项图书则弹出新窗口，显示该图书的所有信息
 * 否则弹窗提示没有选中图书
 */
void Widget::onBtnDetailClicked() {
    Book fetchedBook = getSelectedBook();
    if (fetchedBook.getId() == -1) {
        return; // 如果获取失败，直接返回
    }

    // 创建并显示详情窗口
    bookdetail *detailDialog = new bookdetail();
    detailDialog->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    detailDialog->setBookData(fetchedBook);
    detailDialog->show();
}

/**
 * 借阅按钮
 */
void Widget::onBtnBorrowClicked() {
    Book fetchedBook = getSelectedBook();
    if (fetchedBook.getId() == -1) {
        return; // 如果获取失败，直接返回
    }

    // 检查图书库存
    if (fetchedBook.getQuantity() <= 0) {
        QMessageBox::warning(this, "提示", "该图书已无库存");
        return;
    }

    // 检查图书是否可借
    if (!BorrowRecordDao::isBookAvailable(fetchedBook.getId(),
                                          m_currentUser.getId())) {
        QMessageBox::warning(this, "提示", "该图书已被借出");
        return;
    }

    // 检查用户借书数量限制
    int borrowCount = BorrowRecordDao::getUserBorrowCount(
            m_currentUser.getId());
    if (borrowCount >= 5) {
        QMessageBox::warning(this, "提示", "您已达到最大借书数量(5本)");
        return;
    }

    // 确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认借书",
                                  QString("确认借阅《%1》吗？").arg(
                                          fetchedBook.getTitle()),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    // 创建借阅记录
    QDateTime borrowDate = QDateTime::currentDateTime();
    QDateTime dueDate = borrowDate.addDays(30);

    BorrowRecord record;
    record.setBookId(fetchedBook.getId());
    record.setUserId(m_currentUser.getId());
    record.setBorrowDate(borrowDate);
    record.setDueDate(dueDate);
    record.setStatus(BorrowRecord::BORROWED);

    // 保存借阅记录
    if (!BorrowRecordDao::addRecord(record)) {
        QMessageBox::critical(this, "错误", "借书失败，请重试");
        return;
    }

    // 更新图书库存
    if (!BookDao::updateQuantity(fetchedBook.getId(), -1)) {
        qWarning() << "更新图书库存失败，但借阅记录已创建";
    }

    // 提示借书成功
    QMessageBox::information(this, "成功",
                             QString("借书成功！\n图书:《%1》\n应还日期:%2")
                                     .arg(fetchedBook.getTitle())
                                     .arg(dueDate.toString("yyyy-MM-dd")));

    // 刷新显示
    this->refreshBookList();
}

///**
// * 详情按钮
// * 如果在列表中选中了某项图书则弹出新窗口，显示该图书的所有信息
// * 否则弹窗提示没有选中图书
// */
//void Widget::onBtnDetailClicked() {
//    // 获取当前选中的行
//    int currentRow = ui->tableWidgetSearch->currentRow();
//    if (currentRow < 0) {
//        QMessageBox::warning(this, "提示", "请先选择一本书籍");
//        return;
//    }
//
//    // 从表格中获取ISBN
//    QString isbn = ui->tableWidgetSearch->item(currentRow, 2)->text();
//
//    // 获取图书详细信息
//    Book fetchedBook = BookDao::getBookByIsbn(isbn);
//    if (fetchedBook.getId() == -1) {
//        QMessageBox::warning(this, "错误", "获取图书信息失败");
//        return;
//    }
//
//    // 创建并显示详情窗口
//    bookdetail *detailDialog = new bookdetail();
//    detailDialog->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
//    detailDialog->setBookData(fetchedBook);
//    detailDialog->show();
//}
//
//void Widget::onBtnBorrowClicked() {
//    // 获取当前选中的行
//    int currentRow = ui->tableWidgetSearch->currentRow();
//    if (currentRow < 0) {
//        QMessageBox::warning(this, "提示", "请先选择一本书籍");
//        return;
//    }
//
//    // 从表格中获取ISBN
//    QString isbn = ui->tableWidgetSearch->item(currentRow, 2)->text();
//
//    // 获取图书详细信息
//    Book fetchedBook = BookDao::getBookByIsbn(isbn);
//    if (fetchedBook.getId() == -1) {
//        QMessageBox::warning(this, "错误", "获取图书信息失败");
//        return;
//    }
//
//    // 检查图书库存
//    if (fetchedBook.getQuantity() <= 0) {
//        QMessageBox::warning(this, "提示", "该图书已无库存");
//        return;
//    }
//
//    // 检查图书是否可借
//    if (!BorrowRecordDao::isBookAvailable(fetchedBook.getId(),
//                                          m_currentUser.getId())) {
//        QMessageBox::warning(this, "提示", "该图书已被借出");
//        return;
//    }
//
//    // 检查用户借书数量限制(例如最多借5本)
//    int borrowCount = BorrowRecordDao::getUserBorrowCount(
//            m_currentUser.getId());
//    if (borrowCount >= 5) {
//        QMessageBox::warning(this, "提示", "您已达到最大借书数量(5本)");
//        return;
//    }
//
//    // 确认对话框
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(this, "确认借书",
//                                  QString("确认借阅《%1》吗？").arg(
//                                          fetchedBook.getTitle()),
//                                  QMessageBox::Yes | QMessageBox::No);
//    if (reply != QMessageBox::Yes) {
//        return;
//    }
//
//    // 创建借阅记录
//    QDateTime borrowDate = QDateTime::currentDateTime();
//    QDateTime dueDate = borrowDate.addDays(30); // 假设借期30天
//
//    BorrowRecord record;
//    record.setBookId(fetchedBook.getId());
//    record.setUserId(m_currentUser.getId());
//    record.setBorrowDate(borrowDate);
//    record.setDueDate(dueDate);
//    record.setStatus(BorrowRecord::BORROWED);
//
//    // 保存借阅记录
//    if (!BorrowRecordDao::addRecord(record)) {
//        QMessageBox::critical(this, "错误", "借书失败，请重试");
//        return;
//    }
//
//    // 更新图书库存(如果需要)
//    if (!BookDao::updateQuantity(fetchedBook.getId(), -1)) {
//        qWarning() << "更新图书库存失败，但借阅记录已创建";
//    }
//
//    // 提示借书成功
//    QMessageBox::information(this, "成功",
//                             QString("借书成功！\n图书:《%1》\n应还日期:%2")
//                                     .arg(fetchedBook.getTitle())
//                                     .arg(dueDate.toString("yyyy-MM-dd")));
//
//    // 刷新显示
////    refreshBookList();
//}

/**
 * 删除按钮
 */
void Widget::onBtnDeleteBookClicked() {
    Book fetchedBook = getSelectedBook();
    if (fetchedBook.getId() == -1) {
        return; // 如果获取失败，直接返回
    }

    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除图书《%1》吗？").arg(
                                          fetchedBook.getTitle()),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return; // 用户选择了“否”，则不执行删除
    }

    // 执行删除操作
    if (BookDao::deleteBook(fetchedBook.getId())) {
        QMessageBox::information(this, "成功", "图书删除成功。");
    } else {
        QMessageBox::critical(this, "错误", "图书删除失败，请重试。");
    }

    // 刷新显示
     refreshBookList(); // 可以根据需要刷新书籍列表
}

void Widget::onBtnUpdateBookClicked() {
    Book fetchedBook = getSelectedBook();
    if (fetchedBook.getId() == -1) {
        return; // 如果获取失败，直接返回
    }
    // 创建并显示详情窗口
    updatebook *updatebookDialog = new updatebook();
    updatebookDialog->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    updatebookDialog->setBookData(fetchedBook);
    // 连接 accepted 信号
    connect(updatebookDialog, &QDialog::accepted, this, &Widget::refreshBookList);

    updatebookDialog->show();

}




