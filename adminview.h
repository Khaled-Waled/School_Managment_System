#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QWidget>
#include "DTOs.h"
#include <vector>

namespace Ui {
class AdminView;
}

class AdminView : public QWidget
{
    Q_OBJECT

public:
    explicit AdminView(QWidget *parent = nullptr);
    ~AdminView();
    void placeUsersInTable(std::vector<Student>);
    void placeUsersInTable(std::vector<Teacher>);

private:
    Ui::AdminView *ui;
    void placeUserInTable(Student);
    void placeUserInTable(Teacher);

signals:
    void requestLogout();
    void requestAllStudents();
    void requestAllTeachers();

private slots:
    void on_pushButton_clicked();
    void on_button_logout_clicked();
    void on_button_refresh_students_clicked();
    void on_button_refresh_teachers_clicked();
};

#endif // ADMINVIEW_H
