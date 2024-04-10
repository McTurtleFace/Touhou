#include "mainwindow.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QPainter>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    QImage testImage;
    testImage.load(":/pictures/fourK.jpg");

    QImage topImage;
    topImage.load(":/pictures/images/koumakyou.jpeg");

    QLabel w;
    w.resize(testImage.size());

    QPainter painter(&testImage);
    painter.drawImage(0,0,topImage);

    w.setPixmap(QPixmap::fromImage(testImage));
    w.show();
    //window.show();
    return a.exec();
}
