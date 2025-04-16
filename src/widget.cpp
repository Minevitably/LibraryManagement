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
#include "adduser.h"

/**
 * 构造函数
 * @param parent
 */
Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    this->setWindowTitle("图书管理系统");
    // 登录按钮
    connect(ui->pushButtonLogin, &QPushButton::clicked,
            this, &Widget::onPushButtonLoginClicked);
    // 登出按钮
//    connect(ui->btnLogout, &QPushButton::clicked,
//            this, &Widget::onBtnLogoutClicked);
    // 图书检索按钮（切换到图书检索视图）
    connect(ui->btnSearchBook, &QPushButton::clicked,
            this, &Widget::onBtnSearchBookClicked);
    // 借阅详情按钮（切换到借阅详情视图）
    connect(ui->btnBorDetail, &QPushButton::clicked,
            this, &Widget::onBtnBorDetailClicked);
    // 个人信息按钮（切换到个人信息视图）
    connect(ui->btnSelf, &QPushButton::clicked,
            this, &Widget::onBtnSelfClicked);
    // 图书入库按钮（切换到图书入库视图）
    connect(ui->btnAddBook, &QPushButton::clicked,
            this, &Widget::onBtnAddBookClicked);
    // 用户管理按钮（切换到用户管理视图）
    connect(ui->btnUserMgmt, &QPushButton::clicked,
            this, &Widget::onBtnUserMgmtClicked);

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
    // 借阅详情视图下的详情按钮
    connect(ui->btnBookDetail, &QPushButton::clicked,
            this, &Widget::onBtnBookDetailClicked);
    // 归还图书按钮
    connect(ui->btnReturn, &QPushButton::clicked,
            this, &Widget::onBtnReturnClicked);
    // 更新个人信息按钮
    connect(ui->btnUpdateSelfInfo, &QPushButton::clicked,
            this, &Widget::onBtnUpdateSelfInfoClicked);
    // 图书入库按钮
    connect(ui->btnDoAddBook, &QPushButton::clicked,
            this, &Widget::onBtnDoAddBookClicked);


    // 删除用户按钮
    connect(ui->btnDeleteUser, &QPushButton::clicked,
            this, &Widget::onBtnDeleteUserClicked);

    // 注册用户按钮
    connect(ui->btnAddUser, &QPushButton::clicked,
            this, &Widget::onBtnAddUserClicked);


//    connect(ui->pushButton, &QPushButton::clicked,
//            this, &Widget::onPushButtonClicked);

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
        QMessageBox::warning(this, "登录失败", "用户名或密码不正确.");
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
//    ui->labelUserType->setText("reader");
    // 普通用户的图书检索视图底部两个按钮为详情和借阅
    ui->stackedWidgetSearch->setCurrentIndex(0);

    // 普通用户没有图书入库按钮
    ui->btnAddBook->hide();
    // 普通用户没有用户管理按钮
    ui->btnUserMgmt->hide();

}

/**
 * 设置登录成功后的管理员视图
 */
