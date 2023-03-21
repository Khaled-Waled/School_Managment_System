#ifndef MODEL_H
#define MODEL_H

#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QSqlQuery>



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
};

#endif // MODEL_H
