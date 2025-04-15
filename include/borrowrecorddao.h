#ifndef BORROWRECORDDAO_H
#define BORROWRECORDDAO_H

#include "borrowrecord.h"
#include "dbutil.h"
#include <QList>

class BorrowRecordDao {
public:
    // 添加借阅记录
    static bool addRecord(const BorrowRecord &record);

    // 删除借阅记录
    static bool deleteRecord(int recordId);

    // 更新借阅记录
    static bool updateRecord(const BorrowRecord &record);

    // 根据ID获取借阅记录
    static BorrowRecord getRecordById(int recordId);

    // 根据图书ID获取借阅记录
    static QList<BorrowRecord> getRecordsByBookId(int bookId);

    // 根据用户ID获取借阅记录
    static QList<BorrowRecord> getRecordsByUserId(int userId);

    // 获取所有借阅记录
    static QList<BorrowRecord> getAllRecords();

    // 获取逾期未还的记录
    static QList<BorrowRecord> getOverdueRecords();

    // 检查图书是否可借(未被借出)
    static bool isBookAvailable(int bookId, int userId);

    // 获取用户当前借阅数量
    static int getUserBorrowCount(int userId);

    // 归还图书
    static bool returnBook(int recordId);

    // 续借图书
    static bool renewBook(int recordId, const QDateTime &newDueDate);
};

#endif // BORROWRECORDDAO_H