#include "adminview.h"
#include "ui_adminview.h"

AdminView::AdminView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminView)
{
    ui->setupUi(this);
}

AdminView::~AdminView()
{
    delete ui;
}

void AdminView::on_button_logout_clicked()
{
    emit requestLogout();
}

