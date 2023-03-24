#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QWidget>

namespace Ui {
class AdminView;
}

class AdminView : public QWidget
{
    Q_OBJECT

public:
    explicit AdminView(QWidget *parent = nullptr);
    ~AdminView();

private:
    Ui::AdminView *ui;

signals:
    void requestLogout();

private slots:
    void on_pushButton_clicked();
    void on_button_logout_clicked();
};

#endif // ADMINVIEW_H
