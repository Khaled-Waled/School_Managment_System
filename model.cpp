#include "model.h"

Model* Model::instancePtr = nullptr;
Model::Model(const QString& dbPath)
{
    //Connect to database
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        database = QSqlDatabase::addDatabase(DRIVER);
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
    bool created = createTables();
    printf("Created tables?  %d\n",created);
    created = addDummyUsers();
    printf("Created users?  %d\n",created);
}
bool Model::addDummyUsers()
{
    Student s1 = Student("John", "doe", "John@doe.com", "1234");
    Student s2 = Student("abc", "cba", "abc@a.com", "4567");
    Student s3 = Student("xyz", "zyx", "xyz@x.com", "8910");

    bool status = true;
    status &= addStudent(s1); status &= addStudent(s2); status &= addStudent(s3);
    return status;
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

    status &= executeQuery(QSqlQuery("DROP TABLE IF EXISTS students;"));
    status &= executeQuery(QSqlQuery("DROP TABLE IF EXISTS teachers;"));

    //Create students table
    status &= executeQuery(QSqlQuery("CREATE TABLE IF NOT EXISTS students"
                           "(first_name TEXT, "
                           "last_name TEXT, "
                           "email TEXT PRIMARY KEY, "
                           "gender INTEGER, "
                           "age INTEGER, "
                           "course TEXT, "
                           "password TEXT)"));

    //Create teachers table
    status &= executeQuery(QSqlQuery("CREATE TABLE IF NOT EXISTS teachers"
                           "(first_name TEXT, "
                           "last_name TEXT, "
                           "email TEXT UNIQUE PRIMARY KEY, "
                           "gender INTEGER, "
                           "age INTEGER, "
                           "academic_Background TEXT, "
                           "address TEXT, "
                           "course TEXT, "
                           "password TEXT)"));
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

    QString str = "INSERT INTO students (first_name, last_name, email, age, course, password) "
                   "VALUES ('%1', '%2', '%3', %4, '%5', '%6');";
    str = str.arg(student.firstName,
                  student.lastName,
                  student.email,
                  QString::fromStdString(std::to_string(student.age)),
                  student.course,
                  student.password);
    query.prepare(str);

    try {
        success = executeQuery(query);
//        printf("%s %s %d\n",student.email.toStdString().c_str(), student.firstName.toStdString().c_str(), success);
//        printf("%s\n",query.lastQuery().toStdString().c_str());
//        printf("%s\n",query.lastError().text().toStdString().c_str());
//        printf("QT:%d  SQLite:%d\n",query.boundValues().count(),-1);
//        printf("\n###################\n");
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

    QString str = "INSERT INTO teahers (first_name, last_name, email, age, academic_Background , address, course, password) "
                   "VALUES ('%1', '%2', '%3', %4, '%5', '%6', '%7', '%8');";
    str = str.arg(teacher.firstName,
                  teacher.lastName,
                  teacher.email,
                  QString::fromStdString(std::to_string(teacher.age)),
                  teacher.background,
                  teacher.address,
                  teacher.course,
                  teacher.password);

    query.prepare(str);


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
