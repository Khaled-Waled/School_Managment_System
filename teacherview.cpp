#include "teacherview.h"
#include "ui_teacherview.h"

TeacherView::TeacherView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherView)
{
    ui->setupUi(this);
    setWindowTitle("Teacher details");

    ui->le_fname->setDisabled(true);
    ui->le_lname->setDisabled(true);
    ui->le_course->setDisabled(true);
    ui->le_age->setDisabled(true);
    ui->le_email->setDisabled(true);

    ui->tableWidget_students->setColumnCount(4);
    ui->tableWidget_students->setHorizontalHeaderLabels({"First name", "Last name", "E-Mail", "Course"});
}

TeacherView::~TeacherView()
{
    delete ui;
}

void TeacherView::on_button_logout_clicked()
{
    emit requestLogout();
}

void TeacherView::fillTeacherData(Teacher* teacher)
{
    ui->le_fname->setText(teacher->firstName);
    ui->le_lname->setText(teacher->lastName);
    ui->le_course->setText(teacher->course);
    ui->le_email->setText(teacher->email);
    ui->le_age->setText(QString::fromStdString(std::to_string(teacher->age)));
}

void TeacherView::placeStudentInTable(Student student)
{
    auto* table = ui->tableWidget_students;
    table->insertRow(table->rowCount());

    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(student.firstName));
    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(student.lastName));
    table->setItem(table->rowCount()-1, 2, new QTableWidgetItem(student.email));
    table->setItem(table->rowCount()-1, 3, new QTableWidgetItem(student.course));
}

void TeacherView::fillStudentsTable(std::vector<Student> students)
{
    for(const auto &student: students)
        placeStudentInTable(student);
}
