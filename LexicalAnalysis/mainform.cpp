#include "mainform.h"
#include "ui_mainform.h"
#include "analysis.h"
#include <qpainter.h>
#include <qmath.h>
#include <qscrollbar.h>
#include <QString>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    MarkTableInit();
}

MainForm::~MainForm()
{
    delete ui;
}
void MainForm::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width()-20, this->height()-20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        color.setAlpha(100 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void MainForm::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mMousePoint = e->globalPos() - this->pos();
        e->accept();
        mDragWindow = true;
    }
}

void MainForm::mouseReleaseEvent(QMouseEvent *)
{
    mDragWindow = false;
}

void MainForm::mouseMoveEvent(QMouseEvent *e)
{
    if (mDragWindow && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mMousePoint);
        e->accept();
    }
}

void MainForm::on_CloseBtn_clicked()
{
    this->close();
}

void MainForm::MarkTableInit()
{
    ui->MarkTable->setFrameShape(QFrame::NoFrame);
    ui->MarkTable->setFocusPolicy(Qt::NoFocus);
    ui->MarkTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表格内容不可编辑
    ui->MarkTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次一行
    ui->MarkTable->setSelectionMode(QAbstractItemView::SingleSelection);//单选
    ui->MarkTable->horizontalHeader()->resizeSection(0,50);
    ui->MarkTable->horizontalHeader()->resizeSection(1,160);
    ui->MarkTable->horizontalHeader()->resizeSection(2,73);
    QTextEdit *tb1 = new QTextEdit();
    tb1->setStyleSheet("border:none;margin:0.5px;");
    tb1->setReadOnly(true);
    tb1->setFont(QFont("Consolas",10));
    tb1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示垂直滚动条
    tb1->setText("#");
    tb1->setFixedWidth(50-2);
    tb1->document()->setPageSize(QSizeF(50-4,1));
    ui->MarkTable->setCellWidget(0,0,tb1);
    ui->MarkTable->setItem(0,1,new QTableWidgetItem(QString::fromWCharArray(L"记号")));
    ui->MarkTable->setItem(0,2,new QTableWidgetItem(QString::fromWCharArray(L"属性")));

    ui->MarkTable->setFont(QFont("Consolas",10));
    ui->MarkTable->resizeRowsToContents();
    ui->MarkTable->setRowHeight(0,30);
}
void MainForm::MarkTableRefresh(int count)
{
    ui->MarkTable->clear();
    QTextEdit *tb1 = new QTextEdit();
    tb1->setStyleSheet("border:none;margin:0.5px;");
    tb1->setReadOnly(true);
    tb1->setFont(QFont("Consolas",10));
    tb1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示垂直滚动条
    tb1->setText("#");
    tb1->setFixedWidth(50-2);
    tb1->document()->setPageSize(QSizeF(50-4,1));
    ui->MarkTable->setCellWidget(0,0,tb1);
    ui->MarkTable->setItem(0,1,new QTableWidgetItem(QString::fromWCharArray(L"记号")));
    ui->MarkTable->setItem(0,2,new QTableWidgetItem(QString::fromWCharArray(L"属性")));
    ui->MarkTable->setRowCount(count+1);

    //填充表格
    for(int i=1;i<count+1;i++)
    {

        QTextEdit *tb = new QTextEdit();

        tb->setStyleSheet("border:none;margin:3px 0px 0px 0px;");
        tb->setReadOnly(true);
        tb->setFont(QFont("Consolas",10));
        tb->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示垂直滚动条
        tb->setFixedWidth(160-2);//宽度与单元格宽度一致
        tb->setText(getSstring(LIST[i-1].first,LIST[i-1].second));//添加记号
        tb->document()->setPageSize(QSizeF(160-4,1));//设置纸张大小,要的是宽度,高度不为负数即可
        int h = tb->document()->size().height()+6;//+6原因同上 Margin
        ui->MarkTable->setCellWidget(i,1,tb);

        QTextEdit *tb1 = new QTextEdit();
        tb1->setStyleSheet("border:none;margin:0.5px;");
        tb1->setReadOnly(true);
        tb1->setFont(QFont("Consolas",10));
        tb1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示垂直滚动条
        tb1->setText(QString::number(i));
        tb1->setFixedWidth(50-2);
        tb1->document()->setPageSize(QSizeF(50-4,1));
        ui->MarkTable->setCellWidget(i,0,tb1);

        QTextEdit *tb2 = new QTextEdit();
        tb2->setStyleSheet("border:none;margin:3px 0px 0px 0px;");
        tb2->setReadOnly(true);
        tb2->setFont(QFont("Consolas",10));
        tb2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示垂直滚动条
        tb2->setText(QString::fromStdString(LIST[i-1].first));
        tb2->setFixedWidth(73-2);
        tb2->document()->setPageSize(QSizeF(73-4,1));
        ui->MarkTable->setCellWidget(i,2,tb2);

        ui->MarkTable->setRowHeight(i,h);
    }
}
QString MainForm::getSstring(string &type, int &pos)
{

    if(type=="num")
    {

        return QString::fromStdString(NUM[pos]);
    }
    else if(type=="key")
    {
        return QString::fromStdString(KEY[pos]);
    }
    else if(type=="ID")
    {
        return QString::fromStdString(ID[pos]);
    }
    else if(type=="op")
    {
        return QString::fromStdString(OP[pos]);
    }
    else if(type=="delimiter")
    {
        return QString::fromStdString(DELIMITER[pos]);
    }
    else if(type=="string")
    {
        return QString::fromLocal8Bit(STRING[pos].c_str());
    }
    else return "error";
}
void MainForm::on_pushButton_clicked()
{
     Path = QFileDialog::getOpenFileName(this,"Open C Source File",".","*.c");
     if(Path=="")return;
     ResetList();
     Analysis();
     CodeLoad();
     AnalysisInfoLoad();
     MarkTableRefresh(LIST.size());
}
void MainForm::CodeLoad(void)
{
    ui->textEdit->clear();
    fstream in;
    in.open(Path.toStdString(),ios::in);
    string t;
    while(getline(in,t))
    {
        for(int i=0;i<t.length();i++)
        {
            if(t[i]=='\t')//制表符替换三个空格更加美观
            {
                t[i]=' ';
                t.insert(i,"  ");
            }
        }
        ui->textEdit->append(QString::fromLocal8Bit(t.c_str()));
    }
    in.close();
}
void MainForm::AnalysisInfoLoad(void)
{
    ui->textBrowser->clear();
    //拼接统计信息（行数，字符数）
    QString t=QString::fromWCharArray(L"LINE：")+QString::number(lineCount)+"    "+QString::fromWCharArray(L"CHAR：")
            +QString::number(charCount)+"    "+QString::fromWCharArray(L"WORD：")+QString::number(LIST.size())+"\n";
    //错误信息append
    for(int i=0;i<ERROR.size();i++)
    {
        t.append(QString::fromStdString(ERROR[i]));
    }
    ui->textBrowser->setFont(QFont(QString::fromWCharArray(L"Calibri"),11));
    ui->textBrowser->setText(t);
}
