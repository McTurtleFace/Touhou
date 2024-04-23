#include "mainwindow.hpp"
#include "render.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLabel label;
    Screen screen;
    screen.label = &label;
    screen.image.load(":/pictures/fourK.jpg");
    Visual visual;
    Sprite sprite(":/pictures/images/koumakyou.jpeg");
    visual.sprite = &sprite;
    visual.renderer(&screen);

    return a.exec();
}
