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
    void createTables();
    bool executeQuery(QSqlQuery);

public:
    static Model *getInstance(const QString&);
    static Model *getInstance();

    bool checkIfExists(QString, QString, QString);
    bool deletePerson(QString, QString);
    bool addStudent(Student);
    bool addTeacher(Teacher);
    Teacher getTeacherByEmail(QString);
    Student getStudentByEmail(QString);
};

#endif // MODEL_H
