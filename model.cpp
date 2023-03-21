#include "model.h"

Model* Model::instancePtr = nullptr;
Model::Model(const QString& dbPath)
{
    //Connect to database
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        database = QSqlDatabase::addDatabase(DRIVER);
        database.setDatabaseName(dbPath);
        if (!database.open())
        {
            qWarning() << "ERROR: " << database.lastError().text();
        }
        else
        {
            createTables();
        }
    }

}
Model *Model::getInstance(const QString& dbPath)
{
    if (instancePtr == nullptr) {
        instancePtr = new Model(dbPath);
    }
    return instancePtr;
}
Model *Model::getInstance()
{
    return instancePtr;
}
bool Model::executeQuery(QSqlQuery query) {
    bool success = true;
    if (!query.exec()) {
        success = false;
    }

    return success;
}
void Model::createTables()
{
    //Create students table
    if (!database.tables().contains(QLatin1String("students"))) {
        executeQuery(QSqlQuery("CREATE TABLE students"
                               "(first_name TEXT, "
                               "last_name TEXT, "
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "email TEXT UNIQUE, "
                               "gender INTEGER, "
                               "age INTEGER, "
                               "course TEXT, "
                               "password TEXT)"));
    }

    //Create teachers table
    if (!database.tables().contains(QLatin1String("teachers"))) {
        executeQuery(QSqlQuery("CREATE TABLE students"
                               "(first_name TEXT, "
                               "last_name TEXT, "
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "email TEXT UNIQUE, "
                               "gender INTEGER, "
                               "age INTEGER, "
                               "academic_Background TEXT, "
                               "address TEXT, "
                               "course TEXT, "
                               "password TEXT)"));
    }
}
