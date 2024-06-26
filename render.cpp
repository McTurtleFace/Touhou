#include "render.hpp"
#include <iostream>
#include <chrono>
#include <thread>

void scoreShow(Screen * screen, int score){
    while (screen->renderSemaphore) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    screen->renderSemaphore = true;
    // paint and render
    QPainter painter(&(screen->image));
    painter.setPen(QPen(Qt::white));
    painter.drawText(QRect(1300,400,1800,700),QString::number(score));
    screen->label->setPixmap(QPixmap::fromImage(screen->image));
    screen->renderSemaphore = false;
}


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
            if (visual->position[1] < 10) visual->velocity[1] = 0;
        }
        else if (keyEvent->key() == Qt::Key_D) {
            visual->velocity[0] = 2;
            visual->velocity[1] = 0;
            if (visual->position[0] > 1010) visual->velocity[0] = 0;
        }
        else if (keyEvent->key() == Qt::Key_A) {
            visual->velocity[0] = -2;
            visual->velocity[1] = 0;
            if (visual->position[0] < 190) visual->velocity[0] = 0;
        }
        else if (keyEvent->key() == Qt::Key_S) {
            visual->velocity[0] = 0;
            visual->velocity[1] = 2;
            if (visual->position[1] > 1070) visual->velocity[1] = 0;
        }
        else if (keyEvent->key() == Qt::Key_Space) {
            visual->shooting = true;
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

void Visual::derenderer(Screen * screen){
    while (screen->renderSemaphore) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    screen->renderSemaphore = true;
    // paint and render
    QPainter painter(&(screen->image));
    painter.drawImage(oldPosition[0],oldPosition[1],backgroundSnap);
    backgroundSnap = screen->image.copy(position[0],position[1],sprite->image.width(),sprite->image.height());
    backgroundSnap.setAlphaChannel(sprite->invertAlpha());
    screen->label->setPixmap(QPixmap::fromImage(screen->image));
    screen->renderSemaphore = false;
}

void Visual::move(){
    oldPosition[0] = position[0];
    oldPosition[1] = position[1];
    position[0] += velocity[0];
    position[1] += velocity[1];
}

Character::Character(int velocityP[2], int positionP[2], Sprite * spriteP, Sprite * basicProjectileP, QElapsedTimer * timerP, std::vector<Visual *> * newVisuals){
    this->velocity[0] = velocityP[0];
    this->velocity[1] = velocityP[1];

    this->position[0] = positionP[0];
    this->position[1] = positionP[1];

    this->loadSprite(spriteP);
    this->basicProjectile = basicProjectileP;

    this->spawnTime = timerP->elapsed();
    this->timer = timerP;

    this->visuals = newVisuals;
}

Particle::Particle(int velocityP[2], int positionP[2], Sprite * spriteP){
    this->velocity[0] = velocityP[0];
    this->velocity[1] = velocityP[1];

    this->position[0] = positionP[0];
    this->position[1] = positionP[1];

    this->loadSprite(spriteP);
}

void Visual::render(Screen * screen){
    this->renderState = this->collider(screen);
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
5 = player is dead, kill program

===============================================================
    */

    unsigned short returnValue = 0;

    if (position[0] + velocity[0] < 0 ||
        position[0] + velocity[0] >= WIDTH ||
        position[1] + velocity[1] < 0 ||
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
                if (screen->collision[1][position[0]+j][position[1]+i]) returnValue = 5;
                else if (screen->collision[1][position[0]+j+5][position[1]+i] ||
                         screen->collision[1][position[0]+j-5][position[1]+i] ||
                         screen->collision[1][position[0]+j][position[1]+i+5] ||
                         screen->collision[1][position[0]+j][position[1]+i-5]) {
                    returnValue = 2;
                }
                else if (screen->collision[0][j][i]) returnValue = 3;
            }
        }
    }

    return returnValue;
}


Screen::Screen(QLabel * labelP,const char * imageName, const char * boundName) {
    this->label = labelP;
    this->image.load(imageName);
    this->boundingBox = new Sprite(boundName);

    for (int i = 0; i<boundingBox->image.height(); i++){
        for (int j = 0; j<boundingBox->image.width(); j++){
            if (boundingBox->image.pixelColor(j,i).alpha() == 255) {
                this->collision[5][j][i] = true;
            }
        }
    }
}

void Screen::overlayBox() {

    while (renderSemaphore) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    renderSemaphore = true;
    // paint and render
    QPainter painter(&(image));
    painter.drawImage(0,0,boundingBox->image);
    label->setPixmap(QPixmap::fromImage(image));
    renderSemaphore = false;
}

qint64 NonPlayerCharacter::timeAlive() {
    return this->timer->elapsed() - spawnTime;
}

void Particle::render(Screen * screen) {
    for (int i = 0; i < sprite->image.height(); i++){
        for (int j = 0; j < sprite->image.width(); j++){
            if (j+position[0]>0 && j+position[0]<WIDTH &&
                i+position[1]>0 && i+position[1]<HEIGHT){
                if (sprite->image.pixelColor(j,i).alpha() == 255){
                    screen->collision[renderLayer][j+position[0]][i+position[1]] = true;
                }
                else screen->collision[renderLayer][j+position[0]][i+position[1]] = false;
            }
        }
    }

    this->renderState = this->collider(screen);
}

void PlayerCharacter::shoot() {
    int newVelocity[2] = {0,-5};
    int newPosition[2] = {this->position[0],this->position[1]-35};
    Particle * newShot = new Particle(newVelocity,newPosition,this->basicProjectile);

    this->visuals->emplace_back(newShot);
}

void PlayerCharacter::render(Screen * screen) {
    if (shooting) shoot();
    this->shooting = false;
    this->velocity[0] = 0;
    this->velocity[1] = 0;

    this->renderState = this->collider(screen);
}
