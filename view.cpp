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
    emit LoginAttempt();
}

QString MainWindow::getEmailInput()
{
    return ui->le_email->text();
}
QString MainWindow::getPassInput()
{
    return ui->le_password->text();
}

void MainWindow::hideLoginFrame()
{
    ui->frame_login->setDisabled(true);
    ui->frame_login->hide();
}

void MainWindow::showErrorMessage(QString title, QString message, QString windowTitle = "Warning")
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(windowTitle);
    msgBox.setText(title);
    msgBox.setInformativeText(message);
    msgBox.exec();
}

