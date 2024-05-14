#include "render.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QElapsedTimer>

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    vector<Visual*> visuals;

    QApplication a(argc, argv);

    QLabel label;

    Sprite spriteTwo(":/pictures/images/testing.jpeg");
    Screen screen;
    screen.label = &label;
    screen.image.load(":/pictures/fourK.jpg");

    Sprite sprite(":/pictures/images/koumakyou.jpeg");
    Sprite ufo(":/pictures/images/ufo");

    int velocityasdf[2] = {0,0};
    int positionasdf[2] = {300,200};
    Character visual(velocityasdf,positionasdf,&sprite);
    visuals.emplace_back(&visual);

    int velocity[2] = {-2,0};
    int position[2] = {500,0};
    Character ufoEnemy(velocity,position,&ufo);
    visuals.emplace_back(&ufoEnemy);

    KeyEventHandler keyEventHandler(&visual);
    a.installEventFilter(&keyEventHandler);



    QTimer frame;
    QElapsedTimer animationTimer;
    animationTimer.start();
    frame.setInterval(1000/30);

    bool stillAlive = true;

    QObject::connect(&frame, &QTimer::timeout,[&](){
        int charactersToKill = 0;

        screen.image.load(":/pictures/fourK.jpg");
        for (auto currentVisual = visuals.begin();
             currentVisual != visuals.end(); currentVisual++) {
            unsigned short returnValue = (*currentVisual)->render(&screen);
        }
        if (animationTimer.elapsed() > 20*1000) {
            animationTimer.restart();
        }
        screen.label->show();
        a.processEvents();
    });

    frame.start();

    return a.exec();
}
