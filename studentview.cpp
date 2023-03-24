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
    ui->le_age->setDisabled(true);

    setWindowTitle("Student details");

    emit requestStudentData();
}

StudentView::~StudentView()
{
    delete ui;
}

void StudentView::fillStudentData(Student* student)
{
    ui->le_firstName->setText(student->firstName);
    ui->le_lastName->setText(student->lastName);
    ui->le_courses->setText(student->course);
    ui->le_age->setText(QString::fromStdString(std::to_string(student->age)));
}

void StudentView::on_pushButton_clicked()
{
    emit requestStudentData();
}


void StudentView::on_pushButton_2_clicked()
{
    QString newCourse = ui->le_courses->text();
    printf("new Course = %s\n", newCourse.toStdString().c_str());
    emit changeCourse(newCourse);
}


void StudentView::on_button_logout_clicked()
{
    emit requestLogout();
}

