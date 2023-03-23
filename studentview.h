#ifndef STUDENTVIEW_H
#define STUDENTVIEW_H

#include <QDialog>

namespace Ui {
class StudentView;
}

class StudentView : public QDialog
{
    Q_OBJECT

public:
    explicit StudentView(QWidget *parent = nullptr);
    ~StudentView();

private:
    Ui::StudentView *ui;
};

#endif // STUDENTVIEW_H
