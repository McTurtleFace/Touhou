#include "userClasses.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <QTransform>


void Kappa::render(Screen * screen) {
    static bool standingStill;

    this->renderer(screen);

    if (this->position[1] > 150) {
        this->velocity[0] = 0;
        this->velocity[1] = 0;
        standingStill = true;
    }
    else standingStill = false;
    if ((int)this->timeAlive() % 2000 <= 50 && standingStill) this->shoot();
    if ((int)this->timeAlive() > 30000 && standingStill) this->velocity[1] = -3;

    this->renderState = this->collider(screen);
}

void UFO::render(Screen * screen) {
    this->renderer(screen);
    if (!shotYet) shoot();
    this->renderState = this->collider(screen);
}

void NierHack::render(Screen * screen){
    this->renderer(screen);

    if ((int)this->timeAlive() % 2000 <= 50) this->shoot();

    this->renderState = this->collider(screen);
}

unsigned short Kappa::collider(Screen * screen) {
    /*
===============================================================

render return values:
0 = all good, continue
1 = out of bounds, derender
2 = graze, if pc give graze points
3 = power, if pc give power points
4 = colliding with particle, kill

===============================================================
    */

    unsigned short returnValue = 0;

    if (position[0] + velocity[0] < 0 ||
        position[0] + velocity[0] >= WIDTH ||
        position[1] + velocity[1] < 0 ||
        position[1] + velocity[1] >= HEIGHT) {
        returnValue = 1;
    }   // needs to account for image height and width

    // detect if colliding
    for (int i = 0; i<sprite->image.height(); i++){
        for (int j = 0; j<sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255) {
                if (screen->collision[2][position[0]+j][position[1]+i]) returnValue = 4;
            }
        }
    }

    return returnValue;
}

unsigned short UFO::collider(Screen * screen) {
    /*
===============================================================

render return values:
0 = all good, continue
1 = out of bounds, derender
2 = graze, if pc give graze points
3 = power, if pc give power points
4 = colliding with particle, kill

===============================================================
    */

    unsigned short returnValue = 0;

    if (position[0] < 0 ||
        position[0] >= WIDTH ||
        position[1] + velocity[1] < 0 ||
        position[1] + velocity[1] >= HEIGHT) {
        returnValue = 1;
    }   // needs to account for image height and width

    // detect if colliding
    for (int i = 0; i<sprite->image.height(); i++){
        for (int j = 0; j<sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255) {
                if (screen->collision[2][position[0]+j][position[1]+i]) returnValue = 4;
            }
        }
    }

    return returnValue;
}

unsigned short NierHack::collider(Screen * screen) {
    /*
===============================================================

render return values:
0 = all good, continue
1 = out of bounds, derender
2 = graze, if pc give graze points
3 = power, if pc give power points
4 = colliding with particle, kill

===============================================================
    */

    unsigned short returnValue = 0;

    if (position[0] + velocity[0] < 0 ||
        position[0] + velocity[0] >= WIDTH ||
        position[1] + velocity[1] < 0 ||
        position[1] + velocity[1] >= HEIGHT) {
        returnValue = 1;
    }   // needs to account for image height and width

    // detect if colliding
    for (int i = 0; i<sprite->image.height(); i++){
        for (int j = 0; j<sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255) {
                if (screen->collision[2][position[0]+j][position[1]+i]) returnValue = 4;
            }
        }
    }

    return returnValue;
}

void Kappa::shoot() {
    int newVelocity1[2] = {1,2};
    EnergyBall * newBall1 = new EnergyBall(newVelocity1,this->position,this->basicProjectile);

    this->visuals->emplace_back(newBall1);

    int newVelocity2[2] = {0,2};
    EnergyBall * newBall2 = new EnergyBall(newVelocity2,this->position,this->basicProjectile);

    this->visuals->emplace_back(newBall2);

    int newVelocity3[2] = {-1,2};
    EnergyBall * newBall3 = new EnergyBall(newVelocity3,this->position,this->basicProjectile);

    this->visuals->emplace_back(newBall3);
}

void UFO::shoot() {
    int newPosition[2] = {this->position[0],0};
    int newVelocity[2] = {0,0};
    ScreenSpike * screenSpike = new ScreenSpike(newVelocity,newPosition,this->basicProjectile);
    screenSpike->warningSprite = new Sprite(":/pictures/images/warning.jpg");
    this->visuals->emplace_back(screenSpike);
    this->shotYet = true;
}

