#ifndef DTOS_H
#define DTOS_H

#include <QString>

enum USER_TYPE {STUDENT, TEACHER, ADMIN, DOES_NOT_EXIST};

class User
{
public:
    //Constructors
    User() {}
    User(QString firstName, QString lastName,
         QString email, QString password = "", bool gender = true,
         int age = 20, QString course ="")
        : firstName(firstName), lastName(lastName),
          email(email), gender(gender),
          age(age), course(course), password(password) { }

    QString firstName;
    QString lastName;
    QString email;
    bool gender;
    int age;
    QString course;
    QString password;
};

class Student : public User
{
public:
    Student(){};
    Student(QString firstName, QString lastName,
            QString email, QString password = "0", bool gender = true,
            int age = 20, QString course ="no")
           : User(firstName, lastName, email, password, gender, age, course){ }
};

struct Teacher : public User
{
public:
    Teacher(){};
    Teacher(QString firstName, QString lastName,
            QString email, QString password = "0", bool gender = true,
            int age = 20, QString course ="no", QString background = "no", QString address = "no")
        : User(firstName, lastName, email, password, gender, age, course), background(background), address(address) {}

    QString background;
    QString address;
};

#endif // DTOS_H
