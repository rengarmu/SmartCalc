#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <iomanip>
#include <iostream>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(BasicFunc()));
  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_point, SIGNAL(clicked()), this,
          SLOT(PushButton_func()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this,
          SLOT(PushButton_func()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this,
          SLOT(PushButton_func()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this,
          SLOT(PushButton_func()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this,
          SLOT(PushButton_func()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this,
          SLOT(PushButton_func()));
  connect(ui->pushButton_opn, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_cls, SIGNAL(clicked()), this, SLOT(PushButton_func()));
  connect(ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(ClearLine()));
  connect(ui->pushButton_graph, SIGNAL(clicked()), this, SLOT(GraphFrame()));
  connect(ui->pushButton_credit, SIGNAL(clicked()), this, SLOT(CreditFrame()));
  connect(ui->pushButton_creditres, SIGNAL(clicked()), this,
          SLOT(CreditFunc()));
  ui->frame_graph->hide();
  ui->frame_credit->hide();
  ui->label_2->hide();
  ui->radioButton_dif->click();
  ui->label->setText("");
  ui->doubleSpinBox_xmin->setValue(-10.0);
  ui->doubleSpinBox_xmax->setValue(10.0);
  setFixedSize(ui->frame->width() + 2, ui->frame->height() + 50);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::BasicFunc() {
  QMessageBox MyB;
  double result{};
  result = ui->doubleSpinBox_x->value();
  double x_min = ui->doubleSpinBox_xmin->value();
  double x_max = ui->doubleSpinBox_xmax->value();
  QString str = ui->lineEdit->text();
  QByteArray ba;
  ba = str.toLatin1();
  int err_code;
  err_code = smartcalc(ba.data(), &result);
  if (err_code == 0) {
    ui->label->setText(QString::number(result));
  } else if (err_code == -1) {
    ui->label->setText("Неправильно введено число");
  } else if (err_code == -2) {
    ui->label->setText("Неправильно введен оператор");
  } else if (err_code == -3) {
    ui->label->setText("Неправильная последовательность операторов");
  }
  ui->label_2->setText(QString::number(err_code));

  if (ui->frame_graph->isVisible()) {
    PrintGrafic(x_min, x_max, ba.data());
  }
}

void MainWindow::PushButton_func() {
  QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
  if (clickedButton) {
    ui->lineEdit->insert(clickedButton->text());
  }
}

void MainWindow::ClearLine() {
  ui->lineEdit->clear();
  ui->doubleSpinBox_x->clear();
}

void MainWindow::PrintGrafic(double x_min, double x_max, char* str) {
  QVector<double> x;
  QVector<double> y;
  ui->widget->xAxis->setRangeLower(x_min);
  ui->widget->xAxis->setRangeUpper(x_max);
  ui->widget->yAxis->setRangeLower(x_min);
  ui->widget->yAxis->setRangeUpper(x_max);

  double tmp = 0;
  double step = (x_max - x_min) / 1000;
  for (double x_count = x_min; x_count < x_max; x_count += step) {
    x.push_back(x_count);
    tmp = x_count;
    smartcalc(str, &tmp);
    y.push_back(tmp);
  }

  ui->widget->removeGraph(0);
  ui->widget->addGraph();
  ui->widget->graph(0)->addData(x, y);
  ui->widget->replot();
}

void MainWindow::GraphFrame() {
  ui->frame_credit->hide();
  if (ui->frame_graph->isVisible()) {
    ui->frame_graph->hide();
    setFixedSize(ui->frame->width() + 2, ui->frame->height() + 50);
  } else {
    ui->frame_graph->show();
    setFixedSize(ui->frame->width() + 25 + ui->frame_plus->width(),
                 ui->frame->height() + 50);
  }
}

void MainWindow::CreditFrame() {
  ui->frame_graph->hide();
  if (ui->frame_credit->isVisible()) {
    ui->frame_credit->hide();
    setFixedSize(ui->frame->width() + 2, ui->frame->height() + 50);
  } else {
    ui->frame_credit->show();
    setFixedSize(ui->frame->width() + 25 + ui->frame_plus->width(),
                 ui->frame->height() + 50);
  }
}

void MainWindow::CreditFunc() {
  ui->tableWidget->setRowCount(ui->spinBox_months->value());
  if (ui->radioButton_ann->isChecked()) {
    qDebug() << "ann";
    PrintAnn();
  } else {
    qDebug() << "dif";
    PrintDiff();
  }
}

void MainWindow::PrintAnn() {
  double sum = ui->doubleSpinBox_sum->value();
  double percent = ui->doubleSpinBox_percent->value();
  int months = ui->spinBox_months->value();
  double per_month = ((1 / 12.0) * percent) / 100.0;
  double payment = sum * (per_month / (1 - pow(1 + per_month, -months)));
  double sum_to_months = sum / months;
  double balance = sum;
  double per_to_months = 0;
  double pay_to_months = 0;
  double sum_pay = 0;
  for (int i = 0; i < months; ++i) {
    per_to_months = balance * (percent / 12.0) / 100.0;
    pay_to_months = payment - per_to_months;
    balance -= pay_to_months;
    sum_pay += payment;
    ui->tableWidget->setItem(
        i, 0, new QTableWidgetItem(QString::number(payment, 'f', 2)));
    ui->tableWidget->setItem(
        i, 1, new QTableWidgetItem(QString::number(pay_to_months, 'f', 2)));
    ui->tableWidget->setItem(
        i, 2, new QTableWidgetItem(QString::number(per_to_months, 'f', 2)));
    ui->tableWidget->setItem(
        i, 3, new QTableWidgetItem(QString::number(balance, 'f', 2)));
  }
  ui->label_allsum->setText(QString::number(sum_pay));
  ui->label_allper->setText(QString::number(sum_pay - sum));
}

void MainWindow::PrintDiff() {
  double sum = ui->doubleSpinBox_sum->value();
  double percent = ui->doubleSpinBox_percent->value();
  int months = ui->spinBox_months->value();
  double per_month = ((1 / 12.0) * percent) / 100.0;
  double payment = sum / months + sum * (percent / 100) / months;
  double sum_to_months = sum / months;
  double balance = sum;
  double per_to_months = 0;
  double pay_to_months = sum / months;
  double sum_pay = 0;
  for (int i = 0; i < months; ++i) {
    per_to_months =
        ((sum - (sum / months * i)) * (percent / 100) * (365.0 / 12.0 / 365.0));
    payment = per_to_months + pay_to_months;
    balance -= pay_to_months;
    sum_pay += payment;
    ui->tableWidget->setItem(
        i, 0, new QTableWidgetItem(QString::number(payment, 'f', 2)));
    ui->tableWidget->setItem(
        i, 1, new QTableWidgetItem(QString::number(pay_to_months, 'f', 2)));
    ui->tableWidget->setItem(
        i, 2, new QTableWidgetItem(QString::number(per_to_months, 'f', 2)));
    ui->tableWidget->setItem(
        i, 3, new QTableWidgetItem(QString::number(balance, 'f', 2)));
  }
  ui->label_allsum->setText(QString::number(sum_pay));
  ui->label_allper->setText(QString::number(sum_pay - sum));
}
