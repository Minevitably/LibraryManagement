#include "book.h"

Book::Book() : id(-1), quantity(0) {}

Book::Book(int id, const QString &title, const QString &author,
           const QString &isbn, const QString &publisher,
           const QString &description, const QString &language,
           const QDate &publishDate, unsigned int quantity,
           const QDateTime &createdAt, const QDateTime &updatedAt)
        : id(id), title(title), author(author), isbn(isbn),
          publisher(publisher), description(description),
          language(language), publishDate(publishDate),
          quantity(quantity), createdAt(createdAt),
          updatedAt(updatedAt) {}

// Getters
int Book::getId() const { return id; }

QString Book::getTitle() const { return title; }

QString Book::getAuthor() const { return author; }

QString Book::getIsbn() const { return isbn; }

QString Book::getPublisher() const { return publisher; }

QString Book::getDescription() const { return description; }

QString Book::getLanguage() const { return language; }

QDate Book::getPublishDate() const { return publishDate; }

unsigned int Book::getQuantity() const { return quantity; }

QDateTime Book::getCreatedAt() const { return createdAt; }

QDateTime Book::getUpdatedAt() const { return updatedAt; }

// Setters
void Book::setId(int id) { this->id = id; }

void Book::setTitle(const QString &title) { this->title = title; }

void Book::setAuthor(const QString &author) { this->author = author; }

void Book::setIsbn(const QString &isbn) { this->isbn = isbn; }

void
Book::setPublisher(const QString &publisher) { this->publisher = publisher; }

void Book::setDescription(
        const QString &description) { this->description = description; }

void Book::setLanguage(const QString &language) { this->language = language; }

void Book::setPublishDate(
        const QDate &publishDate) { this->publishDate = publishDate; }

void Book::setQuantity(unsigned int quantity) { this->quantity = quantity; }

void
Book::setCreatedAt(const QDateTime &createdAt) { this->createdAt = createdAt; }

void
Book::setUpdatedAt(const QDateTime &updatedAt) { this->updatedAt = updatedAt; }