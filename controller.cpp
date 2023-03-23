#include "controller.h"

Controller::Controller(Model* m, MainWindow* v)
{
    this->model = m;
    this->view = v;

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
            studentView = new StudentView;
            studentView->show();
            break;
        }
        case STUDENT:
        {
            user = model->getStudentByEmail(username);
            studentView = new StudentView;
            studentView->show();
            break;
        }
        case TEACHER:
        {
            user = model->getTeacherByEmail(username);
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
