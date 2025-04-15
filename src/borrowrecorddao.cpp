#include "borrowrecorddao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool BorrowRecordDao::addRecord(const BorrowRecord &record) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO borrow_records (book_id, user_id, borrow_date, due_date, status) "
                  "VALUES (:book_id, :user_id, :borrow_date, :due_date, :status)");
    query.bindValue(":book_id", record.getBookId());
    query.bindValue(":user_id", record.getUserId());
    query.bindValue(":borrow_date", record.getBorrowDate());
    query.bindValue(":due_date", record.getDueDate());
    query.bindValue(":status", record.getStatusString());

    if (!query.exec()) {
        qCritical() << "Add borrow record failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool BorrowRecordDao::deleteRecord(int recordId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM borrow_records WHERE id = :id");
    query.bindValue(":id", recordId);

    if (!query.exec()) {
        qCritical() << "Delete borrow record failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool BorrowRecordDao::updateRecord(const BorrowRecord &record) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE borrow_records SET "
                  "book_id = :book_id, "
                  "user_id = :user_id, "
                  "borrow_date = :borrow_date, "
                  "due_date = :due_date, "
                  "return_date = :return_date, "
                  "status = :status "
                  "WHERE id = :id");

    query.bindValue(":book_id", record.getBookId());
    query.bindValue(":user_id", record.getUserId());
    query.bindValue(":borrow_date", record.getBorrowDate());
    query.bindValue(":due_date", record.getDueDate());
    query.bindValue(":return_date", record.getReturnDate());
    query.bindValue(":status", record.getStatusString());
    query.bindValue(":id", record.getId());

    if (!query.exec()) {
        qCritical() << "Update borrow record failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

BorrowRecord BorrowRecordDao::getRecordById(int recordId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return BorrowRecord();
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, book_id, user_id, borrow_date, due_date, return_date, status "
                  "FROM borrow_records WHERE id = :id");
    query.bindValue(":id", recordId);

    if (!query.exec() || !query.next()) {
        qWarning() << "Get borrow record by ID failed:" << query.lastError().text();
        return BorrowRecord();
    }

    BorrowRecord record;
    record.setId(query.value("id").toInt());
    record.setBookId(query.value("book_id").toInt());
    record.setUserId(query.value("user_id").toInt());
    record.setBorrowDate(query.value("borrow_date").toDateTime());
    record.setDueDate(query.value("due_date").toDateTime());
    record.setReturnDate(query.value("return_date").toDateTime());
    record.setStatus(query.value("status").toString());

    return record;
}

QList<BorrowRecord> BorrowRecordDao::getRecordsByBookId(int bookId) {
    QList<BorrowRecord> records;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return records;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, book_id, user_id, borrow_date, due_date, return_date, status "
                  "FROM borrow_records WHERE book_id = :book_id");
    query.bindValue(":book_id", bookId);

    if (!query.exec()) {
        qCritical() << "Get records by book ID failed:" << query.lastError().text();
        return records;
    }

    while (query.next()) {
        BorrowRecord record;
        record.setId(query.value("id").toInt());
        record.setBookId(query.value("book_id").toInt());
        record.setUserId(query.value("user_id").toInt());
        record.setBorrowDate(query.value("borrow_date").toDateTime());
        record.setDueDate(query.value("due_date").toDateTime());
        record.setReturnDate(query.value("return_date").toDateTime());
        record.setStatus(query.value("status").toString());

        records.append(record);
    }

    return records;
}

QList<BorrowRecord> BorrowRecordDao::getRecordsByUserId(int userId) {
    QList<BorrowRecord> records;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return records;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, book_id, user_id, borrow_date, due_date, return_date, status "
                  "FROM borrow_records WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qCritical() << "Get records by user ID failed:" << query.lastError().text();
        return records;
    }

    while (query.next()) {
        BorrowRecord record;
        record.setId(query.value("id").toInt());
        record.setBookId(query.value("book_id").toInt());
        record.setUserId(query.value("user_id").toInt());
        record.setBorrowDate(query.value("borrow_date").toDateTime());
        record.setDueDate(query.value("due_date").toDateTime());
        record.setReturnDate(query.value("return_date").toDateTime());
        record.setStatus(query.value("status").toString());

        records.append(record);
    }

    return records;
}

QList<BorrowRecord> BorrowRecordDao::getAllRecords() {
    QList<BorrowRecord> records;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return records;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, book_id, user_id, borrow_date, due_date, return_date, status "
                  "FROM borrow_records");

    if (!query.exec()) {
        qCritical() << "Get all borrow records failed:" << query.lastError().text();
        return records;
    }

    while (query.next()) {
        BorrowRecord record;
        record.setId(query.value("id").toInt());
        record.setBookId(query.value("book_id").toInt());
        record.setUserId(query.value("user_id").toInt());
        record.setBorrowDate(query.value("borrow_date").toDateTime());
        record.setDueDate(query.value("due_date").toDateTime());
        record.setReturnDate(query.value("return_date").toDateTime());
        record.setStatus(query.value("status").toString());

        records.append(record);
    }

    return records;
}

QList<BorrowRecord> BorrowRecordDao::getOverdueRecords() {
    QList<BorrowRecord> records;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return records;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, book_id, user_id, borrow_date, due_date, return_date, status "
                  "FROM borrow_records WHERE status = '逾期' OR "
                  "(status = '借出' AND due_date < CURRENT_TIMESTAMP)");

    if (!query.exec()) {
        qCritical() << "Get overdue records failed:" << query.lastError().text();
        return records;
    }

    while (query.next()) {
        BorrowRecord record;
        record.setId(query.value("id").toInt());
        record.setBookId(query.value("book_id").toInt());
        record.setUserId(query.value("user_id").toInt());
        record.setBorrowDate(query.value("borrow_date").toDateTime());
        record.setDueDate(query.value("due_date").toDateTime());
        record.setReturnDate(query.value("return_date").toDateTime());
        record.setStatus(query.value("status").toString());

        records.append(record);
    }

    return records;
}

bool BorrowRecordDao::isBookAvailable(int bookId, int userId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM borrow_records "
                  "WHERE book_id = :book_id AND "
                  "user_id = :user_id AND status = '借出'");
    query.bindValue(":book_id", bookId);
    query.bindValue(":user_id", userId);

    if (!query.exec() || !query.next()) {
        qCritical() << "Check book availability failed:" << query.lastError().text();
        return false;
    }

    return query.value(0).toInt() == 0;
}

int BorrowRecordDao::getUserBorrowCount(int userId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return 0;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM borrow_records "
                  "WHERE user_id = :user_id AND status = '借出'");
    query.bindValue(":user_id", userId);

    if (!query.exec() || !query.next()) {
        qCritical() << "Get user borrow count failed:" << query.lastError().text();
        return 0;
    }

    return query.value(0).toInt();
}

bool BorrowRecordDao::returnBook(int recordId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE borrow_records SET "
                  "return_date = CURRENT_TIMESTAMP, "
                  "status = '已还' "
                  "WHERE id = :id");
    query.bindValue(":id", recordId);

    if (!query.exec()) {
        qCritical() << "Return book failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool BorrowRecordDao::renewBook(int recordId, const QDateTime &newDueDate) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE borrow_records SET "
                  "due_date = :new_due_date "
                  "WHERE id = :id AND status = '借出'");
    query.bindValue(":new_due_date", newDueDate);
    query.bindValue(":id", recordId);

    if (!query.exec()) {
        qCritical() << "Renew book failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}