#include <QImage>
#include "define.h"

class Sprite {
public:
    Sprite(int height, int width){
        int red[height][width];
        int green[height][width];
        int blue[height][width];
        int mesh[height][width];
    }
};

class Visual {
protected:
    int renderer(Sprite * screen);
};

class Particle : private Visual {
public:
    Particle(void ** sprite);
    void render();
};

class Character : private Visual {
public:
    Particle * loadParticle();
    void shoot();
    Character(int loadX, int loadY, int x, int y);
};