void Widget::setupAdminUI() {
//    ui->labelUserType->setText("admin");
    // 管理员的的图书检索视图底部两个按钮为删除和更新图书信息
    ui->stackedWidgetSearch->setCurrentIndex(1);
    ui->btnAddBook->show();
    ui->btnUserMgmt->show();

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
//    ui->label->setText("Database Connected!");
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
 * 借阅详情按钮
 * 切换到借阅详情视图
 */
void Widget::onBtnBorDetailClicked() {
    ui->stackedWidgetMain->setCurrentIndex(VIEW_BORROW_DETAIL);

    // 清空当前表格内容
    ui->tableWidgetBor->clearContents();
    ui->tableWidgetBor->setRowCount(0); // 重置行数

    QList<BorrowRecord> records;

    // 根据用户类型获取借阅记录
    if (m_currentUser.getUserType() == User::ADMIN) {
        records = BorrowRecordDao::getAllRecords();
        // 在表格中显示借阅信息
        for (const BorrowRecord &record: records) {
            Book book = BookDao::getBookById(record.getBookId());
            User user = UserDao::getUserById(record.getUserId());

            int rowCount = ui->tableWidgetBor->rowCount();
            ui->tableWidgetBor->insertRow(rowCount); // 插入新行

            // 隐藏借阅ID、图书ID和用户ID列
            ui->tableWidgetBor->setItem(rowCount, 6, new QTableWidgetItem
                    (QString::number(record.getId()))); // 借阅ID
            ui->tableWidgetBor->setItem(rowCount, 7, new QTableWidgetItem
                    (QString::number(book.getId()))); // 图书ID
            ui->tableWidgetBor->setItem(rowCount, 8, new QTableWidgetItem
                    (QString::number(user.getId()))); // 用户ID

            // 显示借阅信息
//            ui->tableWidgetBor->setItem(rowCount, 3, new QTableWidgetItem(user.getUsername())); // 用户名
            ui->tableWidgetBor->setItem(rowCount, 0, new QTableWidgetItem
                    (user.getName())); // 用户姓名
            ui->tableWidgetBor->setItem(rowCount, 1, new QTableWidgetItem
                    (book.getTitle())); // 书名
            ui->tableWidgetBor->setItem(rowCount, 2, new QTableWidgetItem
                    (record.getBorrowDate().toString("yyyy-MM-dd"))); // 借出日期
            ui->tableWidgetBor->setItem(rowCount, 3, new QTableWidgetItem
                    (record.getDueDate().toString("yyyy-MM-dd"))); // 应还日期
            ui->tableWidgetBor->setItem(rowCount, 4, new QTableWidgetItem
                    (record.getReturnDate().toString("yyyy-MM-dd"))); // 归还日期
            ui->tableWidgetBor->setItem(rowCount, 5, new QTableWidgetItem
                    (record.getStatusString())); // 状态

            ui->tableWidgetBor->setColumnHidden(0, false); // 隐藏读者列
            ui->tableWidgetBor->setColumnHidden(6, true); // 隐藏借阅ID列
            ui->tableWidgetBor->setColumnHidden(7, true); // 隐藏图书ID列
            ui->tableWidgetBor->setColumnHidden(8, true); // 隐藏用户ID列
        }
    } else {
        records = BorrowRecordDao::getRecordsByUserId(m_currentUser.getId());
        // 在表格中显示借阅信息
        for (const BorrowRecord &record: records) {
            Book book = BookDao::getBookById(record.getBookId());
            User user = UserDao::getUserById(record.getUserId());

            int rowCount = ui->tableWidgetBor->rowCount();
            ui->tableWidgetBor->insertRow(rowCount); // 插入新行

            // 隐藏借阅ID、图书ID和用户ID列
            ui->tableWidgetBor->setItem(rowCount, 6, new QTableWidgetItem
                    (QString::number(record.getId()))); // 借阅ID
            ui->tableWidgetBor->setItem(rowCount, 7, new QTableWidgetItem
                    (QString::number(book.getId()))); // 图书ID
            ui->tableWidgetBor->setItem(rowCount, 8, new QTableWidgetItem
                    (QString::number(user.getId()))); // 用户ID

            // 显示借阅信息
//            ui->tableWidgetBor->setItem(rowCount, 3, new QTableWidgetItem(user.getUsername())); // 用户名
//            ui->tableWidgetBor->setItem(rowCount, 4, new QTableWidgetItem(user.getName())); // 用户姓名
            ui->tableWidgetBor->setItem(rowCount, 1, new QTableWidgetItem
                    (book.getTitle())); // 书名
            ui->tableWidgetBor->setItem(rowCount, 2, new QTableWidgetItem
                    (record.getBorrowDate().toString("yyyy-MM-dd"))); // 借出日期
            ui->tableWidgetBor->setItem(rowCount, 3, new QTableWidgetItem
                    (record.getDueDate().toString("yyyy-MM-dd"))); // 应还日期
            ui->tableWidgetBor->setItem(rowCount, 4, new QTableWidgetItem
                    (record.getReturnDate().toString("yyyy-MM-dd"))); // 归还日期
            ui->tableWidgetBor->setItem(rowCount, 5, new QTableWidgetItem
                    (record.getStatusString())); // 状态

            ui->tableWidgetBor->setColumnHidden(0, true); // 隐藏读者列
            ui->tableWidgetBor->setColumnHidden(6, true); // 隐藏借阅ID列
            ui->tableWidgetBor->setColumnHidden(7, true); // 隐藏图书ID列
            ui->tableWidgetBor->setColumnHidden(8, true); // 隐藏用户ID列
        }
    }

    // 遍历所有单元格
    for (int row = 0; row < ui->tableWidgetBor->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidgetBor->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidgetBor->item(row, col);
            if (!item) {
                item = new QTableWidgetItem();
                ui->tableWidgetBor->setItem(row, col, item);
            }
            item->setTextAlignment(Qt::AlignCenter);  // 水平和垂直居中
        }
    }


}


