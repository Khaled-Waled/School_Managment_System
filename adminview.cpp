#include "adminview.h"
#include "ui_adminview.h"

AdminView::AdminView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminView)
{
    ui->setupUi(this);

    //Setup table headers
    ui->tableWidget_students->setColumnCount(4);
    ui->tableWidget_students->setHorizontalHeaderLabels({"First name", "Last name", "E-Mail", "Course"});
    ui->tableWidget_teachers->setColumnCount(4);
    ui->tableWidget_teachers->setHorizontalHeaderLabels({"First name", "Last name", "E-Mail", "Course"});
}

AdminView::~AdminView()
{
    delete ui;
}

void AdminView::on_button_logout_clicked()
{
    emit requestLogout();
}

void AdminView::on_button_refresh_students_clicked()
{
    emit requestAllStudents();
}

void AdminView::on_button_refresh_teachers_clicked()
{
    emit requestAllTeachers();
}

void AdminView::placeUsersInTable(std::vector<Student> students)
{
    for(const auto &student: students)
        placeUserInTable(student);
}

void AdminView::placeUsersInTable(std::vector<Teacher> teachers)
{
    for(const auto &teacher: teachers)
        placeUserInTable(teacher);
}

void AdminView::placeUserInTable(Student student)
{
    auto* table = ui->tableWidget_students;
    table->insertRow(table->rowCount());

    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(student.firstName));
    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(student.lastName));
    table->setItem(table->rowCount()-1, 2, new QTableWidgetItem(student.email));
    table->setItem(table->rowCount()-1, 3, new QTableWidgetItem(student.course));
}

void AdminView::placeUserInTable(Teacher teacher)
{
    auto* table = ui->tableWidget_teachers;
    table->insertRow(table->rowCount());

    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(teacher.firstName));
    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(teacher.lastName));
    table->setItem(table->rowCount()-1, 2, new QTableWidgetItem(teacher.email));
    table->setItem(table->rowCount()-1, 3, new QTableWidgetItem(teacher.course));
}

