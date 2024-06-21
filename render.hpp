#pragma once
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QThread>
#include <QObject>
#include <QKeyEvent>

#include "define.h"

class Sprite {
public:
    QImage image;
    QImage invertAlpha();
    Sprite(const char * imageFile);
};

class Screen : public QThread{
private:
    Sprite * boundingBox;
public:
    QImage backgroundSnap;
    bool renderSemaphore = false;
    QImage image;
    QLabel * label;
/*
layers: 0-special items, 1-enemy project, 2-friendly project,
        3-enemy char, 4-friendly char, 5-top level
*/
    bool collision[6][WIDTH][HEIGHT] = {{{false}}};
    void overlayBox();
    Screen(QLabel * labelP, const char * imageName, const char * boundName);
};

class Visual {
public:
    unsigned short renderState = 0;
    int velocity[2] = {0};
    int position[2] = {0};
    void nonThreadedRender(Screen * screen) { move();renderer(screen);}
    virtual void render(Screen * screen);
    virtual void derenderer(Screen * screen);
    virtual ~Visual() {}
protected:
    int oldPosition[2] = {0};
    QImage backgroundSnap;
    Sprite * sprite;
    void loadSprite(Sprite * sprite) {this->sprite = sprite;}
    void move();
    virtual void renderer(Screen * screen);
    virtual unsigned short collider(Screen * screen);
};

class Particle : public Visual {
public:
    void render(Screen * screen) override;
    char * shooter;
    Particle(int velocityP[2], int positionP[2], Sprite * spriteP);
};

class Character : public Visual {
public:
    std::vector<Visual *> * visuals;
    Sprite * basicProjectile;
    Particle * loadParticle();
    virtual void shoot() {}
    QElapsedTimer * timer;
    qint64 spawnTime;
    Character(int velocityP[2], int positionP[2], Sprite * spriteP, Sprite * basicProjectileP, QElapsedTimer * timerP, std::vector<Visual *> * newVisuals);
};

class PlayerCharacter : public Character {
private:
    void shoot() override;
public:
    bool shooting = false;
    int lives = 3;
    unsigned short collider(Screen * screen) override;
    void render(Screen * screen) override;
    using Character::Character;
};

class NonPlayerCharacter : public Character {
public:
    qint64 timeAlive();
    using Character::Character;
};

class KeyEventHandler : public QObject
{
public:
    KeyEventHandler(PlayerCharacter* inputVisual) {
        this->visual = inputVisual;
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    PlayerCharacter* visual;
};
