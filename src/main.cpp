#include "widget.h"
#include "dbutil.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include "userdao.h"


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




    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
