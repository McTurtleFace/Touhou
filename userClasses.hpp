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
    unsigned short render(Screen * screen) override;
    using Particle::Particle;
};

