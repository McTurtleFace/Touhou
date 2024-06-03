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
public:
    QImage image;
    QLabel * label;
/*
layers: 0-special items, 1-enemy project, 2-friendly project,
        3-enemy char, 4-friendly char, 5-top level
*/
    bool collision[6][WIDTH][HEIGHT] = {{{false}}};
};

class Visual {
public:
    int velocity[2] = {0};
    int position[2] = {0};
    virtual unsigned short render(Screen * screen);
protected:
    int oldPosition[2] = {0};
    QImage backgroundSnap;
    Sprite * sprite;
    void loadSprite(Sprite * sprite) {this->sprite = sprite;}
    virtual void renderer(Screen * screen);
    virtual unsigned short collider(Screen * screen);
};

class Particle : public Visual {
public:
    Particle(int velocityP[2], int positionP[2], Sprite * spriteP);
};

class Character : public Visual {
public:
    std::vector<Visual *> * visuals;
    Sprite * basicProjectile;
    Particle * loadParticle();
    virtual void shoot();
    QElapsedTimer * timer;
    qint64 spawnTime;
    Character(int velocityP[2], int positionP[2], Sprite * spriteP, Sprite * basicProjectileP, QElapsedTimer * timerP, std::vector<Visual *> * newVisuals);
};

class PlayerCharacter : public Character {
public:
    int lives = 3;
    unsigned short render(Screen * screen) override;
    unsigned short collider(Screen * screen) override;
    using Character::Character;
};

class NonPlayerCharacter : public Character {
public:
    qint64 timeAlive();
    unsigned short render(Screen * screen) override;
    using Character::Character;
};

class KeyEventHandler : public QObject
{
public:
    KeyEventHandler(Character* inputVisual) {
        this->visual = inputVisual;
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Character* visual;
};
