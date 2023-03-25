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
    bool status = true;

    //add dummy students
    Student s1 = Student("John", "doe", "John@doe.com", "1234");
    Student s2 = Student("abc", "cba", "abc@a.com", "4567");
    Student s3 = Student("xyz", "zyx", "xyz@x.com", "8910");
    status &= addStudent(s1); status &= addStudent(s2); status &= addStudent(s3);


    //add dummy teachers
    Teacher t1 = Teacher("Teacher_1", "One", "t1@t.com", "Made");
    Teacher t2 = Teacher("Teacher__2", "Two", "t2@t.com", "By");
    Teacher t3 = Teacher("Teacher_3", "Three", "t3@t.com", "Khaled-Waled");
    status &= addTeacher(t1); status &= addTeacher(t2); status &= addTeacher(t3);

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
    QSqlQuery query(database);
    QString str = "SELECT email FROM %3 WHERE email = '%1' AND password = '%2'";
    str = str.arg(email,password, table);

    query.prepare(str);

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
    QSqlQuery query(database);
    QString str = "SELECT email FROM %3 WHERE email = '%1'";
    str = str.arg(email, table);

    query.prepare(str);

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
            //Check if user exists as a teachers
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
    query.prepare(QString("DELETE FROM %1 WHERE email = '%2';").arg(table, email));
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
    } catch (...) {
        qWarning() << "ERROR: " << database.lastError().text();
    }
    return success;
}
bool Model::addTeacher(Teacher teacher)
{
    bool success = false;

    QSqlQuery query(database);

    QString str = "INSERT INTO teachers (first_name, last_name, email, age, academic_Background , address, course, password) "
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
    QString str = "SELECT * FROM students WHERE email = '%1';";
    str = str.arg(email);
    query.prepare(str);

    Student* student = new Student();

    if (executeQuery(query))
    {
        query.next();

        student->firstName   = query.value(0).toString();
        student->lastName    = query.value(1).toString();
        student->email       = query.value(2).toString();
        student->gender      = true;
        student->age         = std::atoi(query.value(4).toString().toStdString().c_str());
        student->course      = query.value(5).toString();
        student->password    = query.value(6).toString();
    }
    return student;
}
Teacher* Model::getTeacherByEmail(QString email)
{
    QString str = "SELECT * FROM teachers WHERE email = '%1';";
    str = str.arg(email);

    QSqlQuery query;
    query.prepare(str);

    Teacher* teacher = new Teacher();
    if (executeQuery(query))
    {
        query.next();

        teacher->firstName   = query.value(0).toString();
        teacher->lastName    = query.value(1).toString();
        teacher->email       = query.value(2).toString();
        teacher->gender      = true;
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

void Model::changeStudentCourse(QString email, QString newCourse)
{
    QString str = ("UPDATE students SET course = '%2' WHERE email = '%1';");
    str = str.arg(email,newCourse);

    QSqlQuery query;
    query.prepare(str);

    executeQuery(query);
}

std::vector<QString> Model::getAllEmails(QString table)
{
    std::vector<QString> result;
    QSqlQuery query(database);
    query.prepare(QString("Select email from %1").arg(table));
    executeQuery(query);

    while (query.next())
    {
        result.push_back(query.value(0).toString());
    }
    return result;
}


std::vector<Student> Model::getAllStudents()
{

    std::vector<QString> emails = getAllEmails("students");
    std::vector<Student> result{};

    for(const auto &email: emails)
    {
        Student* ptr = getStudentByEmail(email);
        Student student = Student(*ptr);
        delete(ptr);
        result.push_back(student);
    }

    return result;
}

std::vector<Teacher> Model::getAllTeachers()
{
    std::vector<QString> emails = getAllEmails("teachers");
    std::vector<Teacher> result{};

    for(const auto &email: emails)
    {
        Teacher* ptr = getTeacherByEmail(email);
        Teacher teacher = Teacher(*ptr);
        delete(ptr);
        result.push_back(teacher);
    }

    return result;
}

std::vector<Student> Model::getStudentsWithCourse(QString course)
{
    std::vector<Student> result;
    QSqlQuery query(database);
    query.prepare(QString("Select * from %1 where course = '%2'").arg("students", course));
    executeQuery(query);

    while (query.next())
    {
        Student* ptr = new Student();

        ptr->firstName   = query.value(0).toString();
        ptr->lastName    = query.value(1).toString();
        ptr->email       = query.value(2).toString();
        ptr->gender      = true;
        ptr->age         = std::atoi(query.value(4).toString().toStdString().c_str());
        ptr->course      = query.value(5).toString();
        ptr->password    = query.value(6).toString();

        Student student = Student(*ptr);
        delete(ptr);
        result.push_back(student);
    }
    return result;
}
