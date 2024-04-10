#include <QImage>
#include "define.h"

class Sprite {
public:
    Sprite(char * imageFile){
        QImage image;
        image.load(imageFile);

        int mesh[image.height()][image.width()];

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
