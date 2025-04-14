#include "user.h"

User::User() : id(-1), userType(READER) {}

User::User(int id, const QString &username, const QString &password,
           const QString &name, UserType userType,
           const QDateTime &createdAt, const QDateTime &updatedAt)
        : id(id), username(username), password(password), name(name),
          userType(userType), createdAt(createdAt), updatedAt(updatedAt) {}

// Getters
int User::getId() const { return id; }

QString User::getUsername() const { return username; }

QString User::getPassword() const { return password; }

QString User::getName() const { return name; }

User::UserType User::getUserType() const { return userType; }

QDateTime User::getCreatedAt() const { return createdAt; }

QDateTime User::getUpdatedAt() const { return updatedAt; }

QString User::getUserTypeString() const {
    return userType == ADMIN ? "admin" : "reader";
}

// Setters
void User::setId(int id) { this->id = id; }

void User::setUsername(const QString &username) { this->username = username; }

void User::setPassword(const QString &password) { this->password = password; }

void User::setName(const QString &name) { this->name = name; }

void User::setUserType(UserType userType) { this->userType = userType; }

void User::setUserType(const QString &userType) {
    this->userType = (userType.toLower() == "admin") ? ADMIN : READER;
}

void
User::setCreatedAt(const QDateTime &createdAt) { this->createdAt = createdAt; }

void
User::setUpdatedAt(const QDateTime &updatedAt) { this->updatedAt = updatedAt; }