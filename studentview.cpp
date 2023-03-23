#include "studentview.h"
#include "ui_studentview.h"

StudentView::StudentView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentView)
{
    printf("Creating student view\n");
    ui->setupUi(this);

    //Disable editing
    ui->le_firstName->setDisabled(true);
    ui->le_lastName->setDisabled(true);
    ui->le_courses->setDisabled(true);
    ui->le_age->setDisabled(true);

    setWindowTitle("Student details");
}

StudentView::~StudentView()
{
    delete ui;
}
