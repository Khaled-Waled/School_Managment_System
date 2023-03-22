#include "controller.h"
#include "view.h"
#include "model.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Model *m = Model::getInstance("school2.db");
    m->initialize();
    Controller c (m,&w);

    w.show();
    return a.exec();
}