void NierHack::shoot() {
    int newVelocity1[2] = {1,4};
    int newPosition1[2] = {this->position[0]-10,this->position[1] + 50};
    EnergySpike * newSpike1 = new EnergySpike(newVelocity1,newPosition1,this->basicProjectile);
    newSpike1->rotate(340);

    this->visuals->emplace_back(newSpike1);

    int newVelocity2[2] = {-1,4};
    int newPosition2[2] = {this->position[0]-20,this->position[1] + 50};
    EnergySpike * newSpike2 = new EnergySpike(newVelocity2,newPosition2,this->basicProjectile);
    newSpike2->rotate(20);

    this->visuals->emplace_back(newSpike2);
}

void EnergySpike::rotate(double rotation){
    rotatedSprite = new Sprite(":/pictures/images/k.jpg");

    rotatedSprite->image = this->sprite->image.transformed(QTransform().rotate(rotation));
    this->sprite = rotatedSprite;
}

void ScreenSpike::renderer(Screen * screen) {
    if (timer<15) {

        while (screen->renderSemaphore) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        screen->renderSemaphore = true;
        // paint and render
        QPainter painter(&(screen->image));
        painter.drawImage(oldPosition[0],oldPosition[1],backgroundSnap);
        backgroundSnap = screen->image.copy(position[0],position[1],warningSprite->image.width(),warningSprite->image.height());
        backgroundSnap.setAlphaChannel(warningSprite->invertAlpha());
        painter.drawImage(position[0],position[1],warningSprite->image);
        screen->label->setPixmap(QPixmap::fromImage(screen->image));
        screen->renderSemaphore = false;
    }
    else if (timer > 70) this->renderState = 1;
    else {
        if (timer < 65) warningFired = true;
        else warningFired = false;

        while (screen->renderSemaphore) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        screen->renderSemaphore = true;
        // paint and render
        QPainter painter(&(screen->image));
        painter.drawImage(oldPosition[0],oldPosition[1],backgroundSnap);
        backgroundSnap = screen->image.copy(position[0],position[1],sprite->image.width(),sprite->image.height());
        backgroundSnap.setAlphaChannel(sprite->invertAlpha());
        painter.drawImage(position[0],position[1],sprite->image);
        screen->label->setPixmap(QPixmap::fromImage(screen->image));
        screen->renderSemaphore = false;
    }

    timer++;
}

extern int playerLocation[2];

void Reitu::BinaryRise() {
    int newVelocity1[2] = {-5,5};
    int newPosition1[2] = {playerLocation[0]-newVelocity1[0], playerLocation[1]-newVelocity1[1]};
    EnergySpike * newSpike1 = new EnergySpike(newVelocity1,newPosition1,this->basicProjectile);
    newSpike1->rotate(tan(newPosition1[1]/newPosition1[0]));

    this->visuals->emplace_back(newSpike1);

    int newVelocity2[2] = {-2,8};
    int newPosition2[2] = {playerLocation[0]-newVelocity2[0], playerLocation[1]-newVelocity2[1]};
    EnergySpike * newSpike2 = new EnergySpike(newVelocity2,newPosition2,this->basicProjectile);
    newSpike1->rotate(tan(newPosition2[1]/newPosition2[0]));

    this->visuals->emplace_back(newSpike2);

    int newVelocity3[2] = {0,10};
    int newPosition3[2] = {playerLocation[0]-newVelocity3[0], playerLocation[1]-newVelocity3[1]};
    EnergySpike * newSpike3 = new EnergySpike(newVelocity3,newPosition3,this->basicProjectile);
    newSpike1->rotate(tan(newPosition3[1]/newPosition3[0]));

    this->visuals->emplace_back(newSpike3);

    int newVelocity4[2] = {2,8};
    int newPosition4[2] = {playerLocation[0]-newVelocity4[0], playerLocation[1]-newVelocity4[1]};
    EnergySpike * newSpike4 = new EnergySpike(newVelocity4,newPosition4,this->basicProjectile);
    newSpike1->rotate(tan(newPosition4[1]/newPosition4[0]));

    this->visuals->emplace_back(newSpike4);

    int newVelocity5[2] = {5,5};
    int newPosition5[2] = {playerLocation[0]-newVelocity5[0], playerLocation[1]-newVelocity5[1]};
    EnergySpike * newSpike5 = new EnergySpike(newVelocity5,newPosition5,this->basicProjectile);
    newSpike1->rotate(tan(newPosition5[1]/newPosition5[0]));

    this->visuals->emplace_back(newSpike5);
}

void Reitu::BinarySet() {

}