/**
 * 个人信息按钮
 * 切换到个人信息视图
 */
void Widget::onBtnSelfClicked() {
    ui->stackedWidgetMain->setCurrentIndex(VIEW_SELF_INFO);
    ui->lineEditUpdateUsername->setPlaceholderText(m_currentUser.getUsername());
    ui->lineEditUpdateName->setPlaceholderText(m_currentUser.getName());
    ui->lineEditUpdatePasswod->setPlaceholderText(m_currentUser.getPassword());

}

/**
 * 图书入库按钮
 * 切换到图书入库视图
 */
void Widget::onBtnAddBookClicked() {
    ui->stackedWidgetMain->setCurrentIndex(VIEW_ADD_BOOK);

}

/**
 * 用户管理按钮
 * 切换到用户管理视图
 */
void Widget::onBtnUserMgmtClicked() {
    ui->stackedWidgetMain->setCurrentIndex(VIEW_USER_MANAGEMENT);
    this->refreshUserList();
}

/**
 * 刷新图书检索列表
 */
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
    // 遍历所有单元格
    for (int row = 0; row < ui->tableWidgetSearch->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidgetSearch->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidgetSearch->item(row, col);
            if (!item) {
                item = new QTableWidgetItem();
                ui->tableWidgetSearch->setItem(row, col, item);
            }
            item->setTextAlignment(Qt::AlignCenter);  // 水平和垂直居中
        }
    }
    // 显示检索结果数量
    qDebug() << "Found" << bookList.size() << "books.";
}

/**
 * 检索图书
 */
void Widget::onBtnSearchClicked() {
    this->refreshBookList();
}

/**
 * 获取当前选择的图书
 * @return
 */
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
 * 获取当前选中的借阅详情
 * @return
 */
BorrowRecord Widget::getSelectedBorrowRecord() {
    // 获取当前选中的行
    int currentRow = ui->tableWidgetBor->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择一条借阅记录");
        return BorrowRecord(); // 返回一个无效的 BorrowRecord 对象
    }

    // 从表格中获取 Record ID
    QString recordIdString = ui->tableWidgetBor->item(currentRow, 6)->text();

    // 将 Record ID 转换为 int
    bool conversionOk;
    int recordId = recordIdString.toInt(&conversionOk);

    if (!conversionOk) {
        QMessageBox::warning(this, "转换错误", "无效的借阅记录ID");
        return BorrowRecord(); // 返回一个无效的 BorrowRecord 对象
    }

    // 获取借阅记录详细信息
    BorrowRecord fetchedBorrowRecord = BorrowRecordDao::getRecordById(recordId);
    if (fetchedBorrowRecord.getId() == -1) {
        QMessageBox::warning(this, "错误", "获取图书信息失败");
        return BorrowRecord(); // 返回一个无效的 BorrowRecord 对象
    }

    return fetchedBorrowRecord; // 返回有效的 BorrowRecord 对象
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

/**
 * 更新图书信息按钮
 */
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
    connect(updatebookDialog, &QDialog::accepted, this,
            &Widget::refreshBookList);

    updatebookDialog->show();

}

/**
 * 归还图书
 */
void Widget::onBtnReturnClicked() {
    BorrowRecord borrowRecord = this->getSelectedBorrowRecord();
    qDebug() << "借阅id:" + borrowRecord.getId();
    if (borrowRecord.getId() == -1) {
        QMessageBox::critical(this, "错误", "无效的借阅id");
        return; // 如果获取失败，直接返回
    }

    Book fetchedBook = BookDao::getBookById(borrowRecord.getBookId());
    if (fetchedBook.getId() == -1) {
        QMessageBox::critical(this, "错误", "无效的图书id");
        return; // 如果获取失败，直接返回
    }

    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认归还",
                                  QString("确定要归还图书《%1》吗？").arg(
                                          fetchedBook.getTitle()),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        borrowRecord.setStatus(BorrowRecord::RETURNED);
        QDateTime returnDate = QDateTime::currentDateTime();
        borrowRecord.setReturnDate(returnDate);
        BorrowRecordDao::updateRecord(borrowRecord);
        this->onBtnBorDetailClicked();
        return;
    } else {
        return; // 用户选择了“否”，则不执行归还
    }

}

