#include "controller.h"

Controller::Controller(Model* m, MainWindow* v)
{
    this->model = m;
    this->view = v;

    studentView = nullptr;
    adminView   = nullptr;
    teacherView = nullptr;

    connect(v, &MainWindow::LoginAttempt, this, &Controller::handleLogin);
}

void Controller::handleLogin()
{
    printf("logging in...\n");

    //Get data from view
    QString username = view->getEmailInput();
    QString password = view->getPassInput();

    //Get type of user
    userType = model->getTypeOfUser(username, password);
    printf("name: %s\nPassword: %s\n", username.toStdString().c_str(), password.toStdString().c_str());
    //Get user data
    switch(userType)
    {
        case ADMIN:
        {
            user = new User(username, password, username, password);
            createAdminView();
            break;
        }
        case STUDENT:
        {
            user = model->getStudentByEmail(username);
            createStudentView();
            break;
        }
        case TEACHER:
        {
            user = model->getTeacherByEmail(username);
            createTeacherView();
            break;
        }
        default:
        {
            view->showErrorMessage(QString::fromStdString("User not found:"),
                                   QString::fromStdString("The user with this details was not found, please check the credintials and try again"),
                                   QString::fromStdString("Warning"));
            return;
        }
    }

    //Hide this frame
    view->hide();
}

void Controller::createStudentView()
{
    studentView = new StudentView;
    studentView->show();
    sendStudentDataToView();
    QObject::connect(studentView, &StudentView::requestStudentData, this, &Controller::sendStudentDataToView);
    QObject::connect(studentView, &StudentView::changeCourse, this, &Controller::handleChangeCourse);
    QObject::connect(studentView, &StudentView::requestLogout, this, &Controller::handleLogout);
}

void Controller::createAdminView()
{
    adminView = new AdminView();
    adminView->show();
    QObject::connect(adminView, &AdminView::requestLogout, this, &Controller::handleLogout);
    QObject::connect(adminView, &AdminView::requestDeleteStudent, [this](QString email){
        model->deletePerson(email,"students");
        adminView->placeUsersInTable(model->getAllStudents());
    });
    QObject::connect(adminView, &AdminView::requestDeleteTeacher, [this](QString email){
        model->deletePerson(email,"teachers");
        adminView->placeUsersInTable(model->getAllTeachers());
    });
    QObject::connect(adminView, &AdminView::requestAllStudents, [this](){
        adminView->placeUsersInTable(model->getAllStudents());
    });
    QObject::connect(adminView, &AdminView::requestAllTeachers, [this](){
        adminView->placeUsersInTable(model->getAllTeachers());
    });
}

void Controller::createTeacherView()
{
    teacherView = new TeacherView();
    teacherView->show();
    sendTeacherDataToView();
    teacherView->fillStudentsTable(model->getStudentsWithCourse(user->course));
    QObject::connect(teacherView, &TeacherView::requestLogout, this, &Controller::handleLogout);
    QObject::connect(teacherView, &TeacherView::requestTeacherData, this, &Controller::sendTeacherDataToView);
}
void Controller::sendStudentDataToView()
{
    Student* student = (Student*)user;
    studentView->fillStudentData(student);
}
void Controller::sendTeacherDataToView()
{
    Teacher* teacher = (Teacher*)user;
    teacherView->fillTeacherData(teacher);
}
void Controller::handleChangeCourse(QString newCourse)
{
    model->changeStudentCourse(user->email,newCourse);
    user = model->getStudentByEmail(user->email);
}

void Controller::handleLogout()
{
    view->show();
    if(studentView)
    {
        studentView->close();
        delete(studentView);
    }
    if(adminView)
    {
        adminView->close();
        delete(adminView);
    }
    if(teacherView)
    {
        teacherView->close();
        delete(teacherView);
    }
    if(user)
    {
        delete(user);
    }
    studentView = nullptr;
    adminView   = nullptr;
    teacherView = nullptr;
}
