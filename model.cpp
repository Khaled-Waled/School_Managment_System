#include "model.h"

Model* Model::instancePtr = nullptr;
Model::Model(const QString& dbPath)
{
    //Connect to database
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        database = QSqlDatabase::addDatabase(DRIVER, "MyConnect");
        database.setDatabaseName(dbPath);
        database.setHostName("localhost");
        database.setUserName("root");
        database.setPassword("");
        if (!database.open())
        {
            printf("%s\n",database.lastError().text().toStdString().c_str());
            qWarning() << "ERROR: " << database.lastError().text();
        }
    }

}
void Model::initialize()
{
    createTables();
    addDummyUsers();
}
void Model::addDummyUsers()
{
    Student s1 = Student("John", "doe", "John@doe.com", "1234");
    Student s2 = Student("abc", "cba", "abc@a.com", "4567");
    Student s3 = Student("xyz", "zyx", "xyz@x.com", "8910");


    addStudent(s1); addStudent(s2); addStudent(s3);
    printf("Created users\n");
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
bool Model::createTables()
{
    bool status = true;
    //Create students table
    if (!database.tables().contains(QLatin1String("students"))) {
        status &= executeQuery(QSqlQuery("CREATE TABLE students"
                               "(first_name TEXT, "
                               "last_name TEXT, "
                               "email TEXT PRIMARY KEY, "
                               "gender INTEGER, "
                               "age INTEGER, "
                               "course TEXT, "
                               "password TEXT)"));
    }

    //Create teachers table
    if (!database.tables().contains(QLatin1String("teachers"))) {
        status &= executeQuery(QSqlQuery("CREATE TABLE teachers"
                               "(first_name TEXT, "
                               "last_name TEXT, "
                               "email TEXT UNIQUE PRIMARY KEY, "
                               "gender INTEGER, "
                               "age INTEGER, "
                               "academic_Background TEXT, "
                               "address TEXT, "
                               "course TEXT, "
                               "password TEXT)"));
    }
    return status;
}
bool Model::checkIfExists(QString email, QString password, QString table = "students")
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
bool Model::checkIfExists(QString email, QString table = "students")
{
    bool exists = false;
    QSqlQuery query;
    query.prepare("SELECT email FROM (:table) WHERE email = (:email)");
    query.bindValue(":table", table);
    query.bindValue(":email", email);

    if (query.exec())
    {
        if (query.next())
        {
           return true;
        }
    }

    return exists;
}
USER_TYPE Model::getTypeOfUser(QString email, QString password)
{
    //Check if admin
    if(email == "admin" && password == "admin")
    {
        return ADMIN;
    }
    else
    {
        //Check if user exists as a student
        bool is_student = checkIfExists(email, password, "students");
        if(is_student)
        {
            return STUDENT;
        }
        else
        {
            //Check if user exists as a student
            bool is_teacher = checkIfExists(email, password, "teachers");
            if(is_teacher)
            {
                return TEACHER;
            }
        }
    }
    return DOES_NOT_EXIST;
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
    //TODO re-add gender
    bool success = false;

    QSqlQuery query(database);
    /*
    query.prepare("INSERT INTO students (first_name, last_name, email, age, course, password) "
                  "VALUES (':first_name', ':last_name', ':email', ':age', ':course', ':password');");
    query.bindValue(":first_name", student.firstName);
    query.bindValue(":last_name",student.lastName);
    query.bindValue(":email",student.email);
    query.bindValue(":age" ,QString::fromStdString(std::to_string(student.age)));
    query.bindValue(":course", student.course);
    query.bindValue(":password", student.password);
    */

    query.prepare("INSERT INTO students (first_name, last_name, email, age, course, password) "
                  "VALUES (?,?,?,?,?,?);");
    query.addBindValue(student.firstName);
    query.addBindValue(student.lastName);
    query.addBindValue(student.email);
    query.addBindValue(QString::fromStdString(std::to_string(student.age)));
    query.addBindValue(student.course);
    query.addBindValue(student.password);

    try {
        success = executeQuery(query);
//        printf("%s %d\n",student.email.toStdString().c_str(), success);
//        printf("%s %d\n",query.lastQuery().toStdString().c_str(), success);
    } catch (...) {
        qWarning() << "ERROR: " << database.lastError().text();
    }
    return success;
}
bool Model::addTeacher(Teacher teacher)
{
    //TODO re-add gender
    bool success = false;
    QSqlQuery query(database);
    query.prepare("INSERT INTO teachers (first_name, last_name, email, age, academic_Background, address, course, password)"
                  "VALUES (':first_name', ':last_name', :email, :age, ':backg' ,':address',':course', ':password');");
    query.bindValue(":first_name", teacher.firstName);
    query.bindValue(":last_name",teacher.lastName);
    query.bindValue(":email",teacher.email);
    query.bindValue(":age" ,QString::fromStdString(std::to_string(teacher.age)));
    query.bindValue(":backg", teacher.background);
    query.bindValue(":address", teacher.address);
    query.bindValue(":course", teacher.course);
    query.bindValue(":password", teacher.password);

    try {
        success = executeQuery(query);
    } catch (...) {
        qWarning() << "ERROR: " << database.lastError().text();
    }
    return success;

}
Student* Model::getStudentByEmail(QString email)
{
    QSqlQuery query;
    query.prepare("SELECT TOP 1 * FROM students WHERE email = :email;");
    query.bindValue(":email", email);

    Student* student = new Student();
    if (query.exec())
    {
        query.next();

        student->firstName   = query.value(0).toString();
        student->lastName    = query.value(1).toString();
        student->email       = query.value(2).toString();
        student->gender      = true; //TODO assign actual value
        student->age         = std::atoi(query.value(4).toString().toStdString().c_str());
        student->course      = query.value(5).toString();
        student->password    = query.value(6).toString();
    }

    return student;
}
Teacher* Model::getTeacherByEmail(QString email)
{
    QSqlQuery query;
    query.prepare("SELECT TOP 1 * FROM teachers WHERE email = :email;");
    query.bindValue(":email", email);

    Teacher* teacher;
    if (query.exec())
    {
        query.next();

        teacher->firstName   = query.value(0).toString();
        teacher->lastName    = query.value(1).toString();
        teacher->email       = query.value(2).toString();
        teacher->gender      = true; //TODO assign actual value
        teacher->age         = std::atoi(query.value(4).toString().toStdString().c_str());
        teacher->course      = query.value(5).toString();
        teacher->background  = query.value(6).toString();
        teacher->address     = query.value(7).toString();
        teacher->password    = query.value(8).toString();
    }

    return teacher;
}
bool Model::editStudent(Student student)
{
    //TODO implement edit student
    return false;
}
bool Model::editTeacher(Teacher teacher)
{
    //TODO implement edit teacher
    return false;
}

void Model::showAllStudents()
{
    QSqlQuery query(database);
    query.prepare("Select * from students");

    query.exec();
        printf("Start...\n");
        while (query.next())
        {
            printf("%s\n",query.value(0).toString().toStdString().c_str());
        }
    printf("Done\n");
}
