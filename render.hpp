#include <QImage>
#include <QLabel>
#include <QPainter>

#include "define.h"

class Sprite {
public:
    QImage image;
    Sprite(const char * imageFile);
};

class Screen : public QLabel{
public:
    QImage image;
    QLabel * label;
    bool collision[WIDTH][HEIGHT] = {{false}};
    /*Screen(const char * imageName){
        image.load(imageName);
        label.setPixmap(QPixmap::fromImage(image));
    }*/
};

class Visual {
public:     // needs to be reverted to protected
    //Visual(Sprite * sprite);
    int velocity[2] = {0};
    int position[2] = {0};
    Sprite * sprite;
    int renderer(Screen * screen);
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
