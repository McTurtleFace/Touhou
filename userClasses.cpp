#include "userClasses.hpp"

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

void Kappa::shoot(){
        int newVelocity[2] = {this->velocity[0] + }
    EnergyBall * newBall = new EnergyBall();

    this->visuals->emplace_back(newBall);
}

void UFO::shoot(){

}

void NierHack::shoot(){

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

unsigned short ScreenSpike::render(Screen * screen){
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
