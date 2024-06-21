#include "render.hpp"
#include "userClasses.hpp"

#include <vector>
#include <string.h>
#include <thread>
#include <map>
#include <sys/resource.h>
#include <stdlib.h>
#include <new>
#include <typeinfo>

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

int playerLocation[2];

void spawnLoop(int currentBoss, QElapsedTimer * timer, vector<Visual *> * visuals, vector<Visual *> * newVisuals, Sprite * spriteList[NUMSPRITES]) {
    qint64 currentTime = timer->elapsed();

    switch(currentBoss){
    case 1:
        static u_int32_t currentStage = 0;

        switch(currentTime){
        case 0 ... 500: {
            if (currentStage != 0) break;

            int rightVelocity[2] = {2,2};
            int leftVelocity[2] = {-2,2};
            int rightPosition[2] = {600,100};
            int leftPosition[2] = {100,100};

            Kappa * kappaRight = new Kappa(rightVelocity,rightPosition,spriteList[1],spriteList[2],timer,newVisuals);
            newVisuals->emplace_back(kappaRight);

            Kappa * kappaLeft = new Kappa(leftVelocity,leftPosition,spriteList[1],spriteList[2],timer,newVisuals);
            newVisuals->emplace_back(kappaLeft);

            currentStage = 1;
            break;
        }
        case 6000 ... 7000: {
            if (currentStage != 1) break;

            int rightVelocity[2] = {0,2};
            int leftVelocity[2] = {0,2};
            int rightPosition[2] = {600,0};
            int leftPosition[2] = {100,0};

            NierHack * nierRight = new NierHack(rightVelocity,rightPosition,spriteList[3],spriteList[4],timer,newVisuals);
            newVisuals->emplace_back(nierRight);

            NierHack * nierLeft = new NierHack(leftVelocity,leftPosition,spriteList[3],spriteList[4],timer,newVisuals);
            newVisuals->emplace_back(nierLeft);

            currentStage = 2;
            break;
        }
        case 10000 ... 12000: {
            if (currentStage != 2) break;

            int velocity[2] = {0,0};
            int position[2] = {800,300};

            UFO * centerUFO = new UFO(velocity,position,spriteList[5],spriteList[6],timer,newVisuals);
            newVisuals->emplace_back(centerUFO);

            currentStage = 3;
            break;
        }
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
    vector<thread> renderThreads;

    QApplication a(argc, argv);

    QLabel label;   // should make this into one step with the constructor
    Screen screen(&label,":/pictures/fourK.jpg",":/pictures/images/boundingBox.jpg");

    Sprite * spriteList[NUMSPRITES];

    spriteList[0] = new Sprite(":/pictures/images/reimu.jpg");
    spriteList[1] = new Sprite(":/pictures/images/kappa.jpg");
    spriteList[2] = new Sprite(":/pictures/images/energyBall.jpg");
    spriteList[3] = new Sprite(":/pictures/images/nierHack.jpg");
    spriteList[4] = new Sprite(":/pictures/images/energySpike.jpg");
    spriteList[5] = new Sprite(":/pictures/images/ufo.jpg");
    spriteList[6] = new Sprite(":/pictures/images/screenLaser.jpg");
    spriteList[7] = new Sprite(":/pictures/images/playerProject.jpg");

    QElapsedTimer animationTimer;
    int velocityasdf[2] = {0,0};    // pc should have default values for this
    int positionasdf[2] = {500,800};
    PlayerCharacter * playerCharacter = new PlayerCharacter(velocityasdf,positionasdf,spriteList[0],spriteList[7],&animationTimer,&newVisuals);
    visuals.emplace_back(playerCharacter);
    KeyEventHandler keyEventHandler(playerCharacter);
    a.installEventFilter(&keyEventHandler);


    QTimer frame;
    animationTimer.start();
    frame.setInterval(1000/90);

    bool stillAlive = true;

    Screen * screenP = &screen;

    int points = 0, pointMod = 1;

    QObject::connect(&frame, &QTimer::timeout,[&](){
        playerLocation[0] = playerCharacter->position[0];
        playerLocation[1] = playerCharacter->position[1];

        if (stillAlive == false) a.quit();

        screen.collision[1][350][350] = true;

        spawnLoop(1,&animationTimer,&visuals,&newVisuals,spriteList);

        screen.image.load(":/pictures/images/viralHills.jpg");

        for (auto currentVisual = visuals.begin();
             currentVisual != visuals.end(); currentVisual++) {
            (*currentVisual)->nonThreadedRender(screenP);
        }

        vector<thread> visualThreads;
        for (auto currentVisual = visuals.begin();
             currentVisual != visuals.end(); currentVisual++) {
            thread newThread([currentVisual, screenP] { (*currentVisual)->render(screenP); });

            visualThreads.emplace_back(std::move(newThread));
        }

        for(auto& th : visualThreads) {
            th.join();
        }

        for (int i = visuals.size() - 1; i >= 0; i--) {
            switch (visuals[i]->renderState){
            case 1:
                visuals.at(i)->derenderer(screenP);
                delete visuals.at(i);
                visuals.erase(visuals.begin() + i);
                break;
            case 2:
                points += 100 * pointMod;
                break;
            case 3:
                pointMod ++;
                break;
            case 4:
                points += 150 * pointMod;
                visuals.at(i)->derenderer(screenP);
                delete visuals.at(i);
                visuals.erase(visuals.begin() + i);
                break;
            case 5:
                stillAlive = false;
                break;
            }
        }

        visuals.insert(visuals.end(), newVisuals.begin(), newVisuals.end());
        newVisuals.clear();

        screen.overlayBox();
        screen.label->show();
        a.processEvents();
    });

    frame.start();

    return a.exec();
}

int main(int argc, char *argv[])
{
    // taken from stack overflow
    const rlim_t kStackSize = 64L * 1024L * 1024L;   // min stack size = 16 MB
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
