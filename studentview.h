#ifndef STUDENTVIEW_H
#define STUDENTVIEW_H

#include <QDialog>
#include "DTOs.h"


namespace Ui {
class StudentView;
}

class StudentView : public QDialog
{
    Q_OBJECT

public:
    explicit StudentView(QWidget *parent = nullptr);
    ~StudentView();
    void fillStudentData(Student*);

private:
    Ui::StudentView *ui;

signals:
    void requestStudentData();
    void changeCourse(QString);
    void requestLogout();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_button_logout_clicked();
};

#endif // STUDENTVIEW_H
