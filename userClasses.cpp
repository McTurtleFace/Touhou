#include "userClasses.hpp"

#include <chrono>
#include <thread>

extern bool renderSemaphore;


unsigned short Kappa::render(Screen * screen){
    this->renderer(screen);
    if (!(this->timeAlive() % 2000)) this->shoot();
    return this->collider(screen);
}

unsigned short UFO::render(Screen * screen){
    this->renderer(screen);
    if (!(this->timeAlive() % 2000)) this->shoot();
    return this->collider(screen);
}

unsigned short NierHack::render(Screen * screen){
    this->renderer(screen);
    if (!(this->timeAlive() % 2000)) this->shoot();
    return this->collider(screen);
}

unsigned short Kappa::collider(Screen * screen){
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
            if (sprite->image.pixelColor(j,i).alpha() == 255 &&
                (sprite->image.pixelColor(j-1,i).alpha() == 0 ||
                 sprite->image.pixelColor(j+1,i).alpha() == 0 ||
                 sprite->image.pixelColor(j,i-1).alpha() == 0 ||
                 sprite->image.pixelColor(j,i+1).alpha() == 0)) {
                if (screen->collision[1][j][i]) returnValue = 4;
            }
        }
    }

    return returnValue;
}

unsigned short UFO::collider(Screen * screen){
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
            if (sprite->image.pixelColor(j,i).alpha() == 255 &&
                (sprite->image.pixelColor(j-1,i).alpha() == 0 ||
                 sprite->image.pixelColor(j+1,i).alpha() == 0 ||
                 sprite->image.pixelColor(j,i-1).alpha() == 0 ||
                 sprite->image.pixelColor(j,i+1).alpha() == 0)) {
                if (screen->collision[1][j][i]) returnValue = 4;
            }
        }
    }

    return returnValue;
}

unsigned short NierHack::collider(Screen * screen){
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
            if (sprite->image.pixelColor(j,i).alpha() == 255 &&
                (sprite->image.pixelColor(j-1,i).alpha() == 0 ||
                 sprite->image.pixelColor(j+1,i).alpha() == 0 ||
                 sprite->image.pixelColor(j,i-1).alpha() == 0 ||
                 sprite->image.pixelColor(j,i+1).alpha() == 0)) {
                if (screen->collision[1][j][i]) returnValue = 4;
            }
        }
    }

    return returnValue;
}

/*
shooting should be moved to outside of render loop so that we dont emplace into the vector it is looping through

*/

void Kappa::shoot(){
    int newVelocity1[2] = {this->velocity[0] + 0, this->velocity[1] + 2};
    EnergyBall * newBall1 = new EnergyBall(this->position,newVelocity1,this->basicProjectile);

    this->visuals->emplace_back(newBall1);

    int newVelocity2[2] = {this->velocity[0] + 0, this->velocity[1] + 2};
    EnergyBall * newBall2 = new EnergyBall(this->position,newVelocity2,this->basicProjectile);

    this->visuals->emplace_back(newBall2);

    int newVelocity3[2] = {this->velocity[0] + 0, this->velocity[1] + 2};
    EnergyBall * newBall3 = new EnergyBall(this->position,newVelocity3,this->basicProjectile);

    this->visuals->emplace_back(newBall3);
}

void UFO::shoot(){
    ScreenSpike * screenSpike = new ScreenSpike(this->position,this->velocity,this->basicProjectile);
    this->visuals->emplace_back(screenSpike);
}

void NierHack::shoot(){
    int newVelocity1[2] = {this->velocity[0] + 0, this->velocity[1] + 2};
    EnergySpike * newSpike1 = new EnergySpike(this->position,newVelocity1,this->basicProjectile);

    this->visuals->emplace_back(newSpike1);

    int newVelocity2[2] = {this->velocity[0] + 0, this->velocity[1] + 2};
    EnergySpike * newSpike2 = new EnergySpike(this->position,newVelocity2,this->basicProjectile);

    this->visuals->emplace_back(newSpike2);

    int newVelocity3[2] = {this->velocity[0] + 0, this->velocity[1] + 2};
    EnergySpike * newSpike3 = new EnergySpike(this->position,newVelocity3,this->basicProjectile);

    this->visuals->emplace_back(newSpike3);
}

unsigned short EnergyBall::render(Screen * screen){
    this->renderer(screen);

    for (int i = 0; i < sprite->image.height(); i++){
        for (int j = 0; j < sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255){
                screen->collision[1][j+position[0]][i+position[1]] = true;
            }
            else screen->collision[1][j+position[0]][i+position[1]] = false;
        }
    }

    return this->collider(screen);
}

unsigned short EnergySpike::render(Screen * screen){
    this->renderer(screen);

    for (int i = 0; i < sprite->image.height(); i++){
        for (int j = 0; j < sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 255){
                screen->collision[1][j+position[0]][i+position[1]] = true;
            }
            else screen->collision[1][j+position[0]][i+position[1]] = false;
        }
    }

    return this->collider(screen);
}

void ScreenSpike::renderer(Screen * screen){


    if (!warningFired && warningTimer<15) {}

    while (renderSemaphore) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    renderSemaphore = true;
    // paint and render
    QPainter painter(&(screen->image));
    painter.drawImage(oldPosition[0],oldPosition[1],backgroundSnap);
    backgroundSnap = screen->image.copy(position[0],position[1],sprite->image.width(),sprite->image.height());
    backgroundSnap.setAlphaChannel(sprite->invertAlpha());
    painter.drawImage(position[0],position[1],sprite->image);
    screen->label->setPixmap(QPixmap::fromImage(screen->image));
    renderSemaphore = false;
}

unsigned short ScreenSpike::render(Screen * screen){
    this->renderer(screen);

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

    return this->collider(screen);
}

extern int playerLocation[2];

void Reitu::BinaryRise(){
    // 1
}

void Reitu::BinarySet(){
    // 0
}

