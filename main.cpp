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
    visual.loadSprite(&sprite);
    visual.renderer(&screen);
    a.processEvents();

    QTimer timer;
    timer.setInterval(1000/125);

    QTimer::singleShot(1000, [&]() {
        Sprite secondSprite(":/pictures/images/testing.png");
        visual.loadSprite(&secondSprite);
        visual.renderer(&screen);
        a.processEvents(); // Process events again to update UI
    });

    return a.exec();
}
