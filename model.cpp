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
Model* Model::getInstance(const QString& dbPath)
{
    if (instancePtr == nullptr) {
        instancePtr = new Model(dbPath);
    }
    return instancePtr;
}
Model* Model::getInstance()
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
bool Model::checkIfExists(QString email, QString password, QString table = "student")
{
    bool exists = false;
    QSqlQuery query;
    query.prepare("SELECT email FROM (:table) WHERE email = (:email) AND password = (:password)");
    query.bindValue(":table", table);
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec())
    {
        if (query.next())
        {
           return true;
        }
    }

    return exists;
}
bool Model::deletePerson(QString email, QString table)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("DELETE FROM :table WHERE email = :email;");
    query.bindValue(":table", table);
    query.bindValue(":email", email);
    if (query.exec())
    {
        if (query.next())
        {
           return true;
        }
    }
    return success;
}
bool Model::addStudent(Student student)
{
    bool success = false;
    QString q = QString("INSERT INTO students (firstName, lastName, Id, email, gender, age, course, password)"
            "VALUES ('%1', '%2', %3, '%4', '%5', %6, '%7', '%8');");
    q =  q.arg(student.firstName)
          .arg(student.lastName)
          .arg(QString::fromStdString(std::to_string(student.Id)))
          .arg(student.email)
          .arg(student.gender ? "true":"false")
          .arg(QString::fromStdString(std::to_string(student.age)))
          .arg(student.course)
          .arg(student.password);

    try {
        success = executeQuery(QSqlQuery(q));
    } catch (...) {
        qWarning() << "ERROR: " << database.lastError().text();
    }
    return success;
}
bool Model::addTeacher(Teacher teacher)
{
    bool success = false;
    QString q = QString("INSERT INTO teachers (firstName, lastName, Id, email, gender, age, academic_Background, address, course, password)"
            "VALUES ('%1', '%2', %3, '%4', '%5', %6, '%7');");
    q =  q.arg(teacher.firstName)
          .arg(teacher.lastName)
          .arg(QString::fromStdString(std::to_string(teacher.Id)))
          .arg(teacher.email)
          .arg(teacher.gender ? "true":"false")
          .arg(QString::fromStdString(std::to_string(teacher.age)))
          .arg(teacher.background)
          .arg(teacher.address)
          .arg(teacher.course)
          .arg(teacher.password);

    try {
        success = executeQuery(QSqlQuery(q));
    } catch (...) {
        qWarning() << "ERROR: " << database.lastError().text();
    }
    return success;

}
Student Model::getStudentByEmail(QString email)
{
    QSqlQuery query;
    query.prepare("SELECT TOP 1 * FROM students WHERE email = :email;");
    query.bindValue(":email", email);

    Student student;
    if (query.exec())
    {
        query.next();

        student.firstName   = query.value(0).toString();
        student.lastName    = query.value(1).toString();
        student.Id          = std::atoi(query.value(2).toString().toStdString().c_str());
        student.email       = query.value(3).toString();
        student.gender      = true; //TODO assign actual value
        student.age         = std::atoi(query.value(5).toString().toStdString().c_str());
        student.course      = query.value(6).toString();
        student.password    = query.value(7).toString();
    }

    return student;
}
Teacher Model::getTeacherByEmail(QString email)
{
    QSqlQuery query;
    query.prepare("SELECT TOP 1 * FROM teachers WHERE email = :email;");
    query.bindValue(":email", email);

    Teacher teacher;
    if (query.exec())
    {
        query.next();

        teacher.firstName   = query.value(0).toString();
        teacher.lastName    = query.value(1).toString();
        teacher.Id          = std::atoi(query.value(2).toString().toStdString().c_str());
        teacher.email       = query.value(3).toString();
        teacher.gender      = true; //TODO assign actual value
        teacher.age         = std::atoi(query.value(5).toString().toStdString().c_str());
        teacher.course      = query.value(6).toString();
        teacher.background  = query.value(6).toString();
        teacher.address     = query.value(6).toString();
        teacher.password    = query.value(7).toString();
    }

    return teacher;
}
