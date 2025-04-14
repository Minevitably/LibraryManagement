#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QDate>
#include <QDateTime>

class Book {
public:
    Book();

    Book(int id, const QString &title, const QString &author,
         const QString &isbn, const QString &publisher,
         const QString &description, const QString &language,
         const QDate &publishDate, unsigned int quantity,
         const QDateTime &createdAt = QDateTime::currentDateTime(),
         const QDateTime &updatedAt = QDateTime::currentDateTime());

    // Getters
    int getId() const;
    QString getTitle() const;
    QString getAuthor() const;
    QString getIsbn() const;
    QString getPublisher() const;
    QString getDescription() const;
    QString getLanguage() const;
    QDate getPublishDate() const;
    unsigned int getQuantity() const;
    QDateTime getCreatedAt() const;
    QDateTime getUpdatedAt() const;

    // Setters
    void setId(int id);
    void setTitle(const QString &title);
    void setAuthor(const QString &author);
    void setIsbn(const QString &isbn);
    void setPublisher(const QString &publisher);
    void setDescription(const QString &description);
    void setLanguage(const QString &language);
    void setPublishDate(const QDate &publishDate);
    void setQuantity(unsigned int quantity);
    void setCreatedAt(const QDateTime &createdAt);
    void setUpdatedAt(const QDateTime &updatedAt);

private:
    int id;
    QString title;
    QString author;
    QString isbn;
    QString publisher;
    QString description;
    QString language;
    QDate publishDate;
    unsigned int quantity;
    QDateTime createdAt;
    QDateTime updatedAt;
};

#endif // BOOK_H