#ifndef MODEL_H
#define MODEL_H

#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include "DTOs.h"



class Model
{
private:
    QSqlDatabase database;
    Model(const QString&);
    static Model* instancePtr;
    bool createTables();
    bool executeQuery(QSqlQuery);
    void addDummyUsers();

public:
    static Model *getInstance(const QString&);
    static Model *getInstance();
    void initialize();

    bool checkIfExists(QString, QString, QString);
    bool checkIfExists(QString, QString);
    USER_TYPE getTypeOfUser(QString, QString);
    bool deletePerson(QString, QString);
    bool addStudent(Student);
    bool addTeacher(Teacher);
    Teacher* getTeacherByEmail(QString);
    Student* getStudentByEmail(QString);
    bool editStudent(Student);
    bool editTeacher(Teacher);
    void showAllStudents();
};

#endif // MODEL_H
