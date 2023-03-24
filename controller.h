#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "model.h"
#include "view.h"
#include "studentview.h"
#include "adminview.h"


class Controller : public QObject
{
    Q_OBJECT
private:
    Model* model;
    MainWindow* view;
    StudentView* studentView;
    AdminView* adminView;
    USER_TYPE userType;
    User* user;
    void createStudentView();
    void createAdminView();

public:
    Controller(Model*, MainWindow*);

private slots:
    void handleLogin();
    void handleLogout();
    void sendStudentDataToView();
    void handleChangeCourse(QString);

};

#endif // CONTROLLER_H
