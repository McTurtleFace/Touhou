#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the image
    QPixmap image(":/pictures/fourK.jpg"); // Update the path to your image
    // Resize the image to fit the QLabel
    QPixmap scaledImage = image.scaled(ui->label->size(), Qt::KeepAspectRatio);
    // Set the image to the QLabel
    ui->label->setPixmap(scaledImage);
    // Adjust QLabel's size policy to maintain aspect ratio
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

MainWindow::~MainWindow()
{
    delete ui;
}
