#ifndef TEACHERVIEW_H
#define TEACHERVIEW_H

#include <QWidget>

namespace Ui {
class TeacherView;
}

class TeacherView : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherView(QWidget *parent = nullptr);
    ~TeacherView();

private slots:
    void on_button_logout_clicked();

signals:
    void requestLogout();

private:
    Ui::TeacherView *ui;
};

#endif // TEACHERVIEW_H
