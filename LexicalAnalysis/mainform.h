#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
    void MarkTableRefresh(int count);
private slots:
    void on_CloseBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainForm *ui;
    bool mDragWindow;
    QPoint mMousePoint;
    void MarkTableInit();
    void CodeLoad(void);
    void ErrorLoad(void);
    QString getSstring(std::string &type, int &pos);
    void paintEvent(QPaintEvent *event);//重写窗体阴影
    void mousePressEvent(QMouseEvent * e);//以下三个重写鼠标活动，获得窗体移动
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
};

#endif // MAINFORM_H
