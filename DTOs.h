#ifndef DTOS_H
#define DTOS_H

#include <QString>

struct Student
{
    QString firstName;
    QString lastName;
    int Id;
    QString email;
    bool gender;
    int age;
    QString course;
    QString password;
};

struct Teacher
{
    QString firstName;
    QString lastName;
    int Id;
    QString email;
    bool gender;
    int age;
    QString background;
    QString address;
    QString course;
    QString password;
};

#endif // DTOS_H
