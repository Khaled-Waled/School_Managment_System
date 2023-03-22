#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "model.h"
#include "view.h"


class Controller : public QObject
{
    Q_OBJECT
private:
    Model* model;
    MainWindow* view;
    USER_TYPE userType;
    User* user;

public:
    Controller(Model*, MainWindow*);

private slots:
    void handleLogin();
};

#endif // CONTROLLER_H
