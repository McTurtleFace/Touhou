#include "render.hpp"
#include <iostream>
#include <chrono>
#include <thread>


bool renderSemaphore = false;

Sprite::Sprite(const char * imageFile){
    this->image.load(imageFile);


    int mesh[image.height()][image.width()];
    for (int i = 0; i<image.height(); i++){
        for (int j = 0; j < image.width(); j++) {
            // if (image.pixel(j,i) == qRgb(0,0,0)) std::cout << j << i << std::endl;
            mesh[i][j] = (image.pixel(j,i) == qRgb(255,255,255)) ? 0 : 255;
        }
    }

    image = image.convertToFormat(QImage::Format_ARGB32);
    QColor newPixelTransparency;

    for (int i = 0; i<image.height(); i++){
        for (int j = 0; j < image.width(); j++) {
            newPixelTransparency = image.pixelColor(j,i).rgba();
            newPixelTransparency.setAlpha(mesh[i][j]);
            image.setPixelColor(j,i,newPixelTransparency);
        }
    }
}

bool KeyEventHandler::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_W) {
            visual->velocity[0] = 0;
            visual->velocity[1] = -2;
        }
        else if (keyEvent->key() == Qt::Key_D) {
            visual->velocity[0] = 2;
            visual->velocity[1] = 0;
        }
        else if (keyEvent->key() == Qt::Key_A) {
            visual->velocity[0] = -2;
            visual->velocity[1] = 0;
        }
        else if (keyEvent->key() == Qt::Key_S) {
            visual->velocity[0] = 0;
            visual->velocity[1] = 2;
        }
    }

    return QObject::eventFilter(obj, event);
}

QImage Sprite::invertAlpha(){
    QImage invertedImage;
    invertedImage = image;

    for (int i = 0; i<image.height(); i++){
        for (int j = 0; j < image.width(); j++) {
            QColor newPixelTransparency = invertedImage.pixelColor(j,i).rgba();
            newPixelTransparency.setAlpha( -(newPixelTransparency.alpha()-128) +127);
            invertedImage.setPixelColor(j,i,newPixelTransparency);
        }
    }

    return invertedImage;
}

void Visual::renderer(Screen * screen) {
    oldPosition[0] = position[0];
    oldPosition[1] = position[1];
    position[0] += velocity[0];
    position[1] += velocity[1];

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

Character::Character(int velocityP[2], int positionP[2], Sprite * spriteP){
    this->velocity[0] = velocityP[0];
    this->velocity[1] = velocityP[1];

    this->position[0] = positionP[0];
    this->position[1] = positionP[1];

    this->loadSprite(spriteP);
}

unsigned short Visual::render(Screen * screen){
    this->renderer(screen);
    return this->collider(screen);
}

unsigned short Visual::collider(__attribute__((unused)) Screen * screen){
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

    return returnValue;
}

unsigned short PlayerCharacter::collider(Screen * screen){
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
                else if (screen->collision[1][j+1][i] ||
                         screen->collision[1][j-1][i] ||
                         screen->collision[1][j][i+1] ||
                         screen->collision[1][j][i-1]) returnValue = 2;
                else if (screen->collision[0][j][i]) returnValue = 3;
            }
        }
    }

    return returnValue;
}
