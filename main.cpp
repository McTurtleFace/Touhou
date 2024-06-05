#pragma once
#include "render.hpp"
#include "userClasses.hpp"

#include <vector>
#include <map>
#include <sys/resource.h>
#include <stdlib.h>
#include <new>

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

void spawnLoop(int currentBoss, QElapsedTimer * timer, vector<Visual *> * visuals, vector<Visual *> * newVisuals, Sprite * spriteList[NUMSPRITES]) {
    qint64 currentTime = timer->elapsed();
    switch(currentBoss){
    case 1:
        static u_int32_t currentStage = 0;

        switch(currentTime){
        case 0 ... 500000:
            if (currentStage != 0) break;

            int rightVelocity[2] = {2,2};
            int leftVelocity[2] = {-2,2};
            int rightPosition[2] = {600,100};
            int leftPosition[2] = {100,100};

            Kappa * kappaRight = new Kappa(rightVelocity,rightPosition,spriteList[0],spriteList[0],timer,newVisuals);
            visuals->emplace_back(kappaRight);

            Kappa * kappaLeft = new Kappa(leftVelocity,leftPosition,spriteList[0],spriteList[0],timer,newVisuals);
            visuals->emplace_back(kappaLeft);

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
    vector<Visual *> newVisuals;

    QApplication a(argc, argv);

    QLabel label;   // should make this into one step with the constructor
    Screen screen;
    screen.label = &label;
    screen.image.load(":/pictures/fourK.jpg");

    Sprite * spriteList[NUMSPRITES];

    spriteList[0] = new Sprite(":/pictures/images/reimuBase.png");

    QElapsedTimer animationTimer;
    int velocityasdf[2] = {0,0};    // pc should have default values for this
    int positionasdf[2] = {300,200};
    Character playerCharacter(velocityasdf,positionasdf,spriteList[0],spriteList[0],&animationTimer,&newVisuals);
    visuals.emplace_back(&playerCharacter);
    KeyEventHandler keyEventHandler(&playerCharacter);
    a.installEventFilter(&keyEventHandler);


    QTimer frame;
    animationTimer.start();
    frame.setInterval(1000/30);

    bool stillAlive = true;

    QObject::connect(&frame, &QTimer::timeout,[&](){
        int charactersToKill = 0;
        spawnLoop(1,&animationTimer,&visuals,&newVisuals,spriteList);

        screen.image.load(":/pictures/fourK.jpg");
        for (auto currentVisual = visuals.begin();
             currentVisual != visuals.end(); currentVisual++) {
            unsigned short returnValue = (*currentVisual)->render(&screen);
        }

        visuals.insert(visuals.end(), newVisuals.begin(), newVisuals.end());
        newVisuals.clear();

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
