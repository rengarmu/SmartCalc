#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QApplicant>
#include <QVector>
#include "front/qcustomplot.h"

extern "C"{
#include "back/s21_smartcalc.h"
}


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void BasicFunc();
    void PushButton_func();
    void ClearLine(); 
    void GraphFrame();
    void CreditFrame();
    void CreditFunc();

private:
    Ui::MainWindow *ui;
    void PrintGrafic(double x_min, double x_max, char* str);
    void PrintDiff();
    void PrintAnn();
};
#endif // MAINWINDOW_H
