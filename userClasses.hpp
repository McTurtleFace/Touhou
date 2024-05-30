#include "render.hpp"

class Kappa : public NonPlayerCharacter {
private:
    void shoot() override;
public:
    unsigned short render(Screen * screen) override;
    unsigned short collider(Screen * screen) override;
    using NonPlayerCharacter::NonPlayerCharacter;
};

class UFO : public NonPlayerCharacter {
private:
    void shoot() override;
public:
    unsigned short render(Screen * screen) override;
    unsigned short collider(Screen * screen) override;
    using NonPlayerCharacter::NonPlayerCharacter;
};

class NierHack : public NonPlayerCharacter {
private:
    void shoot() override;
public:
    unsigned short render(Screen * screen) override;
    unsigned short collider(Screen * screen) override;
    using NonPlayerCharacter::NonPlayerCharacter;
};

class EnergyBall : public Particle {
public:
    uint32_t size;
    unsigned short render(Screen * screen) override;
    using Particle::Particle;
};

class EnergySpike : public Particle {
public:
    unsigned short render(Screen * screen) override;
    using Particle::Particle;
};

class ScreenSpike : public Particle {
public:
    bool warningFired = false;
    int warningTimer = 0;
    unsigned short render(Screen * screen) override;
    void renderer(Screen * screen) override;
    using Particle::Particle;
};

class Reitu : public Boss {
public:
    uint32_t health = 10;
    void BinaryRise();
    void BinarySet();
    using Boss::Boss;
};
