#include "widget.h"
#include <QApplication>
#include "userdao.h"
#include "bookdao.h"
#include <QDate>

void testBookDao() {
    // 测试添加图书
    Book newBook;
    newBook.setTitle("测试图书");
    newBook.setAuthor("测试作者");
    newBook.setIsbn("9781234567890");
    newBook.setPublisher("测试出版社");
    newBook.setDescription("这是一本测试图书的描述信息");
    newBook.setLanguage("中文");
    newBook.setPublishDate(QDate(2023, 1, 1));
    newBook.setQuantity(10);

    qDebug() << "Adding test book...";
    bool addResult = BookDao::addBook(newBook);
    qDebug() << "Add book result:" << (addResult ? "Success" : "Failed");

    // 测试通过ISBN查询图书
    Book fetchedBook = BookDao::getBookByIsbn("9781234567890");
    qDebug() << "Fetched book:" << fetchedBook.getTitle()
             << "Author:" << fetchedBook.getAuthor()
             << "Quantity:" << fetchedBook.getQuantity();

    // 测试更新图书信息
    fetchedBook.setTitle("修改后的测试图书");
    fetchedBook.setQuantity(20);
    fetchedBook.setDescription("修改后的描述信息");
    bool updateResult = BookDao::updateBook(fetchedBook);
    qDebug() << "Update book result:" << (updateResult ? "Success" : "Failed");

    Book updatedBook = BookDao::getBookById(fetchedBook.getId());
    qDebug() << "Updated book:" << updatedBook.getTitle()
             << "Quantity:" << updatedBook.getQuantity()
             << "Description:" << updatedBook.getDescription();

    // 测试ISBN是否存在
    bool isbnExists = BookDao::isbnExists("9781234567890");
    qDebug() << "ISBN exists:" << (isbnExists ? "Yes" : "No");

    // 测试更新库存数量
    bool updateQtyResult = BookDao::updateQuantity(fetchedBook.getId(), -5);
    qDebug() << "Update quantity result:" << (updateQtyResult ? "Success" : "Failed");
    Book qtyUpdatedBook = BookDao::getBookById(fetchedBook.getId());
    qDebug() << "Updated quantity:" << qtyUpdatedBook.getQuantity();

    // 测试按书名搜索
    QList<Book> titleSearchResults = BookDao::searchBooksByTitle("测试");
    qDebug() << "Title search results count:" << titleSearchResults.size();
    for (const Book &book : titleSearchResults) {
        qDebug() << "Found book by title:" << book.getTitle();
    }

    // 测试按作者搜索
    QList<Book> authorSearchResults = BookDao::searchBooksByAuthor("测试");
    qDebug() << "Author search results count:" << authorSearchResults.size();
    for (const Book &book : authorSearchResults) {
        qDebug() << "Found book by author:" << book.getTitle();
    }

    // 测试获取所有图书
    QList<Book> allBooks = BookDao::getAllBooks();
    qDebug() << "All books count:" << allBooks.size();
    for (const Book &book : allBooks) {
        qDebug() << "Book:" << book.getId() << book.getTitle()
                 << "by" << book.getAuthor();
    }

    // 测试删除图书
    bool deleteResult = BookDao::deleteBook(fetchedBook.getId());
    qDebug() << "Delete book result:" << (deleteResult ? "Success" : "Failed");

    // 验证图书是否已删除
    Book deletedBook = BookDao::getBookById(fetchedBook.getId());
    qDebug() << "Book after deletion exists:"
             << (deletedBook.getId() != -1 ? "Yes" : "No");
}



void testUserDao() {
    // 测试添加用户
    User newUser;
    newUser.setUsername("testuser");
    newUser.setPassword("testpass");
    newUser.setName("测试用户");
    newUser.setUserType(User::READER);
    QString a = "中文示例";

    qDebug() << a;
    qDebug() << "Adding test user...";
    bool addResult = UserDao::addUser(newUser);
    qDebug() << "Add user result:" << (addResult ? "Success" : "Failed");

    // 测试查询用户
    User fetchedUser = UserDao::getUserByUsername("testuser");
    qDebug() << "Fetched user:" << fetchedUser.getName()
             << "Type:" << fetchedUser.getUserTypeString();

    // 测试更新用户
    fetchedUser.setName("修改后的测试用户");
    fetchedUser.setUserType(User::ADMIN);
    bool updateResult = UserDao::updateUser(fetchedUser);
    qDebug() << "Update user result:" << (updateResult ? "Success" : "Failed");

    User updatedUser = UserDao::getUserById(fetchedUser.getId());
    qDebug() << "Updated user:" << updatedUser.getName()
             << "Type:" << updatedUser.getUserTypeString();

    // 测试验证用户
    bool valid = UserDao::validateUser("testuser", "testpass");
    qDebug() << "Validate user:" << (valid ? "Valid" : "Invalid");

    // 测试获取所有用户
    QList<User> allUsers = UserDao::getAllUsers();
    qDebug() << "All users count:" << allUsers.size();
    for (const User &user: allUsers) {
        qDebug() << "User:" << user.getId() << user.getUsername()
                 << user.getName();
    }

    // 测试删除用户
    bool deleteResult = UserDao::deleteUser(fetchedUser.getId());
    qDebug() << "Delete user result:" << (deleteResult ? "Success" : "Failed");

    // 验证用户是否已删除
    User deletedUser = UserDao::getUserById(fetchedUser.getId());
    qDebug() << "User after deletion exists:"
             << (deletedUser.getId() != -1 ? "Yes" : "No");
}




int main(int argc, char *argv[])
{
//    qDebug() << "Starting UserDao tests...";
//    testUserDao();
//    qDebug() << "UserDao tests completed.";

//    qDebug() << "Starting BookDao tests...";
//    testBookDao();
//    qDebug() << "BookDao tests completed.";


    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
