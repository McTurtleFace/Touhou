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

class EnergyBall : public Particle {
public:
    uint32_t size;
    void render(Screen * screen) override;
    using Particle::Particle;
};

class EnergySpike : public Particle {
public:
    Sprite * rotatedSprite;
    void render(Screen * screen) override;
    void rotate(double rotation);
    using Particle::Particle;
};

class ScreenSpike : public Particle {
public:
    Sprite * warningSprite;
    bool warningFired = false;
    int timer = 0;
    void render(Screen * screen) override;
    void renderer(Screen * screen) override;
    using Particle::Particle;
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
