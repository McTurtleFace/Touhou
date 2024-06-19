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
    if (!(this->timeAlive() % 2000)) this->shoot();
    this->renderState = this->collider(screen);
}

void NierHack::render(Screen * screen){
    static bool standingStill;

    this->renderer(screen);

    if (this->position[1] > 50) {
        this->velocity[0] = 0;
        this->velocity[1] = 0;
        standingStill = true;
    }
    else standingStill = false;
    if ((int)this->timeAlive() % 2000 <= 50 && standingStill) this->shoot();
    if ((int)this->timeAlive() > 30000 && standingStill) this->velocity[1] = -3;

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

    if (position[0] + velocity[0] < 0 &&
        position[0] + velocity[0] >= WIDTH &&
        position[1] + velocity[1] < 0 &&
        position[1] + velocity[1] >= HEIGHT) {
        returnValue = 1;
    }   // needs to account for image height and width

    // detect if colliding
    for (int i = 0; i<sprite->image.height(); i++){
        for (int j = 0; j<sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255) {
                if (screen->collision[2][j][i]) returnValue = 4;
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

    if (position[0] + velocity[0] < 0 &&
        position[0] + velocity[0] >= WIDTH &&
        position[1] + velocity[1] < 0 &&
        position[1] + velocity[1] >= HEIGHT) {
        returnValue = 1;
    }   // needs to account for image height and width

    // detect if colliding
    for (int i = 0; i<sprite->image.height(); i++){
        for (int j = 0; j<sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255) {
                if (screen->collision[2][j][i]) returnValue = 4;
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

    if (position[0] + velocity[0] < 0 &&
        position[0] + velocity[0] >= WIDTH &&
        position[1] + velocity[1] < 0 &&
        position[1] + velocity[1] >= HEIGHT) {
        returnValue = 1;
    }   // needs to account for image height and width

    // detect if colliding
    for (int i = 0; i<sprite->image.height(); i++){
        for (int j = 0; j<sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255) {
                if (screen->collision[2][j][i]) returnValue = 4;
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
    ScreenSpike * screenSpike = new ScreenSpike(this->position,this->velocity,this->basicProjectile);
    this->visuals->emplace_back(screenSpike);
}

void NierHack::shoot() {
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

void EnergyBall::render(Screen * screen) {
    for (int i = 0; i < sprite->image.height(); i++){
        for (int j = 0; j < sprite->image.width(); j++){
            if (j+position[0]>0 && j+position[0]<WIDTH &&
                i+position[1]>0 && i+position[1]<HEIGHT){
                if (sprite->image.pixelColor(j,i).alpha() == 255){
                    screen->collision[1][j+position[0]][i+position[1]] = true;
                }
                else screen->collision[1][j+position[0]][i+position[1]] = false;
            }
        }
    }

    this->renderState = this->collider(screen);
}

void EnergySpike::render(Screen * screen) {
    for (int i = 0; i < sprite->image.height(); i++){
        for (int j = 0; j < sprite->image.width(); j++){
            if (j+position[0] > 0 && j+position[0] < WIDTH &&
                i+position[1] > 0 && i+position[1] < HEIGHT) {
                if (sprite->image.pixelColor(j,i).alpha() == 255){
                    screen->collision[1][j+position[0]][i+position[1]] = true;
                }
                else screen->collision[1][j+position[0]][i+position[1]] = false;
            }
        }
    }

    this->renderState = this->collider(screen);
}

void EnergySpike::rotate(double rotation){
    this->rotatedSprite.image = this->sprite->image.transformed(QTransform().rotate(rotation));
    this->sprite = &(this->rotatedSprite);
}

void ScreenSpike::renderer(Screen * screen) {


    if (!warningFired && warningTimer<15) {}

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

void ScreenSpike::render(Screen * screen) {
    if (warningFired){
        for (int i = 0; i < sprite->image.height(); i++){
            for (int j = 0; j < sprite->image.width(); j++){
                if (sprite->image.pixelColor(j,i).alpha() == 255){
                    screen->collision[1][j+position[0]][i+position[1]] = true;
                }
                else screen->collision[1][j+position[0]][i+position[1]] = false;
            }
        }
    }

    this->renderState = this->collider(screen);
}

extern int playerLocation[2];
/*
void ReituRise::rotate(double rotate) {
    QImage img("src.jpg");
    QImage rotatedImg = img.transformed(QMatrix().rotate(90.0));
    rotatedImg.save("rotated.jpg");
}
*/


void Reitu::BinaryRise() {
    if (this->riseLeft  == 0) return;



    riseLeft--;
    if (this->riseLeft == 0) riseLeft = 5;


}

void Reitu::BinarySet() {

}

