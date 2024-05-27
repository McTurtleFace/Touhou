#include "render.hpp"
#include "userClasses.hpp"

#include <vector>
#include <map>
#include <sys/resource.h>
#include <stdlib.h>

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QElapsedTimer>

#include <iostream>

using namespace std;

void spawnLoop(int currentBoss, qint64 currentTime, vector<Visual *> * visuals, Sprite * spriteList[NUMSPRITES]) {
    switch(currentBoss){
    case 1:
        static u_int32_t currentStage = 0;

        switch(currentTime){
        case 0 ... 500000:
            if (currentStage != 0) break;

            int newVelocity[2] = {0,0};
            int newPosition[2] = {100,100};
            Character * newCharacter = new Character(newVelocity,newPosition,spriteList[0]);
            visuals->emplace_back(newCharacter);

            currentStage = 1;
        }
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;
        // put menu screen stuff here
    }
}

int threadedMain(int argc, char *argv[]){
    vector<Visual *> visuals;

    QApplication a(argc, argv);

    QLabel label;   // should make this into one step with the constructor
    Screen screen;
    screen.label = &label;
    screen.image.load(":/pictures/fourK.jpg");

    Sprite * spriteList[NUMSPRITES];

    spriteList[0] = new Sprite(":/pictures/images/reimuBase.png");

    int velocityasdf[2] = {0,0};    // pc should have default values for this
    int positionasdf[2] = {300,200};
    Character playerCharacter(velocityasdf,positionasdf,spriteList[0]);
    visuals.emplace_back(&playerCharacter);
    KeyEventHandler keyEventHandler(&playerCharacter);
    a.installEventFilter(&keyEventHandler);


    QTimer frame;
    QElapsedTimer animationTimer;
    animationTimer.start();
    frame.setInterval(1000/30);

    bool stillAlive = true;

    QObject::connect(&frame, &QTimer::timeout,[&](){
        int charactersToKill = 0;
        spawnLoop(1,animationTimer.elapsed(),&visuals,spriteList);

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

int main(int argc, char *argv[])
{
    // taken from stack overflow
    const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
    // all my code from here on out

    return threadedMain(argc,argv);
}
