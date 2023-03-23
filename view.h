#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qmessagebox.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getEmailInput();
    QString getPassInput();
    void hideLoginFrame();
    void showUserFrame();
    void showErrorMessage(QString, QString, QString);

private slots:
    void on_button_login_clicked();

private:
    Ui::MainWindow *ui;
    void buildLoginScreen();

signals:
    void LoginAttempt();
};
#endif // MAINWINDOW_H
