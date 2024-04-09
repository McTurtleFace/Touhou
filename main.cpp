#include "mainwindow.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    QImage testImage;
    testImage.load("~/marisa.jpeg");

    QLabel w;
    w.setPixmap(QPixmap::fromImage(testImage));

    w.show();
    window.show();
    return a.exec();
}
