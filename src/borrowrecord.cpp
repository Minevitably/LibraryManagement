#include "borrowrecord.h"

BorrowRecord::BorrowRecord()
        : id(-1), bookId(-1), userId(-1), status(BORROWED) {}

BorrowRecord::BorrowRecord(int id, int bookId, int userId,
                           const QDateTime &borrowDate, const QDateTime &dueDate,
                           const QDateTime &returnDate, Status status)
        : id(id), bookId(bookId), userId(userId),
          borrowDate(borrowDate), dueDate(dueDate),
          returnDate(returnDate), status(status) {}

// Getters
int BorrowRecord::getId() const { return id; }
int BorrowRecord::getBookId() const { return bookId; }
int BorrowRecord::getUserId() const { return userId; }
QDateTime BorrowRecord::getBorrowDate() const { return borrowDate; }
QDateTime BorrowRecord::getDueDate() const { return dueDate; }
QDateTime BorrowRecord::getReturnDate() const { return returnDate; }
BorrowRecord::Status BorrowRecord::getStatus() const { return status; }

QString BorrowRecord::getStatusString() const {
    switch(status) {
        case BORROWED: return "借出";
        case RETURNED: return "已还";
        case OVERDUE: return "逾期";
        default: return "未知";
    }
}

// Setters
void BorrowRecord::setId(int id) { this->id = id; }
void BorrowRecord::setBookId(int bookId) { this->bookId = bookId; }
void BorrowRecord::setUserId(int userId) { this->userId = userId; }
void BorrowRecord::setBorrowDate(const QDateTime &borrowDate) { this->borrowDate = borrowDate; }
void BorrowRecord::setDueDate(const QDateTime &dueDate) { this->dueDate = dueDate; }
void BorrowRecord::setReturnDate(const QDateTime &returnDate) { this->returnDate = returnDate; }

void BorrowRecord::setStatus(Status status) {
    this->status = status;
}

void BorrowRecord::setStatus(const QString &status) {
    if (status == "借出") {
        this->status = BORROWED;
    } else if (status == "已还") {
        this->status = RETURNED;
    } else if (status == "逾期") {
        this->status = OVERDUE;
    }
}