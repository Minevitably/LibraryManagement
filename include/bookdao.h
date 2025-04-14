#ifndef BOOKDAO_H
#define BOOKDAO_H

#include "book.h"
#include "dbutil.h"
#include <QList>

class BookDao {
public:
    // 添加图书
    static bool addBook(const Book &book);

    // 根据ID删除图书
    static bool deleteBook(int bookId);

    // 更新图书信息
    static bool updateBook(const Book &book);

    // 根据ID获取图书
    static Book getBookById(int bookId);

    // 根据ISBN获取图书
    static Book getBookByIsbn(const QString &isbn);

    // 获取所有图书
    static QList<Book> getAllBooks();

    // 根据书名搜索图书
    static QList<Book> searchBooksByTitle(const QString &title);

    // 根据作者搜索图书
    static QList<Book> searchBooksByAuthor(const QString &author);

    // 检查ISBN是否存在
    static bool isbnExists(const QString &isbn);

    // 更新库存数量
    static bool updateQuantity(int bookId, int changeAmount);
};

#endif // BOOKDAO_H