/**
 * 查看图书详情
 */
void Widget::onBtnBookDetailClicked() {
    BorrowRecord borrowRecord = this->getSelectedBorrowRecord();
    qDebug() << "借阅id:" + borrowRecord.getId();
    if (borrowRecord.getId() == -1) {
        QMessageBox::critical(this, "错误", "无效的借阅id");
        return; // 如果获取失败，直接返回
    }

    Book fetchedBook = BookDao::getBookById(borrowRecord.getBookId());
    if (fetchedBook.getId() == -1) {
        QMessageBox::critical(this, "错误", "无效的图书id");
        return; // 如果获取失败，直接返回
    }

    // 创建并显示详情窗口
    bookdetail *detailDialog = new bookdetail();
    detailDialog->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    detailDialog->setBookData(fetchedBook);
    detailDialog->show();
}

/**
 * 更新个人信息按钮
 */
void Widget::onBtnUpdateSelfInfoClicked() {
    User updatedUser = m_currentUser;
    updatedUser.setUsername(ui->lineEditUpdateUsername->text());
    updatedUser.setName(ui->lineEditUpdateName->text());
    updatedUser.setPassword(ui->lineEditUpdatePasswod->text());


    if (updatedUser.getUsername().isEmpty() &&
        updatedUser.getName().isEmpty() &&
        updatedUser.getPassword().isEmpty()) {
        QMessageBox::information(this, "提示", "至少需要修改一项信息");
        return;
    }

    if (updatedUser.getUsername().isEmpty() ||
        updatedUser.getName().isEmpty() ||
        updatedUser.getPassword().isEmpty()) {
        // 未填写的信息将使用默认值
        QMessageBox::information(this, "提示", "未填写的信息将使用默认值");
        if (updatedUser.getUsername().isEmpty()) {

            updatedUser.setUsername(ui->lineEditUpdateUsername->placeholderText());
            qDebug() << "getUsername:" + updatedUser.getUsername();
        }
        if (updatedUser.getName().isEmpty()) {
            updatedUser.setName(ui->lineEditUpdateName->placeholderText());
            qDebug() << "getName:" + updatedUser.getName();
        }
        if (updatedUser.getPassword().isEmpty()) {
            updatedUser.setPassword(ui->lineEditUpdatePasswod->placeholderText());
            qDebug() << "getPassword:" + updatedUser.getPassword();
        }
    }


    if (UserDao::updateUser(updatedUser)) {
        QMessageBox::information(this, "提示", "更新用户信息成功，请重新登录");
        ui->stackedWidget->setCurrentIndex(PAGE_LOGIN);

    } else {
        QMessageBox::critical(this, "错误",
                              "更新用户信息失败，建议尝试使用其它用户名");
        return;
    }


}

/**
 * 图书入库按钮
 */
void Widget::onBtnDoAddBookClicked() {
    Book book = Book();

    // 获取并验证标题
    QString title = ui->lineEditTitle->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "错误", "书名不能为空");
        return;
    }
    book.setTitle(title);

    // 获取并验证作者
    QString author = ui->lineEditAuthor->text().trimmed();
    if (author.isEmpty()) {
        QMessageBox::warning(this, "错误", "作者不能为空");
        return;
    }
    book.setAuthor(author);

    // 获取并验证ISBN
    QString isbn = ui->lineEditIsbn->text().trimmed();
    if (isbn.isEmpty()) {
        QMessageBox::warning(this, "错误", "ISBN不能为空");
        return;
    }
    book.setIsbn(isbn);

    // 获取其他可选字段
    book.setPublisher(ui->lineEditPublisher->text().trimmed());
    book.setDescription(ui->textEditDescription->toPlainText().trimmed());
    book.setLanguage(ui->lineEditLanguage->text().trimmed());

    // 处理出版日期
    QString dateStr = ui->lineEditPublishDate->text().trimmed();
    if (!dateStr.isEmpty()) {
        QDate publishDate = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (!publishDate.isValid()) {
            QMessageBox::warning(this, "错误", "日期格式不正确，请使用yyyy-MM-dd格式");
            return;
        }
        book.setPublishDate(publishDate);
    } else {
        book.setPublishDate(QDate()); // 设置为无效日期
    }

    // 处理库存数量
    bool ok;
    int quantity = ui->lineEditQuantity->text().toInt(&ok);
    if (!ok || quantity < 0) {
        QMessageBox::warning(this, "错误", "库存数量必须是非负整数");
        return;
    }
    book.setQuantity(quantity);

    // 更新图书信息
    if (!BookDao::addBook(book)) {
        QMessageBox::critical(this, "错误", "图书入库失败");
        return;
    }
    QMessageBox::information(this, "提示", "图书入库成功");
}


