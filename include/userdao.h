#ifndef USERDAO_H
#define USERDAO_H

#include "user.h"
#include "dbutil.h"
#include <QList>

class UserDao {
public:
    // 添加用户
    static bool addUser(const User &user);

    // 根据ID删除用户
    static bool deleteUser(int userId);

    // 更新用户信息
    static bool updateUser(const User &user);

    // 根据ID获取用户
    static User getUserById(int userId);

    // 根据用户名获取用户
    static User getUserByUsername(const QString &username);

    // 获取所有用户
    static QList<User> getAllUsers();

    // 验证用户登录
    static bool validateUser(const QString &username, const QString &password);

    // 检查用户名是否存在
    static bool usernameExists(const QString &username);
};

#endif // USERDAO_H