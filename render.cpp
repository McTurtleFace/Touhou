#include "render.hpp"
#include <iostream>

Sprite::Sprite(const char * imageFile){
    this->image.load(imageFile);


    int mesh[image.height()][image.width()];
    for (int i = 0; i<image.height(); i++){
        for (int j = 0; j < image.width(); j++) {
            if (image.pixel(j,i) == qRgb(0,0,0)) std::cout << j << i << std::endl;
            mesh[i][j] = (image.pixel(j,i) == qRgb(255,255,255)) ? 0 : 255;
        }
    }

    if (image.pixel(5,5) == qRgb(0,0,0)) std::cout << "white" << std::endl;

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
/*
Particle::Particle(void ** sprites) {
    //asdf
}

Visual::Visual(Sprite * sprite){

}
*/
int Visual::renderer(Screen * screen) {
    if (position[0] + velocity[0] < 0 &&
        position[0] + velocity[0] >= WIDTH &&
        position[1] + velocity[1] < 0 &&
        position[1] + velocity[1] >= HEIGHT) {
        return 2;
    }

    // detect if colliding
    for (int i = 0; i<sprite->image.height(); i++){
        for (int j = 0; j<sprite->image.width(); j++){
            if (sprite->image.pixelColor(j,i).alpha() == 1
                && screen->collision[position[0] + j][position[1] + i] == true){
                return 1;
            }
        }
    }

    // paint and render
    QPainter painter(&(screen->image));
    painter.drawImage(position[0],position[1],sprite->image);
    screen->label->setPixmap(QPixmap::fromImage(screen->image));
    screen->label->show();
    std::cout << sprite->image.pixelColor(3,3).alpha() << std::endl;

    return 0;
}

void Particle::render() {

}
