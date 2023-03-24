#include "teacherview.h"
#include "ui_teacherview.h"

TeacherView::TeacherView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherView)
{
    ui->setupUi(this);
}

TeacherView::~TeacherView()
{
    delete ui;
}

void TeacherView::on_button_logout_clicked()
{
    emit requestLogout();
}

