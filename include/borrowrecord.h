#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <QDateTime>

class BorrowRecord {
public:
    enum Status {
        BORROWED,    // 借出
        RETURNED,     // 已还
        OVERDUE       // 逾期
    };

    BorrowRecord();
    BorrowRecord(int id, int bookId, int userId,
                 const QDateTime &borrowDate, const QDateTime &dueDate,
                 const QDateTime &returnDate = QDateTime(),
                 Status status = BORROWED);

    // Getters
    int getId() const;
    int getBookId() const;
    int getUserId() const;
    QDateTime getBorrowDate() const;
    QDateTime getDueDate() const;
    QDateTime getReturnDate() const;
    Status getStatus() const;
    QString getStatusString() const;

    // Setters
    void setId(int id);
    void setBookId(int bookId);
    void setUserId(int userId);
    void setBorrowDate(const QDateTime &borrowDate);
    void setDueDate(const QDateTime &dueDate);
    void setReturnDate(const QDateTime &returnDate);
    void setStatus(Status status);
    void setStatus(const QString &status);

private:
    int id;
    int bookId;
    int userId;
    QDateTime borrowDate;
    QDateTime dueDate;
    QDateTime returnDate;
    Status status;
};

#endif // BORROWRECORD_H