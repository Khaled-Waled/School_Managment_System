#ifndef TEACHERVIEW_H
#define TEACHERVIEW_H

#include <QWidget>
#include "DTOs.h"

namespace Ui {
class TeacherView;
}

class TeacherView : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherView(QWidget *parent = nullptr);
    ~TeacherView();
    void fillTeacherData(Teacher*);
    void fillStudentsTable(std::vector<Student> students);

private slots:
    void on_button_logout_clicked();

signals:
    void requestLogout();
    void requestTeacherData();

private:
    Ui::TeacherView *ui;
    void placeStudentInTable(Student);
};

#endif // TEACHERVIEW_H