/**
 * 获取当前选择的用户
 * @return
 */
User Widget::getSelectedUser() {
    // 获取当前选中的行
    int currentRow = ui->tableWidgetUserMgmt->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择一个用户");
        return User(); // 返回一个无效的 Book 对象
    }

    // 从表格中获取 用户名
    QString username = ui->tableWidgetUserMgmt->item(currentRow, 0)->text();
    // 获取图书详细信息
    User fetchedUser = UserDao::getUserByUsername(username);
    if (fetchedUser.getId() == -1) {
        QMessageBox::warning(this, "错误", "获取用户信息失败");
        return User(); // 返回一个无效的 User 对象
    }

    return fetchedUser; // 返回有效的 User 对象
}

/**
 * 刷新用户视图的用户列表
 */
void Widget::refreshUserList() {
    QList<User> userList = UserDao::getAllUsers();

    // 清空表格
    ui->tableWidgetUserMgmt->clearContents();
    ui->tableWidgetUserMgmt->setRowCount(0);

    // 设置表格列数和标题
    ui->tableWidgetUserMgmt->setColumnCount(4);
    QStringList headers;
    headers << "用户名" << "姓名" << "密码" << "用户类型";
    ui->tableWidgetUserMgmt->setHorizontalHeaderLabels(headers);

    // 填充表格数据
    for (const User &user: userList) {
        int row = ui->tableWidgetUserMgmt->rowCount();
        ui->tableWidgetUserMgmt->insertRow(row);

        ui->tableWidgetUserMgmt->setItem(row, 0,
                                       new QTableWidgetItem(user.getUsername()));
        ui->tableWidgetUserMgmt->setItem(row, 1,
                                       new QTableWidgetItem(user.getName()));
        ui->tableWidgetUserMgmt->setItem(row, 2,
                                       new QTableWidgetItem(user.getPassword()));
        ui->tableWidgetUserMgmt->setItem(row, 3,
                                         new QTableWidgetItem(user.getUserTypeString()));
    }

    // 自动调整列宽
    ui->tableWidgetUserMgmt->resizeColumnsToContents();
    // 遍历所有单元格
    for (int row = 0; row < ui->tableWidgetUserMgmt->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidgetUserMgmt->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidgetUserMgmt->item(row, col);
            if (!item) {
                item = new QTableWidgetItem();
                ui->tableWidgetUserMgmt->setItem(row, col, item);
            }
            item->setTextAlignment(Qt::AlignCenter);  // 水平和垂直居中
        }
    }
    // 显示检索结果数量
    qDebug() << "Found" << userList.size() << "users.";
}

/**
 * 删除用户
 */
void Widget::onBtnDeleteUserClicked() {
    User fetchedUser = getSelectedUser();
    if (fetchedUser.getId() == -1) {
        return; // 如果获取失败，直接返回
    }

    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除用户%1吗？").arg(
                                          fetchedUser.getUsername()),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return; // 用户选择了“否”，则不执行删除
    }

    // 执行删除操作
    if (UserDao::deleteUser(fetchedUser.getId())) {
        QMessageBox::information(this, "成功", "用户删除成功。");
    } else {
        QMessageBox::critical(this, "错误", "用户删除失败，请重试。");
    }

    // 刷新显示
    this->refreshUserList(); // 可以根据需要刷新书籍列表
}

void Widget::onBtnAddUserClicked() {
    // 创建并显示详情窗口
    auto *adduserDialog = new adduser();
    adduserDialog->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    // 连接 accepted 信号
    connect(adduserDialog, &adduser::userAdded, this,
            &Widget::refreshUserList);
    adduserDialog->show();
}







