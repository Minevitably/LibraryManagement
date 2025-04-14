#include "userdao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool UserDao::addUser(const User &user) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, name, user_type) "
                  "VALUES (:username, :password, :name, :user_type)");
    query.bindValue(":username", user.getUsername());
    query.bindValue(":password", user.getPassword());
    query.bindValue(":name", user.getName());
    query.bindValue(":user_type", user.getUserTypeString());

    if (!query.exec()) {
        qCritical() << "Add user failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool UserDao::deleteUser(int userId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qCritical() << "Delete user failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool UserDao::updateUser(const User &user) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
            "UPDATE users SET username = :username, password = :password, "
            "name = :name, user_type = :user_type WHERE id = :id");
    query.bindValue(":username", user.getUsername());
    query.bindValue(":password", user.getPassword());
    query.bindValue(":name", user.getName());
    query.bindValue(":user_type", user.getUserTypeString());
    query.bindValue(":id", user.getId());

    if (!query.exec()) {
        qCritical() << "Update user failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

User UserDao::getUserById(int userId) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return User();
    }

    QSqlQuery query(db);
    query.prepare(
            "SELECT id, username, password, name, user_type, created_at, updated_at "
            "FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (!query.exec() || !query.next()) {
        qWarning() << "Get user by ID failed:" << query.lastError().text();
        return User();
    }

    User user;
    user.setId(query.value("id").toInt());
    user.setUsername(query.value("username").toString());
    user.setPassword(query.value("password").toString());
    user.setName(query.value("name").toString());
    user.setUserType(query.value("user_type").toString());
    user.setCreatedAt(query.value("created_at").toDateTime());
    user.setUpdatedAt(query.value("updated_at").toDateTime());

    return user;
}

User UserDao::getUserByUsername(const QString &username) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return User();
    }

    QSqlQuery query(db);
    query.prepare(
            "SELECT id, username, password, name, user_type, created_at, updated_at "
            "FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        qWarning() << "Get user by username failed:"
                   << query.lastError().text();
        return User();
    }

    User user;
    user.setId(query.value("id").toInt());
    user.setUsername(query.value("username").toString());
    user.setPassword(query.value("password").toString());
    user.setName(query.value("name").toString());
    user.setUserType(query.value("user_type").toString());
    user.setCreatedAt(query.value("created_at").toDateTime());
    user.setUpdatedAt(query.value("updated_at").toDateTime());

    return user;
}

QList<User> UserDao::getAllUsers() {
    QList<User> users;
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return users;
    }

    QSqlQuery query(db);
    query.prepare(
            "SELECT id, username, password, name, user_type, created_at, updated_at FROM users");

    if (!query.exec()) {
        qCritical() << "Get all users failed:" << query.lastError().text();
        return users;
    }

    while (query.next()) {
        User user;
        user.setId(query.value("id").toInt());
        user.setUsername(query.value("username").toString());
        user.setPassword(query.value("password").toString());
        user.setName(query.value("name").toString());
        user.setUserType(query.value("user_type").toString());
        user.setCreatedAt(query.value("created_at").toDateTime());
        user.setUpdatedAt(query.value("updated_at").toDateTime());

        users.append(user);
    }

    return users;
}

bool UserDao::validateUser(const QString &username, const QString &password) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
            "SELECT COUNT(*) FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec() || !query.next()) {
        qCritical() << "Validate user failed:" << query.lastError().text();
        return false;
    }

    return query.value(0).toInt() > 0;
}

bool UserDao::usernameExists(const QString &username) {
    QSqlDatabase db = DBUtil::getConnection();
    if (!db.isValid()) {
        qCritical() << "Failed to get database connection";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        qCritical() << "Check username exists failed:"
                    << query.lastError().text();
        return false;
    }

    return query.value(0).toInt() > 0;
}