#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pm("~/marisa.jpeg"); // <- path to image file
    ui->label->setPixmap(pm);
    ui->label->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
