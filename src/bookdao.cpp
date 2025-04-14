#include "bookdao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool BookDao::addBook(const Book &book) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO books (title, author, isbn, publisher, description, "
                  "language, publish_date, quantity) "
                  "VALUES (:title, :author, :isbn, :publisher, :description, "
                  ":language, :publish_date, :quantity)");

    query.bindValue(":title", book.getTitle());
    query.bindValue(":author", book.getAuthor());
    query.bindValue(":isbn", book.getIsbn());
    query.bindValue(":publisher", book.getPublisher());
    query.bindValue(":description", book.getDescription());
    query.bindValue(":language", book.getLanguage());
    query.bindValue(":publish_date", book.getPublishDate());
    query.bindValue(":quantity", book.getQuantity());

    if (!query.exec()) {
        qCritical() << "Add book failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool BookDao::deleteBook(int bookId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qCritical() << "Delete book failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool BookDao::updateBook(const Book &book) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE books SET title = :title, author = :author, "
                  "isbn = :isbn, publisher = :publisher, description = :description, "
                  "language = :language, publish_date = :publish_date, "
                  "quantity = :quantity WHERE id = :id");

    query.bindValue(":title", book.getTitle());
    query.bindValue(":author", book.getAuthor());
    query.bindValue(":isbn", book.getIsbn());
    query.bindValue(":publisher", book.getPublisher());
    query.bindValue(":description", book.getDescription());
    query.bindValue(":language", book.getLanguage());
    query.bindValue(":publish_date", book.getPublishDate());
    query.bindValue(":quantity", book.getQuantity());
    query.bindValue(":id", book.getId());

    if (!query.exec()) {
        qCritical() << "Update book failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

Book BookDao::getBookById(int bookId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return Book();
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, title, author, isbn, publisher, description, "
                  "language, publish_date, quantity, created_at, updated_at "
                  "FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    if (!query.exec() || !query.next()) {
        qWarning() << "Get book by ID failed:" << query.lastError().text();
        return Book();
    }

    Book book;
    book.setId(query.value("id").toInt());
    book.setTitle(query.value("title").toString());
    book.setAuthor(query.value("author").toString());
    book.setIsbn(query.value("isbn").toString());
    book.setPublisher(query.value("publisher").toString());
    book.setDescription(query.value("description").toString());
    book.setLanguage(query.value("language").toString());
    book.setPublishDate(query.value("publish_date").toDate());
    book.setQuantity(query.value("quantity").toUInt());
    book.setCreatedAt(query.value("created_at").toDateTime());
    book.setUpdatedAt(query.value("updated_at").toDateTime());

    return book;
}

Book BookDao::getBookByIsbn(const QString &isbn) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return Book();
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, title, author, isbn, publisher, description, "
                  "language, publish_date, quantity, created_at, updated_at "
                  "FROM books WHERE isbn = :isbn");
    query.bindValue(":isbn", isbn);

    if (!query.exec() || !query.next()) {
        qWarning() << "Get book by ISBN failed:" << query.lastError().text();
        return Book();
    }

    Book book;
    book.setId(query.value("id").toInt());
    book.setTitle(query.value("title").toString());
    book.setAuthor(query.value("author").toString());
    book.setIsbn(query.value("isbn").toString());
    book.setPublisher(query.value("publisher").toString());
    book.setDescription(query.value("description").toString());
    book.setLanguage(query.value("language").toString());
    book.setPublishDate(query.value("publish_date").toDate());
    book.setQuantity(query.value("quantity").toUInt());
    book.setCreatedAt(query.value("created_at").toDateTime());
    book.setUpdatedAt(query.value("updated_at").toDateTime());

    return book;
}

QList<Book> BookDao::getAllBooks() {
    QList<Book> books;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return books;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, title, author, isbn, publisher, description, "
                  "language, publish_date, quantity, created_at, updated_at FROM books");

    if (!query.exec()) {
        qCritical() << "Get all books failed:" << query.lastError().text();
        return books;
    }

    while (query.next()) {
        Book book;
        book.setId(query.value("id").toInt());
        book.setTitle(query.value("title").toString());
        book.setAuthor(query.value("author").toString());
        book.setIsbn(query.value("isbn").toString());
        book.setPublisher(query.value("publisher").toString());
        book.setDescription(query.value("description").toString());
        book.setLanguage(query.value("language").toString());
        book.setPublishDate(query.value("publish_date").toDate());
        book.setQuantity(query.value("quantity").toUInt());
        book.setCreatedAt(query.value("created_at").toDateTime());
        book.setUpdatedAt(query.value("updated_at").toDateTime());

        books.append(book);
    }

    return books;
}

QList<Book> BookDao::searchBooksByTitle(const QString &title) {
    QList<Book> books;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return books;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, title, author, isbn, publisher, description, "
                  "language, publish_date, quantity, created_at, updated_at "
                  "FROM books WHERE title LIKE :title");
    query.bindValue(":title", "%" + title + "%");

    if (!query.exec()) {
        qCritical() << "Search books by title failed:" << query.lastError().text();
        return books;
    }

    while (query.next()) {
        Book book;
        book.setId(query.value("id").toInt());
        book.setTitle(query.value("title").toString());
        book.setAuthor(query.value("author").toString());
        book.setIsbn(query.value("isbn").toString());
        book.setPublisher(query.value("publisher").toString());
        book.setDescription(query.value("description").toString());
        book.setLanguage(query.value("language").toString());
        book.setPublishDate(query.value("publish_date").toDate());
        book.setQuantity(query.value("quantity").toUInt());
        book.setCreatedAt(query.value("created_at").toDateTime());
        book.setUpdatedAt(query.value("updated_at").toDateTime());

        books.append(book);
    }

    return books;
}

QList<Book> BookDao::searchBooksByAuthor(const QString &author) {
    QList<Book> books;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return books;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, title, author, isbn, publisher, description, "
                  "language, publish_date, quantity, created_at, updated_at "
                  "FROM books WHERE author LIKE :author");
    query.bindValue(":author", "%" + author + "%");

    if (!query.exec()) {
        qCritical() << "Search books by author failed:" << query.lastError().text();
        return books;
    }

    while (query.next()) {
        Book book;
        book.setId(query.value("id").toInt());
        book.setTitle(query.value("title").toString());
        book.setAuthor(query.value("author").toString());
        book.setIsbn(query.value("isbn").toString());
        book.setPublisher(query.value("publisher").toString());
        book.setDescription(query.value("description").toString());
        book.setLanguage(query.value("language").toString());
        book.setPublishDate(query.value("publish_date").toDate());
        book.setQuantity(query.value("quantity").toUInt());
        book.setCreatedAt(query.value("created_at").toDateTime());
        book.setUpdatedAt(query.value("updated_at").toDateTime());

        books.append(book);
    }

    return books;
}

bool BookDao::isbnExists(const QString &isbn) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM books WHERE isbn = :isbn");
    query.bindValue(":isbn", isbn);

    if (!query.exec() || !query.next()) {
        qCritical() << "Check ISBN exists failed:" << query.lastError().text();
        return false;
    }

    return query.value(0).toInt() > 0;
}

bool BookDao::updateQuantity(int bookId, int changeAmount) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE books SET quantity = quantity + :change WHERE id = :id");
    query.bindValue(":change", changeAmount);
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qCritical() << "Update quantity failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}