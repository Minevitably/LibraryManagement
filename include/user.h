#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User {
public:
    enum UserType {
        ADMIN, READER
    };

    User();

    User(int id, const QString &username, const QString &password,
         const QString &name, UserType userType,
         const QDateTime &createdAt = QDateTime::currentDateTime(),
         const QDateTime &updatedAt = QDateTime::currentDateTime());

    // Getters
    int getId() const;

    QString getUsername() const;

    QString getPassword() const;

    QString getName() const;

    UserType getUserType() const;

    QDateTime getCreatedAt() const;

    QDateTime getUpdatedAt() const;

    QString getUserTypeString() const;

    // Setters
    void setId(int id);

    void setUsername(const QString &username);

    void setPassword(const QString &password);

    void setName(const QString &name);

    void setUserType(UserType userType);

    void setUserType(const QString &userType);

    void setCreatedAt(const QDateTime &createdAt);

    void setUpdatedAt(const QDateTime &updatedAt);

private:
    int id;
    QString username;
    QString password;
    QString name;
    UserType userType;
    QDateTime createdAt;
    QDateTime updatedAt;
};

#endif // USER_H