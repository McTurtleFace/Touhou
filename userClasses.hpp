#pragma once
#include "render.hpp"

class Kappa : public NonPlayerCharacter {
private:
    void shoot() override;
public:
    void render(Screen * screen) override;
    unsigned short collider(Screen * screen) override;
    using NonPlayerCharacter::NonPlayerCharacter;
};

class UFO : public NonPlayerCharacter {
private:
    void shoot() override;
public:
    bool shotYet = false;
    void render(Screen * screen) override;
    unsigned short collider(Screen * screen) override;
    using NonPlayerCharacter::NonPlayerCharacter;
};

class NierHack : public NonPlayerCharacter {
private:
    void shoot() override;
public:
    void render(Screen * screen) override;
    unsigned short collider(Screen * screen) override;
    using NonPlayerCharacter::NonPlayerCharacter;
};

class EnergyBall : public EnemyParticle {
public:
    uint32_t size;
    using EnemyParticle::EnemyParticle;
};

class EnergySpike : public EnemyParticle {
public:
    Sprite * rotatedSprite;
    void rotate(double rotation);
    using EnemyParticle::EnemyParticle;
};

class ScreenSpike : public EnemyParticle {
public:
    Sprite * warningSprite;
    bool warningFired = false;
    int timer = 0;
    void renderer(Screen * screen) override;
    using EnemyParticle::EnemyParticle;
};

class Reitu : public NonPlayerCharacter {
private:
    void shoot() override;
public:
    uint32_t health = 10;
    Sprite * rise;
    Sprite * set;
    void BinaryRise();
    void BinarySet();
    void render(Screen * screen) override;
    using NonPlayerCharacter::NonPlayerCharacter;
};
