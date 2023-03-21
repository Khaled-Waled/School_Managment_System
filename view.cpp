#include "view.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_login_clicked()
{
    //Check if admin
    //Check if user exists
    //Set user type to the correct one
    //Hide this frame
    //Show user screen frame
}

