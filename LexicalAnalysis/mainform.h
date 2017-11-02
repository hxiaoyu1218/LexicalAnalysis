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
    void MarkTableRefresh(int count);//记号列表载入
private slots:
    void on_CloseBtn_clicked();//关闭程序

    void on_pushButton_clicked();//打开文件button

private:
    Ui::MainForm *ui;
    bool mDragWindow;
    QPoint mMousePoint;
    void MarkTableInit();//分析结果列表初始化
    void CodeLoad(void);//展示源代码
    void AnalysisInfoLoad(void);//分析信息（错误、统计信息）载入
    QString getSstring(std::string &type, int &pos);//从各个表获取内容
    void paintEvent(QPaintEvent *event);//重写窗体阴影
    void mousePressEvent(QMouseEvent * e);//以下三个重写鼠标活动，获得窗体移动
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
};

#endif // MAINFORM_H
